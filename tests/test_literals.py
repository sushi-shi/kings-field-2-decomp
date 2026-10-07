"""Controls for the literal sink census and its value-flow domains."""

from __future__ import annotations

from pathlib import Path
from tempfile import TemporaryDirectory
import unittest

from scripts.kf.literals import build_domains, collect, load_kf1
from scripts.kf.manifest import Manifest, Profile, Unit


def census(sources: dict[str, str], *, kf1: dict[str, str] | None = None,
           hub_degree: int = 12) -> dict:
    with TemporaryDirectory() as directory:
        repo = Path(directory) / "repo"
        for name, source in sources.items():
            path = repo / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(source)
        (repo / "include").mkdir(exist_ok=True)
        sdk = repo / "sdk"
        sdk.mkdir()
        kf1_root = None
        if kf1 is not None:
            kf1_root = Path(directory) / "kf1"
            for name, source in kf1.items():
                path = kf1_root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(source)
            (kf1_root / "src").mkdir(exist_ok=True)
        profile = Profile("c", "c", "gcc257-native", "O2", 0, "1.07", ())
        units = tuple(Unit(f"game.{Path(name).stem}", "GAME.EXE", name, "c", ())
                      for name in sources if name.endswith(".c"))
        return collect(repo=repo, sdk=sdk, manifest=Manifest({"c": profile}, units), jobs=1,
                       kf1_root=kf1_root, hub_degree=hub_degree)


def sinks(report: dict, line: int, file: str = "src/probe.c") -> list[tuple[str, str, int]]:
    return sorted((row["sink"]["kind"], row["sink"]["target"], row["value"])
                  for row in report["sites"] if row["file"] == file
                  and row["line"] == line and row["origin"] in {"literal", "character"})


def domain_of(report: dict, slot: str) -> dict:
    matches = [domain for domain in report["domains"]
               if slot in {member["slot"] for member in domain["members"]}]
    if len(matches) != 1:
        raise AssertionError((slot, matches))
    return matches[0]


HEADER = """
#define ACTION_LIMIT 0x40
#define TWICE(value) ((value) * 2)
enum { KIND_DOOR = 5 };
typedef struct Template { unsigned char kind; unsigned char flags; } Template;
typedef struct Object { unsigned char action; short timer; int *data; } Object;
typedef char template_size[sizeof(Template) == 2 ? 1 : -1];
void start(Object *object, unsigned char action);
int lookup(int index);
extern int table[16];
"""


class LiteralCensusTests(unittest.TestCase):
    def test_each_literal_names_its_sink(self) -> None:
        report = census({"include/probe.h": HEADER, "src/probe.c": """#include <probe.h>
int table[16];
int probe(Object *object, const Template *definition) {
    object->action = 5;
    if (object->action == 7) start(object, 9);
    switch (definition->kind) { case 0x58: return 3; default: break; }
    object->timer += 30;
    object->timer = table[4] << 2;
    object->data = 0;
    if (definition->flags & 0x20) return -1;
    return TWICE(6) + ACTION_LIMIT;
}
"""})
        self.assertEqual(sinks(report, 4), [("assign", "Object.action", 5)])
        self.assertEqual(sinks(report, 5), [("argument", "start:arg1", 9),
                                            ("compare", "Object.action", 7)])
        self.assertEqual(sinks(report, 6), [("case", "Template.kind", 0x58),
                                            ("return", "probe:return", 3)])
        self.assertEqual(sinks(report, 7), [("arithmetic", "Object.timer", 30)])
        self.assertEqual(sinks(report, 8), [("index", "[table]", 4), ("shift", "table", 2)])
        self.assertEqual(sinks(report, 9), [("pointer-zero", "Object.data", 0)])
        self.assertEqual(sinks(report, 10), [("mask", "Template.flags", 0x20),
                                             ("return", "probe:return", -1)])
        # A macro argument is a written literal; a macro body is a named constant.
        self.assertEqual(sinks(report, 11), [("arithmetic", "", 6)])
        macro = [row for row in report["sites"] if row["origin"] == "macro-constant"]
        self.assertEqual(sorted((row["constant"], row["value"], row["line"]) for row in macro),
                         [("ACTION_LIMIT", 0x40, 11), ("TWICE", 2, 11)])
        classes = {row["sink"]["kind"] for row in report["sites"]
                   if row["file"] == "include/probe.h"}
        self.assertEqual(classes, {"declaration", "enum-definition"})

    def test_flow_joins_one_domain_and_quantities_stay_apart(self) -> None:
        report = census({"include/probe.h": HEADER, "src/probe.c": """#include <probe.h>
void start(Object *object, unsigned char action) {
    if (object->action == 0xff) object->action = action;
}
void load(Object *object, const Template *definition) {
    switch (definition->kind) {
    case 2:
        object->action = 2;
        break;
    case 3:
        object->action = 3;
        break;
    case 4:
        object->timer = 4;
        break;
    case KIND_DOOR:
        start(object, definition->kind);
        break;
    }
}
void tick(Object *object, int frames) {
    object->timer = frames * 3;
    if (object->timer == 4) object->timer = 0;
}
int count(int frames) { return frames; }
"""})
        action = domain_of(report, "Object.action")
        self.assertEqual({member["slot"] for member in action["members"]},
                         {"Object.action", "Template.kind", "start:arg1"})
        self.assertEqual([row["value"] for row in action["values"]], [2, 3, 4, 0xff])
        self.assertEqual([row["name"] for row in action["constants_used"]], ["KIND_DOOR"])
        self.assertEqual(action["verdict"], "enum-candidate")
        self.assertIn("case-echo=3", " ".join(action["edges"]))
        # One coincidental echo (case 4 into the timer) does not join a domain.
        self.assertNotIn("Object.timer", {member["slot"] for member in action["members"]})
        timer = domain_of(report, "Object.timer")
        self.assertEqual(timer["verdict"], "quantity")
        self.assertNotIn("tick:arg1", {member["slot"] for member in timer["members"]})

    def test_hubs_do_not_weld_domains(self) -> None:
        sites = [{"sink": {"kind": "assign", "target": name}, "value": value,
                  "origin": "literal", "constant": "", "constant_file": "", "class": "assign",
                  "function": "f", "file": "src/a.c", "line": 1}
                 for name, value in (("A.a", 1), ("B.b", 2), ("C.c", 3))]
        edges = [{"left": "A.a", "right": "hub", "kind": "copy", "file": "src/a.c", "line": 1},
                 {"left": "B.b", "right": "hub", "kind": "copy", "file": "src/a.c", "line": 2},
                 {"left": "C.c", "right": "hub", "kind": "copy", "file": "src/a.c", "line": 3}]
        domains, hubs = build_domains(sites, edges, {}, set(), {}, hub_degree=2)
        self.assertEqual(hubs, ["hub"])
        self.assertEqual(len(domains), 3)
        domains, hubs = build_domains(sites, edges, {}, set(), {}, hub_degree=3)
        self.assertEqual((len(domains), hubs), (1, []))

    def test_kf1_counterparts_prefer_qualified_slots(self) -> None:
        kf1 = {"include/kf/map.h": """
KF_ENUM_BEGIN(KfMapObjectOperation, u8)
    KF_MAP_OBJECT_OP_LIFT_DOOR = 2,
    KF_MAP_OBJECT_OP_NONE = 255
KF_ENUM_END(KfMapObjectOperation)
KF_ENUM_BEGIN(KfActorAction, u8)
    KF_ACTOR_ACTION_IDLE = 0,
    KF_ACTOR_ACTION_WALK = 2
KF_ENUM_END(KfActorAction)
typedef struct Actor { KF_ENUM_STORAGE(KfActorAction, u8) action; } Actor;
typedef struct Object { KfMapObjectOperation action; /* runtime */ } Object;
void start(Object *object, KfMapObjectOperation action);
"""}
        with TemporaryDirectory() as directory:
            root = Path(directory)
            for name, text in kf1.items():
                (root / name).parent.mkdir(parents=True, exist_ok=True)
                (root / name).write_text(text)
            (root / "src").mkdir()
            loaded = load_kf1(root)
        self.assertEqual(loaded["qualified"]["Object.action"], ["KfMapObjectOperation"])
        self.assertEqual(loaded["qualified"]["Actor.action"], ["KfActorAction"])
        self.assertEqual(loaded["qualified"]["start:arg1"], ["KfMapObjectOperation"])
        self.assertEqual(loaded["enums"]["KfMapObjectOperation"]["members"],
                         {"KF_MAP_OBJECT_OP_LIFT_DOOR": 2, "KF_MAP_OBJECT_OP_NONE": 255})
        report = census({"include/probe.h": HEADER, "src/probe.c": """#include <probe.h>
void reset(Object *object) { object->action = 0xff; object->action = 2; }
"""}, kf1=kf1)
        best = domain_of(report, "Object.action")["kf1"][0]
        self.assertEqual(best["enum"], "KfMapObjectOperation")
        self.assertEqual(best["value_overlap"], 2)


if __name__ == "__main__":
    unittest.main()

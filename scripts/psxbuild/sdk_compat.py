"""Compatibility corrections to pinned SDK linker inputs."""


def library_input(name: str, data: bytes) -> tuple[bytes, list[dict]]:
    """Return a linker input and the corrections applied to it.

    The Psy-Q 3.0 archives are linked unchanged. The King's Field (SLPS-00017)
    project's Release 2.5 LIBETC interrupt-return correction does not carry
    over: it is bound to that archive's bytes and to a runtime failure not
    reproduced here.
    """
    return data, []

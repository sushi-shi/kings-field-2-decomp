"""The DOS tool runner must not let host speed schedule emulated interrupts."""

from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest import mock

from scripts.psxbuild import sdk


class DosRunTests(unittest.TestCase):
    def test_emulated_cpu_rate_is_fixed(self):
        with tempfile.TemporaryDirectory() as directory:
            completed = subprocess.CompletedProcess([], 0, b'', b'')
            with mock.patch.object(sdk.subprocess, 'run', return_value=completed) as run:
                sdk.dos_run(Path(directory), ['cpe2x GAME.CPE > CONVERT.TXT'], 'convert')
        arguments = run.call_args.args[0]
        settings = [arguments[i + 1] for i, value in enumerate(arguments) if value == '-set']
        cycles = [value for value in settings if value.startswith('cpu cycles=')]
        self.assertEqual(cycles, [f'cpu cycles={sdk.DOS_CYCLES}'])
        self.assertRegex(sdk.DOS_CYCLES, r'^fixed [1-9][0-9]*$')


if __name__ == '__main__':
    unittest.main()

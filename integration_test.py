import subprocess, sys, os
import re
from typing import Tuple

RED   = "\033[1;31m"
GREEN = "\033[0;32m"
RESET = "\033[0;0m"

DEFAULT_EXECUTABLE = "./main" if os.name != 'nt' else 'main.exe'
executable = DEFAULT_EXECUTABLE

tests = [
	['x3+4x2-3x-18', -3, 2],
	['18x4+9x7-4x5+x3-5x2+50x-100', 1.15],
	['x7+10x6+27x5-57x3-30x2+29x+20', 1, -1, -5, -4],
	['x6+3x5-41x4-87x3+400x2+444x-720', 1, -2, 3, -4, 5, -6],
	['x3+40.5x2+399.5x-210', 0.5, -20, -21],
	['x4-10x3+35x2-50x+24', 1, 2, 3, 4],
	['x2-10000', -100, 100],
	['x2-1000000000000', -1000000, 1000000]
]

max_input_length = max([len(x[0]) for x in tests])

def get_output(input: str, executable: str) -> str:
	"""Starts an executable, passes input into its STDIN and returns STDOUT"""
	input = input.encode('utf-8')
	result = subprocess.run([executable], stdout=subprocess.PIPE, input=input)
	output = result.stdout.decode('utf-8')
	return output

def check_roots(input: str, *roots: Tuple[float]) -> bool:
	regex = r'^Root: x = (\S+)'
	output = get_output(input, executable)
	roots = [float(x) for x in roots]
	matches = re.findall(regex, output, re.MULTILINE)
	matches = [float(x) for x in matches]
	matches = [round(x, 2) for x in matches]
	same = set(matches) == set(roots)
	print('{input} {success}'
		.format(input=input.ljust(max_input_length), success=(GREEN + 'success' + RESET if same
		else RED + 'fail' + RESET)))
	if not same:
		print('\tExpected:\t{expected}\n\tActual:  \t{actual}'
			.format(expected=sorted(list(roots)), actual=sorted(matches)),
			file=sys.stderr)
	return same

if __name__ == '__main__':
	if len(sys.argv) >= 2:
		executable = sys.argv[1]
	else:
		executable = DEFAULT_EXECUTABLE

	if not all([check_roots(x[0], *x[1:]) for x in tests]):
		sys.exit(1)
	sys.exit(0)

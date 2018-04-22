import os
import subprocess
import sys
import typing as t

import frontdoor

ROOT = os.path.dirname(os.path.realpath(__file__))
PROJECT = 'Lp3-Engine'
SOURCE_DIR = os.path.join(ROOT, 'source')
BUILD_DIR = os.path.join(ROOT, 'build')

REGISTRY = frontdoor.CommandRegistry('fd-ci')
cmd = REGISTRY.decorate


def from_root(path: str) -> str:
    """Returns a path relative to the root directory."""
    if os.name == 'nt':
        path = path.replace('/', '\\')
    return os.path.join(ROOT, path)


def sphinx_build(op: str='html') -> int:
    cmd = f'sphinx-build -M {op} {SOURCE_DIR} {BUILD_DIR}'
    return subprocess.call(cmd, shell=True)


@cmd('build', 'Build docs')
def build(args: t.List[str]) -> int:
    return sphinx_build()


if __name__ == "__main__":
    args = [arg.strip() for arg in sys.argv[1:]]
    result = REGISTRY.dispatch(args)
    sys.exit(result)

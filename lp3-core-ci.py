import os
import shutil
import subprocess
import sys

import frontdoor


REGISTRY = frontdoor.CommandRegistry('lp3-core-ci')
cmd = REGISTRY.decorate
ROOT=os.path.dirname(os.path.realpath(__file__))


def from_root(path):
    """Returns a path relative to the root directory."""
    if os.name == 'nt':
        path = path.replace('/', '\\')
    return os.path.join(ROOT, path)


def mkdir(path):
    if not os.path.exists(path):
        os.makedirs(path)


BUILD_DIR = from_root('build')
SRC_DIR = from_root('standalone')


@cmd('clean', 'Wipes out build directory.')
def clean(args):
    mkdir(BUILD_DIR)
    shutil.rmtree(BUILD_DIR)
    print(' * spotless! *')


@cmd('windows', 'Build on Windows')
def windows(args):
    mkdir(BUILD_DIR)
    code = subprocess.call(
        'cmake -G "Visual Studio 14 2015 Win64" -H{src_dir} -B{build_dir}'
            .format(src_dir=SRC_DIR, build_dir=BUILD_DIR),
        shell=True,
        cwd=BUILD_DIR)
    if code:
        return code

    code2 = subprocess.call(
        'cmake --build {}'.format(BUILD_DIR),
        shell=True,
        cwd=BUILD_DIR)
    if code2:
        return code2

    new_env = os.environ.copy()
    new_env['LP3_ROOT_PATH'] = os.path.join(SRC_DIR, 'media')
    return subprocess.call(
        'ctest -C "Debug"',
        shell=True,
        cwd=BUILD_DIR,
        env=new_env)


if __name__ == "__main__":
    # Fix goofy bug when using Windows command prompt to ssh into Vagrant box
    # that puts \r into the strings.
    args = [arg.strip() for arg in sys.argv[1:]]
    result = REGISTRY.dispatch(args)
    sys.exit(result)

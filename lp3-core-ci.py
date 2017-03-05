import argparse
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
    registry = frontdoor.CommandRegistry('windows')
    c = registry.decorate

    wb_dir = os.path.join(BUILD_DIR, 'windows')

    @c('clean')
    def clean(args):
        mkdir(wb_dir)
        shutil.rmtree(wb_dir)
        print(' * clean! *')

    @c('config')
    def config(args):
        bits = '64' if len(args) < 1 else args[0]
        mkdir(wb_dir)
        return subprocess.call(
            'cmake -G "Visual Studio 14 2015 Win{bits}" -H{src_dir} -B{build_dir}'
                .format(src_dir=SRC_DIR, build_dir=wb_dir, bits=bits),
            shell=True,
            cwd=wb_dir)

    def build(btype):
        new_env = os.environ.copy()
        new_env['LP3_ROOT_PATH'] = os.path.join(SRC_DIR, 'media')

        return (
            config([])
            or
            subprocess.call(
                'cmake --build {} --config {} '.format(
                    wb_dir, btype),
                shell=True,
                cwd=wb_dir)
            or
            subprocess.call(
                'ctest -C "{}"'.format(btype),
                shell=True,
                cwd=wb_dir,
                env=new_env)
        )

    @c('debug')
    def debug(args):
        return build('Debug')

    @c('release')
    def release(args):
        return build('Release')

    @c('both')
    def both(args):
        return debug(args) or release(args)

    return registry.dispatch(args)


if __name__ == "__main__":
    # Fix goofy bug when using Windows command prompt to ssh into Vagrant box
    # that puts \r into the strings.
    args = [arg.strip() for arg in sys.argv[1:]]
    result = REGISTRY.dispatch(args)
    sys.exit(result)

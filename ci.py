import argparse
import os
import shutil
import subprocess
import sys

import frontdoor


REGISTRY = frontdoor.CommandRegistry('ci')
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
    wb_dir = os.path.join(BUILD_DIR, 'windows')

    registry = frontdoor.CommandRegistry('windows')
    c = registry.decorate

    @c('clean')
    def clean(args):
        mkdir(wb_dir)
        shutil.rmtree(wb_dir)
        print(' * clean! *')

    @c('build')
    def build(args):
        parser = argparse.ArgumentParser(prog='windows',
                                         description='Builds the program')
        parser.add_argument('--bits', type=int, help='64 or 32', default=64)
        parser.add_argument('--type', type=str, help='debug / release',
                            default='debug')

        ap_args = parser.parse_args(args=args)

        print('#' * 80)
        print('# Building {} bit in {} mode'.format(
            ap_args.bits, ap_args.type))
        print('#' * 80)

        wb_dir = os.path.join(BUILD_DIR, 'windows', str(ap_args.bits))

        mkdir(wb_dir)

        bit_str = {64: ' Win64', 32: ''}[ap_args.bits]

        new_env = os.environ.copy()
        new_env['LP3_ROOT_PATH'] = os.path.join(SRC_DIR, 'media')

        return any((
            subprocess.call(
                'cmake -G "Visual Studio 14 2015{bits}" -H{src_dir} -B{build_dir}'
                    .format(src_dir=SRC_DIR, build_dir=wb_dir, bits=bit_str),
                shell=True,
                cwd=wb_dir),
            subprocess.call(
                'cmake --build {} --config {} '.format(
                    wb_dir, ap_args.type),
                shell=True,
                cwd=wb_dir),
            subprocess.call(
                'ctest -C "{}"'.format(ap_args.type),
                shell=True,
                cwd=wb_dir,
                env=new_env),
        ))

    @c('build-all')
    def build_all(args):
        return any((
            build(['--bits=32', '--type=debug']),
            build(['--bits=32', '--type=release']),
            build(['--bits=64', '--type=debug']),
            build(['--bits=64', '--type=release']),
        ))

    return registry.dispatch(args)


if __name__ == "__main__":
    # Fix goofy bug when using Windows command prompt to ssh into Vagrant box
    # that puts \r into the strings.
    args = [arg.strip() for arg in sys.argv[1:]]
    result = REGISTRY.dispatch(args)
    sys.exit(result)

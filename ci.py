
import argparse
import os
import shutil
import subprocess
import sys
import time
import zipfile

import frontdoor


REGISTRY = frontdoor.CommandRegistry('ci')
cmd = REGISTRY.decorate
ROOT=os.path.dirname(os.path.realpath(__file__))


def mkdir(path):
    if not os.path.exists(path):
        os.makedirs(path)


def from_root(path):
    """Returns a path relative to the root directory."""
    if os.name == 'nt':
        path = path.replace('/', '\\')
    return os.path.join(ROOT, path)


BUILD_DIR = from_root('build')
DEPS_DIR = from_root('build/deps')
SRC_DIR = from_root("""standalone""")


@cmd('clean', 'Wipes out build directory.')
def clean(args):
    mkdir(BUILD_DIR)
    shutil.rmtree(BUILD_DIR)
    print(' * spotless! *')


@cmd('deps', 'Downloads needed libraries to build standalone')
def deps(args):
    mkdir(BUILD_DIR)
    mkdir(DEPS_DIR)

    platform = None
    if len(args) > 1:
        platform = args[0]

    print('Assuming Boost is already available.')


@cmd('ubuntu', 'Build on Ubuntu')
def ubuntu(args):
    wb_dir = os.path.join(BUILD_DIR, 'ubuntu')

    registry = frontdoor.CommandRegistry('ubuntu')
    c = registry.decorate

    @c('clean')
    def clean(args):
        mkdir(wb_dir)
        shutil.rmtree(wb_dir)
        print(' * clean! *')

    @c('build')
    def build(args):
        parser = argparse.ArgumentParser(prog='ubuntu',
                                         description='Builds the program')
        parser.add_argument('--ntfs-work-around', type=bool, default=False)
        parser.add_argument('--shared', type=bool, default=False)
        parser.add_argument('--type', type=str, help='debug / release',
                            default='debug')
        parser.add_argument('--compiler', type=str, help='gcc/clang',
                            default='gcc')
        parser.add_argument('--clang-tidy', type=bool, default=False)
        parser.add_argument('--clean', type=bool, default=False)
        ap_args = parser.parse_args(args=args)

        build_subdir = os.path.join(
            'ubuntu', ap_args.compiler, ap_args.type, 'shared' if ap_args.shared else 'static')
        print('#' * 80)
        print('# Building {}'.format(build_subdir))
        print('#' * 80)

        wb_dir = os.path.join(BUILD_DIR, build_subdir)

        cc = {'gcc': 'gcc-6',
              'clang': 'clang-3.8'}[ap_args.compiler]
        cxx = {'gcc': 'g++-6',
               'clang': 'clang++-3.8'}[ap_args.compiler]

        new_env = os.environ.copy()
        new_env['CC'] = subprocess.check_output('which {}'.format(cc), shell=True).strip()
        new_env['CXX'] = subprocess.check_output('which {}'.format(cxx), shell=True).strip()
        new_env['LP3_ROOT_PATH'] = os.path.join(SRC_DIR, 'media')

        options = []
        options.append('--define CMAKE_BUILD_TYPE={}'.format(
            {'debug': 'Debug', 'release': 'Release'}[ap_args.type]))
        if ap_args.shared:
            options.append('--define BUILD_SHARED_LIBS=true')
        if ap_args.clang_tidy:
            # TODO: add -warnings-as-errors with clang-tidy-4
            options.append('--define CMAKE_CXX_CLANG_TIDY:STRING="clang-tidy-3.8;-checks=-*,readability-*,-std=c++14"')

        if ap_args.clean:
            print(' * spotless! *')
            shutil.rmtree(wb_dir)

        mkdir(wb_dir)

        with open('{}/requirements.txt'.format(SRC_DIR), 'r') as f:
            reqs = f.read()
        reqs = reqs.replace('\n', ' ')

        return (
            subprocess.call(
                'cget init {options} --prefix={wb_dir}/cget'.format(
                    options=' '.join(options),
                    wb_dir=wb_dir),
                cwd=wb_dir,
                shell=True,
                )
            or subprocess.call(
                'cget install {}'.format(reqs),
                shell=True,
                cwd=wb_dir,
                )
            or subprocess.call(
                'cmake -G "Ninja" '
                '-DCMAKE_TOOLCHAIN_FILE={build_dir}/cget/cget/cget.cmake '
                '-H{src_dir} -B{build_dir}'
                    .format(options=' '.join(options), src_dir=SRC_DIR,
                            build_dir=wb_dir),
                shell=True,
                cwd=wb_dir,
                env=new_env)
            or subprocess.call(
                'cmake --build {}'.format(wb_dir),
                shell=True,
                cwd=wb_dir)
            or subprocess.call(
                'ctest -C "{}"'.format(ap_args.type),
                shell=True,
                cwd=wb_dir,
                env=new_env)
        )

    @c('build-all')
    def build_all(args):
        prefix = []
        if '--ntfs-work-around=true' in args:
            prefix = ['--ntfs-work-around=true']

        for c in ('gcc', 'clang'):
            for t in ('debug', 'release'):
                for s in ('--shared=true', ''):
                    b_args = prefix + [
                        '--type={}'.format(t), '--compiler={}'.format(c)]
                    if s:
                        b_args.append(s)
                    result = build(b_args)
                    if result:
                        return result
        return 0

    return registry.dispatch(args)


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
        arch = {64: 'x64', 32: 'x86'}[ap_args.bits]

        # The following environment variables are needed by the tests.
        new_env = os.environ.copy()
        new_env['PATH'] += ';{}'.format(from_root('build/deps/sdl2/lib'))
        new_env['LP3_ROOT_PATH'] = os.path.join(SRC_DIR, 'media')


        with open('{}/requirements.txt'.format(SRC_DIR), 'r') as f:
            reqs = f.read()
        reqs = reqs.replace('\n', ' ')

        return (
            subprocess.call(
                'cget init --prefix={wb_dir}/cget '
                '--define CMAKE_GENERATOR_PLATFORM={arch}'.format(
                    arch=arch,
                    wb_dir=wb_dir),
                cwd=wb_dir,
                shell=True,
                )
            or subprocess.call(
                'cget install --{type} {reqs}'.format(
                    type=ap_args.type, reqs=reqs),
                shell=True,
                cwd=wb_dir,
                )
            or subprocess.call(
                'cmake -G "Visual Studio 14 2015{bits}" -H{src_dir} -B{build_dir}'
                    .format(src_dir=SRC_DIR, build_dir=wb_dir, bits=bit_str),
                shell=True,
                cwd=wb_dir)
            or subprocess.call(
                'cmake --build {} --config {} '.format(
                    wb_dir, ap_args.type),
                shell=True,
                cwd=wb_dir)
            or subprocess.call(
                'ctest -C "{}"'.format(ap_args.type),
                shell=True,
                cwd=wb_dir,
                env=new_env)
        )

    @c('build-all')
    def build_all(args):
        # It's not worth it to me to get 32 bit versions of SDL and Glew
        # onto Travis, so just ignore these for now.
        return (
            build(['--bits=64', '--type=debug'])
            or build(['--bits=64', '--type=release'])
        )

    return registry.dispatch(args)


@cmd('travis', 'Run Travis CI tasks')
def travis(args):
    return (
        deps(['linux'])
        or ubuntu(['build-all'])
    )


if __name__ == "__main__":
    # Fix goofy bug when using Windows command prompt to ssh into Vagrant box
    # that puts \r into the strings.
    args = [arg.strip() for arg in sys.argv[1:]]
    result = REGISTRY.dispatch(args)
    sys.exit(result)
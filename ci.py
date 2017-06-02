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
SRC_DIR = from_root('standalone')


def github_check_call(*args, **kwargs):
    # Github can fail requests due to rate limiting.
    for attempts in range(10):
        try:
            return subprocess.check_call(*args, **kwargs)
        except:
            print('Github call failed, retrying...')
            time.sleep(1)
    raise RuntimeError('Failed to call GitHub!')


@cmd('clean', 'Wipes out build directory.')
def clean(args):
    mkdir(BUILD_DIR)
    shutil.rmtree(BUILD_DIR)
    print(' * spotless! *')


@cmd('deps', 'Downloads needed libraries to build standalone')
def deps(args):
    mkdir(BUILD_DIR)

    deps = os.path.join(BUILD_DIR, 'deps')
    mkdir(deps)

    platform = None
    if len(args) > 1:
        platform = args[0]

    print('Checking out GSL...')
    gsl_dir = os.path.join(deps, 'gsl')
    mkdir(gsl_dir)
    github_check_call([
        'git', 'clone',
        'https://github.com/Microsoft/GSL.git',
        gsl_dir
    ])
    subprocess.check_call([
        'git', 'checkout',
        'c2f953f2eb7ab501325a7ec5656b400d54b8a345'
    ], cwd=gsl_dir)
    
    
    print('Assuming Boost is already available.')
    
    
    if not platform or platform == 'windows':
        sdl2_url = 'https://9988350550b21bab76e1-e4f58d473b8b67b7df073d18b2ddc43c.ssl.cf1.rackcdn.com/sdl2-win-cmake.zip'
        subprocess.check_call(['curl', '-o', 'sdl2.zip', sdl2_url], cwd=BUILD_DIR)
    
        name = 'sdl2'
        url = sdl2_url
        zip_name = '{}.zip'.format(name)
        print('Downloading {}...'.format(zip_name))
        subprocess.check_call(['curl', '-o', zip_name, url], cwd=BUILD_DIR)
    
        try:
            ZipFile = zipfile.Zipfile  # py2
        except AttributeError:
            ZipFile = zipfile.ZipFile  # py3
    
        z = ZipFile(os.path.join(BUILD_DIR, zip_name))
        z.extractall(os.path.join(deps, name))
    
    print('Downloading SDL2 Cmake Script from TwinklebearDev')
    cmake_scripts = os.path.join(deps, 'cmake')
    mkdir(cmake_scripts)
    find_sdl_url = 'https://raw.githubusercontent.com/Twinklebear/TwinklebearDev-Lessons/master/cmake/FindSDL2.cmake'
    github_check_call(['curl', '-o', 'FindSDL2.cmake', find_sdl_url],
                      cwd=cmake_scripts)
    
    
    
    
    print('Checking out Catch...')
    catch_dir = os.path.join(deps, 'Catch')
    mkdir(catch_dir)
    github_check_call([
        'git', 'clone',
        'https://github.com/philsquared/Catch.git',
        catch_dir
    ])
    subprocess.check_call([
        'git', 'checkout', 'v1.8.2'
    ], cwd=catch_dir)


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
        options.append('-DCMAKE_BUILD_TYPE={}'.format(
            {'debug': 'Debug', 'release': 'Release'}[ap_args.type]))
        if ap_args.shared:
            options.append('-DBUILD_SHARED_LIBS=true')
        if ap_args.clang_tidy:
            # TODO: add -warnings-as-errors with clang-tidy-4
            options.append('-DCMAKE_CXX_CLANG_TIDY:STRING="clang-tidy-3.8;-checks=-*,readability-*,-std=c++14"')

        # This is annoying. It exists only so I can use Bash for Ubuntu
        # for Windows (aka Bfufw) as Git will try to check out a file with
        # a colon there, screwing up the CMake call.
        if ap_args.ntfs_work_around:
            options.append('-DGSL_CHECKED_OUT=true')

        if ap_args.clean:
            print(' * spotless! *')
            shutil.rmtree(wb_dir)

        mkdir(wb_dir)

        return (
            subprocess.call(
                'cmake -G "Ninja" {options} -H{src_dir} -B{build_dir}'
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

        # The following environment variables are needed by the tests.
        new_env = os.environ.copy()
        new_env['PATH'] += ';{}'.format(from_root('build/deps/sdl2/lib'))
        new_env['LP3_ROOT_PATH'] = os.path.join(SRC_DIR, 'media')


        return (
            subprocess.call(
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
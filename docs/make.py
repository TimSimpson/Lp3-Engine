import glob
import os
import re
import subprocess
import shutil
import sys
import tempfile
import typing as t

import frontdoor
import cpp_rst

ROOT = os.path.dirname(os.path.realpath(__file__))
PROJECT = 'Lp3-Engine'
SOURCE_DIR = os.path.join(ROOT, 'source')
OUTPUT_DIR = os.path.join(ROOT, 'output')
GEN_SOURCE_DIR = os.path.join(OUTPUT_DIR, 'gen')
BUILD_DIR = os.path.join(OUTPUT_DIR, 'build')

DUMPFILE_RE = re.compile('~dumpfile "([^"]*)"')
DUMPFILE_RE_2 = re.compile('~dumpfile "([^"]*)" ?(.*)$')

REGISTRY = frontdoor.CommandRegistry('fd-ci')
cmd = REGISTRY.decorate



def from_root(path: str) -> str:
    """Returns a path relative to the root directory."""
    if os.name == 'nt':
        path = path.replace('/', '\\')
    return os.path.join(ROOT, path)


def convert_md_to_rst(mark_down_abs_path: str, rst_file_rel_path: str) -> None:
    rst_abs_path = os.path.join(GEN_SOURCE_DIR, rst_file_rel_path)
    cmd = (f'pandoc {mark_down_abs_path} --from markdown '
           f'--to rst -s -o {rst_abs_path} --wrap=none')
    subprocess.check_call(cmd, shell=True)


def make_temp_rst_from_md(lines):
    with tempfile.TemporaryDirectory() as tmpdir:
        md_file = os.path.join(tmpdir, 'input.md')
        rst_file = os.path.join(tmpdir, 'output.rst')
        with open(md_file, 'w') as w:
            for line in lines:
                w.write(line)

        convert_md_to_rst(md_file, rst_file)
        with open(rst_file) as r:
            return [l.rstrip() for l in r.readlines()]


def dump_file(input_file, start, end, indent, section, write_stream) -> None:
    print(f' ^---- dumpfile {input_file} {start} {end} {indent} {section}')
    with open(input_file, 'r') as r:
        lines = r.readlines()

    if start and end:
        subset = lines[start: end]
    elif start:
        subset = lines[start:]
    else:
        subset = lines


    prefix = ' ' * indent

    if input_file.endswith('.md'):
        final_lines = make_temp_rst_from_md(subset)
    elif input_file.endswith('.hpp') or input_file.endswith('.cpp'):
        final_lines = cpp_rst.translate_cpp_file(subset, section)
        # final_lines = make_temp_rst_from_md(cpp_md)
    else:
        final_lines = [prefix + l.rstrip() for l in subset]

    write_stream.write('\n'.join(final_lines))


def dumpfile_directive(current_source, matches, write_stream):
    input_file, rest = matches.groups(0)
    args = rest.split(' ')
    kwargs = {
        'start': None,
        'end': None,
        'indent': None,
        'section': None,
    }
    pos_arg_indices = ['start', 'end', 'indent']
    pos_arg_index = 0
    for arg in args:
        if '=' in arg:
            name, value = arg.split('=', 1)
        else:
            name = pos_arg_indices[pos_arg_index]
            pos_arg_index +=1
            value = arg

        if name not in kwargs:
            raise RuntimeError(
                f'Unknown dumpfile arg: {name}')
        if kwargs[name] is not None:
            raise RuntimeError(
                f'dumpfile arg {name} set twice')

        kwargs[name] = value

    if kwargs['end'] == '~':
        kwargs['end'] = None

    def intify(arg_name):
        if kwargs[arg_name]:
            kwargs[arg_name] = int(kwargs[arg_name])
        else:
            kwargs[arg_name] = 0

    intify('start')
    intify('end')
    intify('indent')

    full_input_file = os.path.join(
        os.path.dirname(current_source), input_file)
    print(kwargs)
    dump_file(full_input_file, write_stream=write_stream, **kwargs)


def parse_m_rst(source, dst):
    with open(dst, 'w') as w:
        with open(source, 'r') as f:
            for line in f.readlines():
                matches = DUMPFILE_RE_2.match(line)
                if matches:
                    dumpfile_directive(source, matches, w)
                else:
                    w.write(f'{line}')


def copy_rst_files(source, dst):
    rel = os.path.relpath(dst, source)
    print(rel)
    for file in glob.iglob(f'{source}/**/*', recursive=True):
        if os.path.isfile(file):
            print(file)
            rel_path = file[len(source):]
            print(rel_path)
            if rel_path.startswith(os.sep):
                rel_path = rel_path[1:]
            print(rel_path)
            to_path = os.path.join(dst, rel_path)
            if file.endswith('.rst'):
                print(f'{file} -> {to_path}')
                shutil.copy(file, to_path)
            elif file.endswith('.mrst'):
                print(f'parse {file} -> {to_path}')
                parse_m_rst(file, to_path)


def sphinx_build(op: str='html') -> int:
    cmd = f'sphinx-build -M {op} {GEN_SOURCE_DIR} {BUILD_DIR}'
    return subprocess.call(cmd, shell=True)


@cmd('build', 'Build docs')
def build(args: t.List[str]) -> int:
    try:
        shutil.rmtree(GEN_SOURCE_DIR)
    except FileNotFoundError:
        pass

    os.makedirs(GEN_SOURCE_DIR, exist_ok=True)
    os.makedirs(BUILD_DIR, exist_ok=True)

    shutil.copy(os.path.join(SOURCE_DIR, 'conf.py'),
                os.path.join(GEN_SOURCE_DIR, 'conf.py'),)

    copy_rst_files(SOURCE_DIR, GEN_SOURCE_DIR)
    # return 1
    # # shutil.copytree(SOURCE_DIR, GEN_SOURCE_DIR)

    # convert_md_to_rst(
    #     os.path.join(ROOT, '../README.md'), 'readme.rst')
    # convert_md_to_rst(
    #     os.path.join(ROOT, '../core/README.md'), 'core-readme.rst')
    # # convert_md_to_rst(
    # #     os.path.join(ROOT, '../core/README.md'), 'core-readme.rst')
    return sphinx_build()


if __name__ == "__main__":
    args = [arg.strip() for arg in sys.argv[1:]]
    result = REGISTRY.dispatch(args)
    sys.exit(result)

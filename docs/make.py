import glob
import os
import re
import subprocess
import shutil
import sys
import tempfile
import typing as t

import frontdoor

ROOT = os.path.dirname(os.path.realpath(__file__))
PROJECT = 'Lp3-Engine'
SOURCE_DIR = os.path.join(ROOT, 'source')
OUTPUT_DIR = os.path.join(ROOT, 'output')
GEN_SOURCE_DIR = os.path.join(OUTPUT_DIR, 'gen')
BUILD_DIR = os.path.join(OUTPUT_DIR, 'build')

DUMPFILE_RE = re.compile('~dumpfile "([^"]*)"')
DUMPFILE_RE_2 = re.compile(
    '~dumpfile "([^"]*)" ?([0-9]*)? ?([0-9~]*)? ?([0-9]*)?')

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


# def parse_cpp_file(source, write_stream):
#     lines = cpp_rst.translate_cpp_file(file)
#     for l in lines:

def make_temp_rst_from_md(lines):
    with tempfile.TemporaryDirectory() as tmpdir:
        md_file = os.path.join(tmpdir, 'input.md')
        rst_file = os.path.join(tmpdir, 'output.rst')
        with open(md_file, 'w') as w:
            for line in lines:
                w.write(line)

        subprocess.check_call(f'cat {md_file}', shell=True)
        convert_md_to_rst(md_file, rst_file)
        with open(rst_file) as r:
            return [l.strip() for l in r.readlines()]


def dump_file(input_file, start, end, indent, write_stream) -> None:
    print(f' ^---- dumpfile {input_file}')
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
    else:
        final_lines = [prefix + l.strip() for l in subset]

    write_stream.write('\n'.join(final_lines))


def parse_m_rst(source, dst):
    with open(dst, 'w') as w:
        with open(source, 'r') as f:
            for line in f.readlines():
                matches = DUMPFILE_RE_2.match(line)
                if matches:
                    input_file, start, end, indent = matches.groups(0)
                    if start:
                        start = int(start)
                    if end == '~':
                        end = None
                    elif end:
                        end = int(end)
                    if indent:
                        indent = int(indent)
                    else:
                        indent = 0

                    full_input_file = os.path.join(
                        os.path.dirname(source), input_file)
                    dump_file(full_input_file, start, end, indent, w)
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
            # elif file.endswith('.cpp') or file.endswith('.hpp'):
            #     parse_cpp_file(file, to_path)


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

from enum import Enum
from typing import List, Optional


class Line(object):
    """Represents a line of text."""

    def __init__(self, line: str):
        self.line = line

    def class_keyword(self) -> bool:
        return 'class' in self.line

    def starts_with_doc_line(self) -> bool:
        return self.line.startswith('// --')

    def starts_with(self, text: str) -> bool:
        """True if the line starts with the given text."""
        return self.line.startswith(text)

    def strip_comment_slashes(self) -> str:
        """Removes "// " from the text."""
        if self.line.startswith('//'):
            return self.line[2:]
        else:
            return self.line

    def text(self) -> str:
        """Just returns the text."""
        return self.line

    def end_class_marker(self) -> bool:
        """Special }; at start of line."""
        return self.line.strip().startswith('};')

    def end_marker(self) -> bool:
        """Has "// end-doc" """
        return (self.line.startswith('// -/') or
                self.line.startswith('// end-doc'))

    def doc_line_with_tail(self) -> bool:
        return self.line.rstrip().endswith('-/')


class TokenType(Enum):
    NONE = 0
    BIG_HEADER = 1
    SECTION_HEADER = 2
    SECTION_TEXT = 3
    CODE = 4
    EOF = 5


class Token(object):

    def __init__(self, type: TokenType=TokenType.NONE,
                       text: List[str]=None,
                       line_number: int=0):
        self.type = type
        self.text = text
        self.line_number = line_number


class Mode(Enum):
    OUTER_SPACE = 0
    BIG_HEADER = 1
    SECTION_HEADER = 2
    SECTION_TEXT = 3
    UNKNOWN_CODE = 4
    CLASS_CODE = 5
    NONCLASS_CODE = 6


class Tokenizer(object):

    def __init__(self):
        self._line_number = 0
        self._m  = Mode.OUTER_SPACE
        self._indent_level = 0
        self._text = []
        self._section_text_max_dedent = 0

    def read(self, l: Line) -> Optional[Token]:
        self._line_number += 1
        method_name = '_case_{}'.format(self._m.name.lower())
        method = getattr(self, method_name)
        if method is None:
            raise ValueError('Unhandled Mode! {}'.format(method_name))

        return method(l)

    def _case_outer_space(self, l: Line) -> Optional[Token]:
        if l.starts_with_doc_line():
            if self._line_number <= 2:
                self._m = Mode.BIG_HEADER
            else:
                self._m = Mode.SECTION_HEADER
            self._text = []
        return None

    def _case_big_header(self, l: Line) -> Optional[Token]:
        return self._case_section_header(l, Mode.BIG_HEADER)

    def _case_section_header(self, l: Line,
                             m: Mode=Mode.SECTION_HEADER) -> Optional[Token]:
            if l.starts_with_doc_line():
                if m == Mode.SECTION_HEADER:
                    tt = TokenType.SECTION_HEADER
                else:
                    tt = TokenType.BIG_HEADER
                t = Token(tt, self._text, self._line_number)
                self._m = Mode.SECTION_TEXT
                self._section_text_max_dedent = 256
                self._text = []
                return t
            else:
                self._text.append(l.strip_comment_slashes().strip());
                return None

    def _case_section_text(self, l: Line) -> Optional[Token]:
        if l.starts_with_doc_line():
            dedent_text = []
            for text in self._text:
                if len(text) >= self._section_text_max_dedent:
                    dedent_text.append(text[self._section_text_max_dedent:])
                else:
                    dedent_text.append(text)

            t = Token(TokenType.SECTION_TEXT, dedent_text, self._line_number)
            self._text = []
            if l.doc_line_with_tail():
                self._m = Mode.OUTER_SPACE
            else:
                self._m = Mode.UNKNOWN_CODE
            return t
        else:
            s_text = l.strip_comment_slashes().rstrip()
            content = s_text.lstrip()
            if len(content) > 0:
                spaces = len(s_text) - len(content)
                self._section_text_max_dedent = \
                    min(spaces, self._section_text_max_dedent)

            self._text.append(s_text)
            return None

    def _case_unknown_code(self, l: Line) -> Optional[Token]:
        if l.end_marker():
            self._m = Mode.OUTER_SPACE
            return None
        elif l.class_keyword():
            self._m = Mode.CLASS_CODE
        else:
            self._m = Mode.NONCLASS_CODE
        self._text.append('    {}'.format(l.text()))
        return None

    def _case_class_code(self, l: Line) -> Optional[Token]:
        if l.end_class_marker():
            self._text.append('    {}'.format(l.text()))
            self._m = Mode.OUTER_SPACE
            t = Token(TokenType.CODE, self._text, self._line_number)
            self._text = []
            return t
        return self._case_nonclass_code(l)

    def _case_nonclass_code(self, l: Line) -> Optional[Token]:
        if l.end_marker():
            self._m = Mode.OUTER_SPACE
            t = Token(TokenType.CODE, self._text, self._line_number)
            self._text = []
            return t
        elif l.starts_with_doc_line():
            self._m = Mode.SECTION_HEADER
            t = Token(TokenType.CODE, self._text, self._line_number)
            self._text = []
            return t
        else:
            self._text.append('    {}'.format(l.text()))
            return None


def read_source(lines: List[str]) -> List[Token]:
    tokens = []  # type: List[Token]

    tokenizer = Tokenizer()

    for line in lines:
        line = line.rstrip()
        result = tokenizer.read(Line(line))
        if result:
            tokens.append(result)

    tokens.append(Token(TokenType.EOF, []))
    return tokens


HEADERS = [
    '=',
    '-',
    '~',
    '^',
    '\'',
    '`',
]

def translate_cpp_file(lines: List[str], section: Optional[str]) -> List[str]:
    if not section:
        header_depth = 1
    else:
        header_depth = HEADERS.index(section) + 1

    tokens = read_source(lines)
    output = []
    for token in tokens:
        if token.type in [TokenType.BIG_HEADER, TokenType.SECTION_HEADER]:
            if len(token.text) != 1:
                # Looked like a section header, but it wasn't!
                raise ValueError('Section header starting at line {} was '
                                 'malformed.'.format(token.line_number))
            header_char = HEADERS[header_depth % len(HEADERS)]
            output.append(token.text[0])
            output.append(header_char * len(token.text[0]))
            if token.type == TokenType.BIG_HEADER:
                header_depth += 1
        elif token.type == TokenType.SECTION_TEXT:
            output += token.text
            output.append('\n')
        elif token.type == TokenType.CODE:
            output.append('.. code-block:: c++\n')

            for cl in token.text:
                cl_lines = cl.split('\n')
                for cl_line in cl_lines:
                    output.append('    ' + cl_line)

        elif token.type == TokenType.EOF:
            header_depth -= 1
        else:
            raise AssertionError('Unexpected case: {}'.format(token.type))

    return output

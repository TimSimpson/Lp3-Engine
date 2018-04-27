# Docs

Build with

    pipenv install
    pipenv run python make.py build

## Bespoke doc builder instructions

All docs in `source` gets copied into `output/gen` which is then fed to Sphinx.

Files ending with `mrst` stand for Ms. ReStructured Text. Like Ms. Pacman, it's better than the original. There's a custom parser for them which writes them to the generated doc directory.

In addition to including .rst files, these files can include MarkDown and C++ files and translate them to rst in the process. Here's how it works.

First off, the ~dumpfile directive looks like this:

    ~dumpfile "file" <start> <end> <indent> <section>

~dumpfile takes several directives by position or it can use keyword args, like so:

    ~dumpfile "file" 0 10 4 ~

is equivalent to

    ~dumpfile "file" end=10 start=0 section=~ indent=4

`start` and `indent` default to 0 if not set. `end` defaults to the end of the file.

`end` also excepts the tilda character `~` to mean end of file as well. So this means "include all text from line 12 until the end of the file and indent everything by 4 characters:

    ~dumpfile "file" 12 ~ 4

This simply dumps the entire file:

    ~dumpfile "file"

Finally when parsing C++ files it's sometimes necessary to tell the C++ to rst generator what section header the incoming dumped rst should be nested under. The expected order of the section headers can be found in the `HEADERS` var defined in cpp_rst.py.

So let's say you want to document a header file under a section header. You'd do this:

    namespace blah
    ~~~~~~~~~~~~~~

    ~dumpfile "blah/util.hpp" section=~

This would tell the C++ rst translator to start the next section after `~`, meaning the first section header would be generated as `^`.

## Notes on Markdown Translation

Markdown translation is provided courtesy of Pandoc. A subset of the given Markdown file is generated in a temporary directory (so that `start` and `end` will work) and Pandoc then produces a file which is read and included where `~dumpfile` is seen.

## Notes on C++ to ReStructured Text Translation

The parser reads a C++ code and ignores everything until it sees special comment syntax it likes, which looks like this:

    // --------------------------------------------
    // Section Header
    // --------------------------------------------
    // This describes something important.
    // -------------------------------------------/

This gets translated to the following rst:

    Section Header
    --------------
    This describes something important.

Note the last C++ comment is a line full of dashes ending with `/`: that's important. It tells the translator to stop until it sees the next comment.

Alternatively, it's possible to make the translator scoop up actual C++ code as a snippet following a special comment if the comment ends with `// -------` (without the ending `/`).

The C++ code below the comment will be taken until either:

* the end of the file
* the next special comment section
* the line `// end-doc` appears

Here's an example of a class being included in rst:

    // --------------------------------------------
    // class RenderPlatform
    // --------------------------------------------
    //      A platform for renderers.
    //      Note how this text will get de-dented.
    // --------------------------------------------

    class RenderPlatform {
    public:
        virtual ~RenderPlatform();
        virtual const char * get_name() const;
        virtual const int priority() const;
    };
    // end-doc

the above turns into:

    class RenderPlatform
    --------------------
    A platform for renderers.
    Note how this text will get de-dented.

    .. code-block:: c++

       class RenderPlatform {
        public:
            virtual ~RenderPlatform();
            virtual const char * get_name() const;
            virtual const int priority() const;
        };

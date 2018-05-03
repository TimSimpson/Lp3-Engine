# Docs

The docs for this project are in [MsRst](https://github.com/TimSimpson/mrst) format. That means it's a [Sphinx](http://www.sphinx-doc.org/en/master/) project augmented with some extra features that lets much of the documentation get pulled from existing header and README files.

## How to Build

Build with

    pipenv install
    pipenv run mrst --source source --output output

The docs will end up in `output/build/html/index.html`.

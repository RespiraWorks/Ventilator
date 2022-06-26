# Wiring assemblies

[WireViz]: https://github.com/formatc1702/WireViz
[schema]: file:///home/martin/dev/ventilator/docs/_build/html/purchasing/numbering-schema.html

Here live the source files for wiring assemblies. The documents are generated using
[WireViz][WireViz] and a mix of python and bash scripts.

## Creating a new assembly:

1. Add any new wiring-harness-specific parts to [docs/purchasing/parts.json](../purchasing/parts.json); don't forget to use a unique `"pn"` field so the part can be referenced from other pages. Please use our [parts numbering schema][schema] and follow examples.

3. Create a `*.yml` file as a [WireViz][WireViz] input, using the `"pn"` field to reference the same parts defined above

3. Create an `*.rst` file with the instructions, which references the `*.bom.tsv` and `*.png` files created by WireViz (same name as your `yml`, just different extensions).

4. Images for pinout diagrams or photos of assembly process should go into [docs/wiring/images](images), to be referenced in the definition files above.

5. Should you want to identify and link to parts inline in the text of the assembly instructions, you should use this syntax:
```
:ref:`[2] <ew-pre2>`
```

Where
* `[2]` can be any label you want, preferably matching the WireViz generated BOM item number
* `ew-pre2` should be replaced by whatever `"pn"` identifier

## Scripts

[decorate_table.py](decorate_table.py) - takes a `*.bom.tsv` file and modifies it so that each entry in the `P/N` column is wrapped in RST link-to-reference syntax. Thus, when a the table is embedded in an RST file, the part numbers are rendered as links to the purchasing/parts page

[make_wiring.sh](make_wiring.sh) - runs wireviz to generate BOM and diagram, then runs `decorate_table.py` to add links to BOM

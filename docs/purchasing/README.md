# Purchasing documentation

Here live the source files for purchasing documentation.

All part and supplier definitions are tracked in [parts.json](parts.json).

The syntax/semantics for a part definition are as follows:

```json
  {
    "pn": "unique-identifier", // follow RespiraWorks parts numbering schema
    "notes": "Verbal description of part",
    "manufacturer": "Sprocket Corp.",
    "mpn": "sprocket-4269", // manufacturer part number
    "avg_price": 4.20, // average or nominal price in USD, manually calculated from below
    "price_unit": "cm", // assumed to be per-item if left blank
    "sources": [
      {
        "supplier": "Sprocket Depot",
        "spn": "sprocket-depot-3235", // supplier part number
        "price": 4.20, // price in USD
        "price_units": 100, // useful if price is per length or area, or in bulk
        "url": "https://www.sprocket-depot.com/product/sd-3235"
      }
    ]
  }
```

## Noteworthy

[schema]: file:///home/martin/dev/ventilator/docs/_build/html/purchasing/numbering-schema.html

* Please use unique `"pn"` codes, so they can be easily linked from other documents.
* Please use our parts numbering schema, documented in the generated docs from this section [here][schema]
* Calculate the `"avg_price"` manually based on whichever supplier(s) you think are representative
* When calculating `"avg_price"` don't forget to divide by `"price_units"` if appropriate
* All prices are in USD (for now)

## Scripts

[make_parts.py](make_parts.py) - reads [parts.json](parts.json) and from it generates `parts.rst`,with tables and link nodes which can then be rendered by sphinx

[make_parts.sh](make_parts.sh) - wrapper script that ensures correct operating system and runs the python script from within this directory

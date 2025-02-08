Parts numbering schema
----------------------

The RespiraWorks parts numbering schema is designed with interdisciplinary maintenance of documentation in mind. Because of this, part numbers must be:

* semantically intuitive - reading the number should give you a rough idea of what this part is
* unambiguous - should be easy to disambiguate similar parts

Currently proposed (incomplete) schema is as follows::

    CS-descriptor[-nn]

**Mandatory sections:**

* CS - Category + Subcategory
    * e - electrical
        * ee - electronics, computing
        * ep - power - batteries, external adapters, etc..
        * ew - wiring
        * ec - connectors
    * p - pneumatics
        * pa - active valves - solenoids, etc..
        * pp - passive valves - one-way, overpressure
        * pf - fittings and tubing adapters
        * pt - tubing
    * m - mechanical
        * mm - metal, sheet metal, etc..
        * mp - plastics, acrylic sheets, etc..
        * mh - hardware
    * c - cosmetic
        * cs - stickers, safety labels
    * t - tools
        * te - tools, electrical
        * tm - tools, mechanical
        * tp - tools, pneumatic
        * tc - tools, cosmetic
* descriptor - short alphanumeric descriptor, preferably around 4-5 characters long

**Optional section:**

The *nn* is some numeric suffix that may be added to disambiguate a part that would otherwise have the same code to the left of it.

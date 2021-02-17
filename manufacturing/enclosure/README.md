# Enclosure

| Rendering | Exploded View |
:------------------:|:-----------------:|
| ![](images/enclosure_rendering.jpg)  | ![](images/enclosure_exploded.jpg)  |

## Design rationale

The current version of the ventilator assembly is meant to demonstrate its packaging in "production" housing. Based
on lessons learned from previous iterations, the ventilator will use a detachable internal assembly, to which all
pneumatic components can be mounted and connected. This allows for 360 degree accessibility to components during initial
assembly. When complete, this internal assembly is bolted into the enclosure, and the enclosure is screwed shut. The
top-rear positioning of the removable enclosure lid provides access to components that will need routine maintenance.

We chose a sheet metal construction because:
* it is cheap and quick - sheet metal is cheap to fabricate. Unlike plastic parts, it doesn't require any expensive
  molds. This is great for prototyping stages and should also be an accessible manufacture process in many countries.
* strength and durability - stainless steel and other highly durable materials can be used. The frame and enclosure
  should be rigid and withstand mechanical stresses. The simple two-piece enclosure should prove to be quite rugged.
* staged assembly - we wanted the ability to assemble all internals outside the enclosure. The design is built around
  a strong bracket upon which all pneumatics and electrical equipment is installed. This makes assembly and maintenance
  easier. Also useful when prototyping and making design modifications.

## Custom parts

The ventilator enclosure is made up of three parts, each made out of a folded, 1.52mm thick, stainless steel sheet.

| Part  | Source | Export |
|:------|:-------|---|
| Main enclosure | [.ipt](enclosure.ipt)         | **TODO** |
| Equipment tray | [.ipt](equipment_bracket.ipt) | **TODO** |
| Closure panel  | [.ipt](closure_panel.ipt)     | **TODO** |

### Fabrication Drawings

Enclosure fabrication drawings.

|             |     |  |
:------------------:|:-----------------:|:---:|
| ![](images/fab_drawing_1.png) | ![](images/fab_drawing_2.png) | ![](images/fab_drawing_3.png) |
| ![](images/fab_drawing_4.png) | ![](images/fab_drawing_5.png) | ![](images/fab_drawing_6.png) |
| ![](images/fab_drawing_7.png) | ![](images/fab_drawing_8.png) | ![](images/fab_drawing_9.png) |


## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                   | Price (USD) | Sources[*][ppg]| Notes |
| ----- |---------:| ------------- | ------------------------ | -----------:|:----------:|:------|
|**A1** | 1        | RespiraWorks  | ventilator_enclosure     |             | [Rw][a1rw]   | Ventilator enclosure, bent sheet metal |
|**A2** | 1        | RespiraWorks  | equipment_bracket        |             | [Rw][a2rw]   | Ventilator equipment bracket, bent sheet metal |
|**A3** | 1        | RespiraWorks  | ventilator_closure       |             | [Rw][a3rw]   | Ventilator closure (back panel), bent sheet metal |
|**A4** | 10       | McMaster-Carr | 97525A415                | 8.87 / 100  | [C][a4mcmc]  | 1/8" blind rivets |
|**A5** | 4        | McMaster-Carr | 96439A710                | 6.04 / 5    | [C][a5mcmc]  | M6 self-clinching / press-fit nut |
|**A6** | 12       | McMaster-Carr | 96439A650                | 3.89 / 10   | [C][a6mcmc]  | M4 self-clinching / press-fit nut |
|**A7** | 4        | McMaster-Carr | 9541K82                  | 8.34 / 10   | [C][a7mcmc]  | M6 threaded-stud bumper, used as feet |
|**A8** | 12       | McMaster-Carr | 92095A188                | 10.04 / 100 | [C][a8mcmc]  | M4 screws, 6mm: main bracket and closure fastening |

>**#TODO: How much sheet metal?**

[a1rw]:   #custom-parts
[a2rw]:   #custom-parts
[a3rw]:   #custom-parts
[a4mcmc]: https://www.mcmaster.com/97525A415/
[a5mcmc]: https://www.mcmaster.com/96439A710/
[a6mcmc]: https://www.mcmaster.com/96439A650/
[a7mcmc]: https://www.mcmaster.com/9541K82/
[a8mcmc]: https://www.mcmaster.com/92095A188/


## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../methods).**

| Assembly | Source |
|:---------|:-------|
| Equipment bracket assembly | [.iam](equipment_bracket.iam)  |
| Enclosure assembly         | [.iam](enclosure_assembly.iam) |

**Hazard:** sheet metal edges may be sharp and you could hurt yourself while working with these components.
It is highly advise that you deburr, file and/or send all edges before you do anything else.

The main panel is folded from a single sheet and fastened together using stainless steel blind rivets. There
are rubber bumper feet on the bottom of the enclosure that are threaded into M6 press-fit nuts. The rubber bumpers
can be removed to allow the enclosure to be mounted to a medical cart.

The rear closure panel is folded from a single sheet.

The internal equipment bracket is folded from a single sheet. The bracket is designed to be simple and lightweight.
Acrylic sheets are either fastened to or hung from the bracket to allow for design adjustments while utilizing the same
bracket.

**#TODO : Add photos of only the assembled enclosure**

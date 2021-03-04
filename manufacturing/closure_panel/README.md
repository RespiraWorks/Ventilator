# Closure panel

| Rendering | Assembled |
|:-------------------------:|:-------------------------:|
| ![](images/rendering.jpg) | ![](images/assembled.jpg) |


## Design rationale

> **TODO:** Content..

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                     | Price (USD) | Sources[*][ppg]| Notes |
| ----- |---------:| ------------- | -------------------------- | -----------:|:--------------:|:------|
|**A1** | 1        | RespiraWorks  | [Closure panel][a1rw]      | 109.25      | Rw             | Ventilator closure (back panel), bent sheet metal |
|**A2** | 1        | RespiraWorks  | [Main power harness][a2rw] | 8.61        | Rw             | Main power wire harness, with 4pin DIN female plug |
|**A3** | 2        | McMaster-Carr | 92095A461                  | 0.38        | [C][a3mcmc]    | M2.5 screw, 12mm |
|**A4** | 2        | McMaster-Carr | 93625A102                  | 0.11        | [C][a4mcmc]    | M2.5 locknut |
|**A5** | 1        | McMaster-Carr | 92095A189                  | 0.09        | [C][a5mcmc]    | M4 screw, 8mm |
|**A6** | 1        | McMaster-Carr | 93475A230                  | 0.02        | [C][a6mcmc]    | M4 washer, 9mm OD |
|**A7** | 1        | McMaster-Carr | 93625A150                  | 0.06        | [C][a7mcmc]    | M4 locknut |

**Total assembly price:** USD 119.01

>**#TODO: How much sheet metal?**

[a1rw]:    #custom-parts
[a2rw]:    ../wiring/README.md#main-power-input
[a3mcmc]:  https://www.mcmaster.com/92095A461/
[a4mcmc]:  https://www.mcmaster.com/93625A102/
[a5mcmc]: https://www.mcmaster.com/92095A189/
[a6mcmc]: https://www.mcmaster.com/93475A230/
[a7mcmc]: https://www.mcmaster.com/93625A150/

## Custom parts

### Closure Panel Fabrication
The closure panel is made out of a folded, 1.52mm thick, stainless steel sheet.

| | |
|:------|:-------|
|[![](images/fab_drawing_1.jpg)](closure_assembly.pdf)|[![](images/fab_drawing_2.jpg)](closure_assembly.pdf)|


| Download type | Link   |
|:--------------|:-------|
| Fabrication drawing    | [.idw](closure_assembly.idw) [.pdf](closure_assembly.pdf) |
| Source: part CAD model | [.ipt](closure_panel.ipt) |

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../methods).**

| Download type | Link   |
|:--------------|:-------|
| Engineering drawing        | [.idw](closure_assembly.idw) [.pdf](closure_assembly.pdf) |
| Source: assembly CAD model | [.iam](closure_assembly.iam) |


The main power wire harness `[A2]` terminates with a female 4-pin DIN connector. This connector must be mounted to the
closure panel. Use M2.5 screws `[A3]` and locknuts `[A4]`. Orient the DIN socket with the locking pin facing up.

The main power harness also has a grounding loop that should be used to ground the entire ventilator enclosure.
Us the M4 hardware to secure it at the nearby hole.

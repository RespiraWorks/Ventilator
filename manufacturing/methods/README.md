# Manufacturing methods

Since we have to manufacture some custom components for the prototypes, it is worth trying to do so in as
reproducible a way as possible, so that we are testing (even in the prototyping phase) with components of similar
properties. Secondarily, this should help everyone get started faster and avoid having to reinvent the wheel.

**DISLAIMER:** None of the following constitutes an endorsement of the listed products. We have done no systematic
comparative study of alternate products and have no evidence to indicate that the following are the most appropriate
for any particular purpose.

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE buying any parts.**

[ppg]: ../purchasing_guidelines.md

Below you will find advice on the following:
* [Sheet metal](#sheet-metal)
* [3d Printing](#3d-printing)
* [Gaskets](#gaskets)
* [Heat set inserts](#heat-set-inserts)
* [Acrylic panels](#acrylic-panels)
* [Flexible tubing](#flexible-tubing)
* [Tapered thread connectors](#tapered-threads)
* [Screwdrivers and hand taps](#screwdrivers)

## Sheet metal

**Hazard:** sheet metal edges may be sharp and you could hurt yourself while working with these components.
It is highly advise that you deburr, file and/or send all edges before you do any work with sheet metal parts.

## 3d Printing

We have been printing quite a few custom pneumatics and mechanical components. The following is a summary of what has
worked.

### Printers

We have been printing with SLA printers, such as:

* [Anycubic Photon](https://www.anycubic.com/collections/anycubic-photon-3d-printers/products/anycubic-photon-3d-printer)
* [Elegoo Mars](https://www.elegoo.com/collections/mars-series)

### Resin

We have been using either:
* [SirayaTech Fast](https://www.amazon.com/ABS-Like-Curing-Non-Brittle-Printing-Resin/dp/B07WFJ53LP) ABS-like resin
  (comes in various colors)
* The above mixed with ~30% [SirayaTech Tenacious](https://www.amazon.com/Tenacious-Flexible-Resistant-Siraya-Tech/dp/B07PLJ9XW9)
  to mitigate brittleness
  * If you have problems with parts cracking from impacts, try this mix.  It strong but slightly flexible, which makes
    it resistant to cracking.
  * The densities are similar so it doesn't matter too much if you mix it by mass or volume.  If you have a scale, you
    can mix it by mass right in the bottle, and this may reduce mess.
* [SirayaTech Clear V2](https://www.amazon.com/Blu-Strong-Precise-Resolution-Printing/dp/B083DLSR1W) Nearly transparent,
  but harder to print with. Good for things like filter housings that need to look good but don't require precision
  and strength.

Guides for printing and curing are provided on the [manufacturer's website](https://siraya.tech/pages/support).

### Slicing

Software included with lower-end SLA printers may not be the most up-to-date. We have found it useful to scaffold and
slice the models using [ChituBox](https://www.chitubox.com/). It is available for free and runs on most operating
systems, including Linux.

The following are configuration profiles we have used in Chitubox for some of the above-mentioned printer-resin combinations:
* [Anycubic Photon & Fast resin](AnyCubic_Photon_Siraya_fast_profile.cfg)
* [Anycubic Photon & 70:30 Fast:Tenacious blend](AnyCubic_Photon_Fast7_+_tenacious3_profile.cfg)
* [Anycubic Photon & Blu Clear 2 resin](AnyCubic_Photon_Siraya_Blu_Clear_2_profile.cfg)
* Elegoo Mars & Fast Resin: the default settings in Chitubox for Printer: "Elegoo Mars" and Resin: "Elegoo Standard Resin Grey for 0.05mm" work perfectly.
* [Elegoo Mars & 70:30 Fast:Tenacious blend](Elegoo_Mars_70fast_30tenacious_profile.cfg)
  * This is exactly the same as the Elegoo Standard Grey resin profile, except with lifting speed reduced to 60mm/min to prevent layer separation issues.

There is a page with additional advice on using Chitubox with the Anycubic Photon printer
[here](https://all3dp.com/2/chitubox-anycubic-photon-settings-profile/).
The Elegoo Mars works natively with chitubox.

### Kind request

When you print a new part, you may customize the scaffolding which provides some improvement over the defaults. If you
have used Chitubox to do this, please save a `*.chitubox` of your intermediate object before slicing, and upload this
file to share with others. This will save everyone time and, again, improve consistency across the team. You may
also provide whatever final printer-specific files, such as `*.photon`.

Please:
* reference version of CAD model that was rendered for printing
* add any new binary file types to be tracked by [git LFS](https://github.com/RespiraWorks/Ventilator/wiki#adding-new-binary-files).

### Post-printing finishing

For finer pieces that require tighter fits and smoother edges, you might find it useful to have
a set of [needle files](https://www.grainger.com/product/23N602).

## Gaskets

For cutting gaskets, you can use a
[craft knife](https://www.amazon.com/Fiskars-167110-1001-Heavy-Craft-Knife/dp/B001A42CTW).

## Heat set inserts

A number of components may have to be mounted using heat-set inserts in lieu of nuts. These may go into
either acrylic of 3d-printed parts. The following describes how to install them.

First off, you will need a soldering iron. It need not be an expensive one you would use for assembling electronics.
In fact, you should prefer a bulkier one, such one used for wood-burning crafts, such as
[this](https://www.amazon.com/Walnut-Hollow-Versa-Temp-Temperature-Woodburning/dp/B005P1TRAS).

1. Select a soldering iron tip large enough to provide uniform pressure on the center of the heat-set insert, but small
   enough to not touch the surrounding material.
2. Heat up your soldering iron to an operating temperature of around 343-399&deg;C (650-750&deg;F).
3. Place the heat-set insert into the mounting hole with the smaller diameter facing down into the hole away from you.
4. Touch the tip of the soldering iron to the top of the heat-set insert and wait for a few seconds.
5. The weight of the soldering iron should gently push the heat-set insert into the now melted plastic.
6. Press down slowly until the half the heat-set insert inserted into the plastic. At this point only the first set of
   knurls should be inside the plastic.
7. Allow the plastic to cool. Then push inserts in all the way till it is flush with the face of the plastic.
8. Remove heat and allow to cool and harden.

Works best if you push each of the inserts you are working on halfway, let them cool and come back to finish
them in turn.

| Start (step 4)             | Half-way (step 6)           |
|:--------------------------:|:---------------------------:|
|![](images/insert1.jpg) | ![](images/insert2.jpg) |


## Acrylic panels

We have a number of acrylic panels in the enclosed prototype. Clear acrylic was partly chosen because it
can be easily modified while quickly iterating our design, certainly more easily than any metal components.

Ideally, acrylic panels should be cut by laser. They can be cut precisely to specification.

>*TODO:* add section on laser cutters, settings, etc..

### Acrylic and hand-tools

You may not have access to a laser cutter. Or you may need to make a quick modification that does not warrant
creating a whole new CAD model and producing a part from scratch.

Common woodworking tools are perfectly adequate for modifying acrylic panels. Some of the following might be useful:

| Item   | Manufacturer   | Part #             | Price (USD)  | Sources[*][ppg] | Notes |
| ------ | -------------- | -------------------| ------------:|:---------------:|:------|
| **B1** | Black & Decker | BDEJS300C          | 24.49        | [A][b1amzn]     | Jigsaw, for cutting acrylic |
| **B2** | Black & Decker | BDEDMT             | 29.99        | [A][b2amzn]     | Electric hand drill, **drill press preferred if you have one**, for drilling acrylic |
| **B3** | XLCMSY         | EAN: 0742880496381 | 12.99        | [A][b3amzn]     | Cobalt metric drill bits, in particular - 4.5mm |
| **B4** | Porter Cable   | PC1014             | 59.59        | [A][b4amzn]     | Forstner bits, **preferred for drill press** |
| **B5** | KATA TOOLS     | KTL89020           | 19.99        | [A][b5amzn]     | Hole saws, **preferred for hand drill** |

[b1amzn]: https://www.amazon.com/BLACK-DECKER-Jig-Amp-BDEJS300C/dp/B00OJ72L84
[b2amzn]: https://www.amazon.com/BLACK-DECKER-BDEDMT-Matrix-Driver/dp/B0094B9BHE
[b3amzn]: https://www.amazon.com/XLCMSY-Straight-Resistant-Stainless-1-5-6-5mm/dp/B07ZQGXWGL
[b4amzn]: https://www.amazon.com/PORTER-CABLE-PC1014-Forstner-Bit-14-Piece/dp/B004TSZEB4
[b5amzn]: https://www.amazon.com/KATA-19-152mm-Mandrels-Installation-Plywood/dp/B08JCHY2DM

You might also want to use some 200 grit sandpaper to clean up the edges of the pieces you cut and drill.

## Flexible tubing

We have been using various types of rubber, silicon and Tygon tubing. Not all cut easily with the same type of tools.
Sometimes a [PVC pipe cutter](https://www.amazon.com/gp/product/B07Y997XKC) has worked well, other times - household
scissors or a sharp knife will do.

You may have to clamp tubing to barbed fittings, or you may want to remove the clips so that you can remove the tubing.
These [self-adjusting pliers](https://www.amazon.com/gp/product/B008S0FVWG) have proven themselves useful for either
task.

## Tapered threads

Use PTFE tape ([McMaster](https://www.mcmaster.com/6802K11/) or [Amazon](https://www.amazon.com/gp/product/B01L2F428C))
on all tapered thread joints, such as NPT. The tape not only ensures a tighter seal, but also makes it go in easier and
prevents it from seizing up if you do eventually need to remove the part.

Make sure you wrap it in the same direction as the threads will go in. There are videos online on how
to do this correctly.

You may need an [adjustable wrench](https://www.amazon.com/GETUPOWER-10-Inch-Adjustable-Opening-Vanadium/dp/B07RGV2VK1)
as well, though depending on the type of fitting you are attempting to thread pliers might also work.

## Screwdrivers

Most of the screws in the ventilator assembly require a metric hex-shaped driver.

If you already have some sort of socket driver, you could just get some
[metric hex bits](https://www.amazon.com/Wiha-71397-Metric-Insert-6-Piece/dp/B0084B7S70).

If you got nothing, a cheap choice would be a set of
[hex key wrenches](https://www.amazon.com/TEKTON-Wrench-Metric-30-Piece-25253/dp/B00I5TH074).

For something more comfortable and versatile, you could also go with a
[precision screw driver set](https://www.amazon.com/gp/product/B07R9VTWG5).

There may be a few components that require tapping or chasing of printed threads. The dedicated parts pages
will hopefully identify the exact type of tap you need. However, you may need a handle to operate these taps.

Something like [this set](https://www.amazon.com/gp/product/B003GKJYKI) should be able to handle the range of diameters
of the taps listed.

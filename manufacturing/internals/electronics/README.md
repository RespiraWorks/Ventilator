# Electronics panel

![](images/rendering.jpg)

This build constitutes the electrical, computing and electronics components.

**Note:** The Main board are custom components and will need to be fabricated by a contract manufacturer before proceeding.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item    | Quantity | Manufacturer  | Part Number                       | Price (USD) | Sources[*][ppg] | Notes                                            |
|---------|---------:|---------------|-----------------------------------|------------:|:---------------:|--------------------------------------------------|
| **A1**  |        1 | RespiraWorks  | [Electronics support frame][a1rw] |   (?)150.00 |       Rw        | Electronics support frame, bent sheet metal      |
| **A2**  |        4 | McMaster-Carr | 96439A650                         |        0.39 |   [C][a2mcmc]   | M4 self-clinching / press-fit nut                |
| **A3**  |        1 | RespiraWorks  | [Mainboard][a3rw]                 |   (?)300.00 |       Rw        | Custom main board                                |
| **A4**  |      (1) | Duracell      | 2016 3V                           |        1.67 |   [Z][a4amzn]   | CR2016 battery                                   |
| **A5**  |        1 | Raspberry Pi  | RPI4-MODBP-2GB                    |       53.23 |   [M][a5mous]   | Raspberry Pi 4, 4GB                              |
| **A6**  |        1 | Pastall       | PT2019120501                      |        1.00 |   [Z][a6amzn]   | Heat sink for RasPi                              |
| **A7**  |        1 | SanDisk       | SDSQXAF-032G-GN6MA                |       10.73 |   [Z][a7amzn]   | UHS-1 Class 10 micro SD, memory for Raspberry Pi |
| **A8**  |        1 | UGREEN        | B0773N757H                        |        6.99 |   [Z][a8amzn]   | USB-A to micro-USB cable, 1.5ft                  |
| **A9**  |        1 | CERRXIAN      | B01N5PHSJE                        |        6.95 |   [Z][a9amzn]   | USB-A to mini-USB cable, 9", right angle         |
| **A10** |        4 | DigiKey       | M2117-2545-AL                     |        1.09 |   [K][a10key]   | M2.5 standoff, 22mm                              |
| **A11** |       18 | McMaster-Carr | 93475A196                         |        0.02 |  [C][a11mcmc]   | M2.5 washer, 6mm OD                              |
| **A12** |       18 | McMaster-Carr | 92095A458                         |        0.05 |  [C][a12mcmc]   | M2.5 screw, 6mm                                  |
| **A13** |        5 | DigiKey       | 970100154                         |        0.57 |   [K][a13key]   | M2.5 standoff, 10mm                              |

**Total assembly price:** USD ~540.00

[a1rw]:   #display-panel-support-frame
[a2mcmc]: https://www.mcmaster.com/96439A650/
[a3rw]:    #mainboard-pcb
[a4amzn]:  https://www.amazon.com/Duracell-Lithium-Battery-lasting-battery/dp/B00006JPGV
[a5mous]:  https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-4GB-BULK/?qs=vtpzqDgAobkxvB%252BH7oxSXw%3D%3D
[a6amzn]:  https://www.amazon.com/Raspberry-Heatsink-Conductive-Adhesive-Aluminum/dp/B082RKKQ2D
[a7amzn]:  https://www.amazon.com/dp/B06XWMQ81P
[a8amzn]: https://www.amazon.com/UGREEN-Braided-Charger-Charging-Controller/dp/B01MTXZ3U8
[a9amzn]: https://www.amazon.com/CERRXIAN-9Inch-Cable-Charge-2-Pack/dp/B01N5PHSJE
[a10key]:  https://www.digikey.com/en/products/detail/raf-electronic-hardware/M2117-2545-AL/9836050
[a11mcmc]: https://www.mcmaster.com/93475A196/
[a12mcmc]: https://www.mcmaster.com/92095A458/
[a13key]:  https://www.digikey.com/short/75n45t8w

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../methods).**

### Display panel support frame

Recently exported files for sheet metal components can be found on our Google Drive at
[Manufacturing/Output files v0.4/Sheet metal](https://tinyurl.com/2p89pjja).

Make sure the PEM nuts `[A2]` are installed properly into the support frame `[A1]`.

### Mainboard (PCB)

* The mainboard provides the electrical control of the ventilator.  It is a custom-designed part that is most commonly fabricated by a PCB contract manufacturer.  It uses standard PCB manufacturing processes that can be handled by a wide range of vendors.
* Design and manufacturing files for the main board can be found on the [PCB page](../../../pcb).
* The custom circuit board (PCB) may require some modification to work with the current prototype. Please read any errata on the page linked above and follow the instructions to make the necessary modifications, then proceed.

### Raspberry Pi

The Raspberry Pi serves as the user interface computer for the ventilator. It should be installed onto the PCB **before** attaching the PCB to the support frame.

* install a heat sinks `[A6]` onto the Raspberry Pi `[A5]`
* set up the memory card `[A7]` with the appropriate operating system and software as described in the [software deployment instructions](../../../software/utils/rpi_config).
* install memory card into the RaspberryPi
* Install the 22mm spacers `[A10]` onto the PCB `[A3]` at the location where the Raspberry Pi should be mounted.
* Install the Raspberry Pi `[A5]` onto the PCB `[A3]` using bolts and washers `[A11-12]`.

### Mounting the PCB

* Install spacers `[A13]` onto the support frame using bolts and washers `[A11-12]`.
* Fasten the PCB to the frame using more bolts and washers `[A11-12]`.

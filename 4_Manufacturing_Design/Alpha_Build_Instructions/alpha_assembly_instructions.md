# TODO: give this a name

1. Mount the rPi to the back of the rPi display. Connect display cables.
2. Identify [Front Panel - Inner] and determine the inside and outside of the panel. Mount the following components to the panel:  
    1. Outer side: rPi display + rPi, inhale and exhale ports, filter housing, O2 barb
    2. Inner side: Blower, Arduino, Pressure sensor board, blower driver
3. Complete wiring connections between pressure sensor board, arduino, rPi, and blower driver
4. Bolt the power port to [Front Panel - Outer], with the jack facing out and set aside.
5. Slot [Gusset] into [Front Panel - Inner] - hold it in final position indicated in CAD model
6. Slot [Shelf Panel] into place, by doing the following:  
    1. slot [Shelf Panel] over the bottom tabs of [Gusset] until tabs have fully passed through
    2. shift [Shelf Panel] forward towards [Front Panel - Inner] until the tabs on [Shelf Panel] have fully slotted through the slots on [Front Panel - Inner]
    3. shift [Shelf Panel] left - relative to [Front Panel - Inner] and [Gusset] - to lock tabs into place in both [Gusset] and [Front Panel - Inner]
7. Slot [Front Panel - Outer] onto the front of [Front Panel - Inner], locking the position of all tabs.
8. Add bolts, washers, and lock nuts through 4x 1/4-20 mounting holes in the front of [Front Panel - Outer] - _note: for final assembly, this bolt stack should include the enclosure lid. For prototyping, the lid may be omitted, however, adding the 1/4-20 bolts is important for the purpose of keeping all tabbed connections tight._
9. Create a subassembly of the following parts, including short hose sections and hose clamps where needed (see model):
    1. brass elbow
    2. inhale venturi
    3. brass hose barb
    4. plastic tee
    5. overpressure relief valve (or NPT plug, if valve has not arrived)
    6. brass hose barb
10. Make a tubing connection between the previous assembly and the back side of inhale port. Secure subassembly to plastic structure using zip-tie loops through provided slots.
11. Make a tubing connection between the exhale venturi and the back side of the exhale port. Secure venturi to plastic structure using zip-ties.
12. Create a subassembly of the following parts, including short hose sections and hose clamps where needed (see model):
    1. brass elbow
    2. check valve
    3. plastic nipple
    4. plastic tee
    5. small brass hose barb
    6. brass elbow
13. Make a tubing connection between the previous assembly and the blower outlet. Secure subassembly to plastic structure using zip-ties.
14. Make a tubing connection between the oxygen inlet port and the mixing tee.
15. Create a subassembly of the following parts (see model):
    1. brass elbow
    2. solenoid
    3. brass elbow
16. Secure previous subassembly to plastic structure using zip-ties. Solenoid flow direction goes towards [Gusset].
17. Complete wiring connection between power inlet port and boards, and solenoid if needed.
18. Create tubing connections between venturi pressure ports and differential pressure sensors. Include a Y tubing splitter on the outlet of the inhale venturi to read patient pressure.
19. Make the following tubing connections to complete the flow circuit:
    1. outlet of oxygen mixing tee to inlet of inhale venturi
    2. exhale venturi to inlet of solenoid
    3. outlet of solenoid to exhaust filter port
    4. blower inlet to inlet filter port
20. If enclosure lid was included in assembly, snap lid onto enclosure to complete the ventilator assembly. Add external viral filtration, humidifier chamber, and breathing circuit.
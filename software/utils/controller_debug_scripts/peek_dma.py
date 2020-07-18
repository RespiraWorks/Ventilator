# This is a simple script that can be run from the console debug interface
# to display the current values of the DMA registers.
#
# From the debug console you would enter:
#   run peek_dma.py [which]
#
# which is either 1 or 2 to look at DMA1_BASE or DMA2_BASE.
# If not supplied it shows you DMA1


def main():
    DMA1_BASE = 0x40020000
    DMA2_BASE = 0x40020400
    base = DMA1_BASE

    cl = cmdline.split()
    if len(cl) > 1:
        dma = int(cl[1])
        if dma == 1:
            base = DMA1_BASE
        elif dma == 2:
            base = DMA2_BASE
        else:
            print("Bad DMA number, should be 1 or 2")
            return

    reg = Peek32(base, 43)

    sel = reg[42]
    print("0x%08x Int Status: 0x%08x" % (base, reg[0]))
    print("0x%08x Int clear:  0x%08x" % (base + 4, reg[1]))
    print("0x%08x Chan sel:   0x%08x" % (base + 4 * 42, sel))
    print()

    print("                         config      count       pAddr       mAddr     csel")
    for i in range(7):
        n = 5 * i + 2
        S = "0x%08x Channel %d:  " % (base + 4 * n, i + 1)

        for j in range(4):
            S += "0x%08x  " % reg[n + j]

        S += "%d" % ((sel >> 4 * i) & 0xF)

        print(S)


main()

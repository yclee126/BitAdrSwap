# BitAdrSwap
Rearrange(swap) byte bits and address bits by user input.

For example, typing 7 6 5 4 0 2 1 3 will swap bit 0 and bit 3 of bytes.

It also can process much more complex ones like 3 2 6 4 7 0 5 1.

![example.png](https://github.com/yclee126/BitAdrSwap/blob/master/example.png)

This is same to address bits, but with more bits.

I used this to repair corrupted ROM file caused by wrong data and address pin wiring when dumping.

## How to use
Rename the file's name to "source.hex", then launch the program.

Enter byte bit order first, then address bit order. (to skip typing, just type any alphabet)

The converted file "converted.hex" will be created after it's done.

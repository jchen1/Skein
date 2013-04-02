Skein
=====

Brute-force a solution for xkcd.com/1193.

To compile in general: `gcc -O3 skein_block.c skein.c main.c -o skein`

If you are on Linux x64: `gcc -O3 skein_block_x64.s skein.c main.c -o skein`

To run: `./skein [string]`

[string] is the string you wish to start with. It should only be alpha-numeric characters, though this is not explicitly checked for.

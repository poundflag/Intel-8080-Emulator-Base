# Intel-8080-Emulator-Base
## A C++ implementation of the Intel 8080 processor

Intel-8080-Emulator-Base is an Intel 8080 intended for the use of Altair Basic and an Altair 8800 Simulator

### Milestones

- [x] Passing self-made unit tests
- [x] Passing the [diagnostic test by Microcosm Associates](https://github.com/superzazu/8080/blob/master/cpu_tests/TST8080.ASM)
- [x] Passing the [preliminary test by Frank D. Cringle](https://github.com/superzazu/8080/blob/master/cpu_tests/8080PRE.MAC)
- [x] Passing the [DIAGNOSTICS II V1.2 Test by Supersoft Associates](https://github.com/superzazu/8080/blob/master/cpu_tests/CPUTEST.COM)
- [X] Passing the [Z80 instruction set exerciser by Frank D. Cringle](https://github.com/superzazu/8080/blob/master/cpu_tests/8080EXM.COM)
- [ ] Make a proper 88-Sio implementation
- [X] Running Altair 4k Basic
- [X] Running Altair 8k Basic (I don't know yet, but it's likely)

### TODO 
- Add diagrams to the readme

## Resources
- [Intel 8080 Programmers Manual](https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf)
- [GunshipPenguin's Emulator for the flagregister refinements and ALU idea](https://github.com/GunshipPenguin/lib8080/blob/master/src/i8080.c)
- [Superzazu for the CPU-Test resources](https://github.com/superzazu/8080/tree/master/cpu_tests)
- [Rokkerruslan's Emulator for some minor testing](https://rokkerruslan.github.io/assembler)
- [Oshonsoft's 8085 Simulator (which apparently produces false flagregister results!)](https://www.oshonsoft.com/8085.php)
- [Peter Schorn for the Altair Software (Yielded better results than the software from altairclone.com)](https://schorn.ch/altair_3.php)
- https://pastraiser.com/cpu/i8080/i8080_opcodes.html
- http://dunfield.classiccmp.org/r/8080.txt

#!/bin/bash
outpath="BuildFiles"
mkdir -p $outpath
avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -L./ -Wall -Os $1 -o $outpath/$(basename $1 ".c").elf -llcd
avr-objcopy -O ihex $outpath/$(basename $1 ".c").elf $outpath/$(basename $1 ".c").hex
avrdude -c usbasp -p m644p -U flash:w:$outpath/$(basename $1 ".c").hex
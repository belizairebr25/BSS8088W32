#!/bin/bash
#this program assembles and formats 8088 assembly programs so they can be flashed to rom with an arduino :3 requires nasm, vim, figlet, cowsay
#run it with "bash program_name"

#clears the screen... duh
clear_screen() {
    echo -e "\033[H\033[J"
}

#welcome bullshit
figlet "HIIIIIIII welcome to ma nasm-y boi :3"
echo "ok so like, what .asm program u wan me to make all demure for ya?"
#stores address to assembly program
read program
cowsay "ok ok- i'll do it just chill for a sec"

#assembles and converts assembly program to hex
nasm -f bin $program -o compilerLocalBin.bin
objcopy -I binary -O ihex --change-addresses 0xF000 compilerLocalBin.bin local.hex
cat local.hex

#lazyness
file="local.hex"

#byte count for arduino array size
byte_count=$(wc -c < "$file")
echo "Byte count: $byte_count"

#formats hex
echo "Formatted Hexdump, have fun copy pasting :3"
xxd -p "$file" | tr -d '\n' | sed 's/\(..\)/\1, /g' | sed 's/, $//'


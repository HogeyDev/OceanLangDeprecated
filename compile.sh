nasm -f elf32 main.asm 
ld -m elf_i386 -o a.out main.o
./a.out
printf "\nprogram returned $?\n"
rm main.o a.out

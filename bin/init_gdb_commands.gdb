add-symbol-file ../build/kernelfull.o 0x100000
target remote | qemu-system-x86_64 -hda ./os.bin -gdb stdio -S

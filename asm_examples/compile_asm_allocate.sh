# https://cs.lmu.edu/~ray/notes/nasmtutorial/
#
# ld -arch x86_64 -macos_version_min 10.10.0 -static asm_allocate.o -o asm_allocate -lc
# using ld_classic makes warnings go away from nasm platform
# ld -arch x86_64 -macos_version_min 10.10.0 /Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libSystem -lSystem asm_allocate.o -o asm_allocate

nasm -fmacho64 asm_allocate.asm -o asm_allocate.o
gcc asm_allocate.o -o asm_allocate -ld_classic


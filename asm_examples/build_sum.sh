nasm -fmacho64 -o sum_array.o sum_array.asm
ld -static -macos_version_min 10.10.0 sum_array.o -o sum_array


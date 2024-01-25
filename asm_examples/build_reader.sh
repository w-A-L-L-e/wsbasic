nasm -fmacho64 -o reader.o reader.asm
ld -static -macos_version_min 10.10.0 reader.o -o reader


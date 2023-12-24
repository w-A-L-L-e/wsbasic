nasm -fmacho64 triangle.asm
ld -static -macos_version_min 10.10.0 triangle.o -o triangle
./triangle

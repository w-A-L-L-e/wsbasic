nasm -fmacho64 -o file_writer.o file_writer.asm
ld -static -macos_version_min 10.10.0 file_writer.o -o file_writer


all: readword

readword: readword.o
	ld -e _start -static -macos_version_min 10.10.0  -o $@ $^

readword.o: readword.asm
	nasm -fmacho64 -o $@ $^


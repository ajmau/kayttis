CC = /home/ajjlmau/src/x86_64-elf-cross/bin/x86_64-elf-gcc
CFLAGS = -g -fno-pic               \
    -mno-sse                       \
    -mno-sse2                      \
    -mno-mmx                       \
    -mno-80387                     \
    -mno-red-zone                  \
    -mcmodel=kernel                \
    -ffreestanding                 \
    -fno-stack-protector           \
    -O2                            \
    -fno-omit-frame-pointer

OBJ = ${C_SOURCES:.c=.o} ${NASM_SOURCES:.asm=.o}
C_SOURCES = $(shell find src/ -type f -name '*.c')
NASM_SOURCES = $(shell find src/ -type f -name '*.asm')

kernel.elf: ${OBJ}
	${CC} -Wl,-z,max-page-size=0x1000 -nostdlib -o $@ -T linker.ld ${OBJ}

%.o: %.c
	${CC} ${CFLAGS} -I src -c $< -o $@

%.o: %.asm
	nasm -f elf64 -o $@ $<




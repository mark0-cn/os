BUILD:=./build
SRC:=./src

ENTRYPOINT:=0x10000

CFLAGS:= -m32 #32位程序
CFLAGS+= -fno-builtin #不需要gcc内置函数
CFLAGS+= -nostdinc #不需要标准头文件
CFLAGS+= -fno-pic #不需要位置无关代码
CFLAGS+= -nostdlib #不需要标准库
CFLAGS+= -fno-stack-protector #不需要栈保护


DEBUG:= -g
INCLUDE:=-I${SRC}/include

${BUILD}/boot/%.bin:${SRC}/boot/%.asm
	$(shell mkdir -p $(dir $@))
	nasm -f bin $< -o $@

${BUILD}/kernel/%.o:${SRC}/kernel/%.asm
	$(shell mkdir -p $(dir $@))
	nasm -f elf32 ${DEBUG} $< -o $@

${BUILD}/kernel/%.o:${SRC}/kernel/%.c
	$(shell mkdir -p $(dir $@))
	x86_64-elf-gcc ${CFLAGS} ${DEBUG} ${INCLUDE} -c $< -o $@

${BUILD}/kernel.bin:\
	${BUILD}/kernel/start.o\
	${BUILD}/kernel/main.o
	$(shell mkdir -p $(dir $@))
	x86_64-elf-ld -m elf_i386 -static $^ -o $@ -Ttext ${ENTRYPOINT}

${BUILD}/system.bin:${BUILD}/kernel.bin
	objcopy -O binary $< $@

${BUILD}/system.map:${BUILD}/kernel.bin
	$(shell nm $< | sort > $@)
	
${BUILD}/master.img: \
	${BUILD}/boot/boot.bin \
	${BUILD}/boot/loader.bin \
	${BUILD}/kernel.bin \
	${BUILD}/system.bin \
	${BUILD}/system.map
	yes | bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $@
	dd if=${BUILD}/boot/boot.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=${BUILD}/boot/loader.bin of=$@ bs=512 count=4 seek=2 conv=notrunc
	dd if=${BUILD}/system.bin of=$@ bs=512 count=200 seek=10  conv=notrunc

all:${BUILD}/master.img

test:${BUILD}/system.bin

.PHONY:clean run

clean:
	rm -rf ${BUILD}

run:
	bochs -q
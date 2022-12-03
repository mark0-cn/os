BUILD=./build
SRC=./src

${BUILD}/boot/%.bin:${SRC}/boot/%.asm
	nasm -f bin $< -o $@
	
all: ${BUILD}/boot/boot.bin ${BUILD}/boot/loader.bin
	yes | bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat master.img
	dd if=${BUILD}/boot/boot.bin of=master.img bs=512 count=1 conv=notrunc
	dd if=${BUILD}/boot/loader.bin of=master.img bs=512 count=4 seek=2 conv=notrunc

.PHONY:clean run

clean:
	rm -rf *.bin *.img

run:
	bochs -q
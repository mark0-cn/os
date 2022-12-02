
%.bin:%.asm
	nasm -f bin $< -o $@
	
all: boot.bin 
	yes | bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat master.img
	dd if=boot.bin of=master.img bs=512 count=1 conv=notrunc

.PHONY:clean run

clean:
	rm -rf *.bin *.img

run:
	bochs -q
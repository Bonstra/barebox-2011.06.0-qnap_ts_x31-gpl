all: clean header install

header:
	$(CC) c2kimage_gen.c -o c2kimage_gen

install:
	cp mk_c2kimage.sh /usr/local/bin/
	cp c2kimage_gen /usr/local/bin/

clean:
	rm -f c2kimage_gen *.c2kimg


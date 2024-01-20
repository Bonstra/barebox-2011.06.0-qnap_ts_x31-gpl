HOSTCC ?= gcc

all: clean header install

header:
	$(HOSTCC) c2kimage_gen.c -o c2kimage_gen

install:
	echo cp mk_c2kimage.sh /usr/local/bin/
	echo cp c2kimage_gen /usr/local/bin/

clean:
	rm -f c2kimage_gen *.c2kimg


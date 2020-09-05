#!/bin/sh

while IFS=: read f1 f2 f3 f4 f5
do
field1=$f1
field2=$f2
field3=$f3
field4=$f4
field5=$f5
done < hdr_gen_cfg

make clean;make header


#generate private key, public key and hash on public key
echo "Generating private key with size 1K..."
openssl genrsa -out privatekey-1k.pem 1024
echo "Done.\n\n"

echo "Generating private key with size 2K..."
openssl genrsa -out privatekey-2k.pem 2048
echo "Done.\n\n"

echo "Generating public key with size 1K and hash on public key..."
sh mk_c2kimage.sh nonlegacy gen_key privatekey-1k.pem publickey-1k.rawkey \
$field5
mv publichash publickeyhash-1k
echo "Done.\n\n"

echo "Generating public key with size 2K and hash on public key..."
sh mk_c2kimage.sh nonlegacy gen_key privatekey-2k.pem publickey-2k.rawkey \
$field5
mv publichash publickeyhash-2k
echo "Done.\n\n"


echo "Generating microloader" 
sh mk_c2kimage.sh $field1 $field2 $field3 $field4 $field5
mv $field5.c2kimg ../../uloader.bin
echo "Done.\n\n"

echo "Deleting barebox.bin file" 
rm barebox.bin
echo "Done.\n\n"


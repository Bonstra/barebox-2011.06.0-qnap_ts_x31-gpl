#! /bin/sh
[ -z "$1" -o -z "$2" -o -z "$3" ] && {
  echo "Invalid arguments"
#  exit
}

cd $1
sku=$(echo $2 | sed -e 's\uloader\\g' | sed -e 's\c2kevm\\g' | sed -e 's\c2kasic\\g' | sed -e 's\nand\\g' | sed -e 's\c2kmfcnevm\\g')
bin_file=$3
size=$(ls -l barebox.bin | cut -d' ' -f 5)
head -c 48 barebox.bin > temp.bin

[ "$sku" = "m86260" ] && printf "\x01\x00\x00\x00" >>temp.bin
[ "$sku" = "m86261" ] && printf "\x02\x00\x00\x00" >>temp.bin
[ "$sku" = "m86262" ] && printf "\x03\x00\x00\x00" >>temp.bin
[ "$sku" = "m86291" ] && printf "\x04\x00\x00\x00" >>temp.bin
[ "$sku" = "m86292" ] && printf "\x05\x00\x00\x00" >>temp.bin
[ "$sku" = "m86293" ] && printf "\x06\x00\x00\x00" >>temp.bin
[ "$sku" = "m86294" ] && printf "\x07\x00\x00\x00" >>temp.bin
[ "$sku" = "m86295" ] && printf "\x08\x00\x00\x00" >>temp.bin
[ "$sku" = "m86296" ] && printf "\x09\x00\x00\x00" >>temp.bin
[ "$sku" = "m86297" ] && printf "\x0a\x00\x00\x00" >>temp.bin
[ "$sku" = "m86298" ] && printf "\x0b\x00\x00\x00" >>temp.bin
[ "$sku" = "m86201" ] && printf "\x0c\x00\x00\x00" >>temp.bin
[ "$sku" = "m86202" ] && printf "\x0d\x00\x00\x00" >>temp.bin
[ "$sku" = "m86203" ] && printf "\x0e\x00\x00\x00" >>temp.bin
[ "$sku" = "m86204" ] && printf "\x0f\x00\x00\x00" >>temp.bin
[ "$sku" = "m86206" ] && printf "\x10\x00\x00\x00" >>temp.bin
[ "$sku" = "m86207" ] && printf "\x11\x00\x00\x00" >>temp.bin
[ "$sku" = "m86208" ] && printf "\x12\x00\x00\x00" >>temp.bin
[ "$sku" = "m86261_nas" ] && printf "\x13\x00\x00\x00" >>temp.bin

rem_size=` expr $size - 52 `
#echo "rem_size=$rem_size"
tail -c $rem_size barebox.bin >> temp.bin
cp temp.bin tools/ImageGenarator/barebox.bin
cd tools/ImageGenarator
chmod 755 *.sh
./uldr_gen.sh 
cd $1
mv uloader.bin $bin_file
rm barebox.bin


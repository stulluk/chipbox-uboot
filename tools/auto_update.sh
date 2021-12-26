#!/bin/bash

ramdisk_name="ramdisk.img"
touch misc.bin
if 
test -f u-boot.bin
then
mkimage -n 'U-boot' -A arm -O linux -T firmware -C none -d u-boot.bin u-boot.img
cat u-boot.img >> misc.bin
fi

if
test -f uImage
then
cat uImage >> misc.bin
fi

if
test -f $ramdisk_name
then
mkimage -n 'Ramdisk' -A arm -O linux -T ramdisk -C none -d $ramdisk_name {$ramdisk_name}.img
cat {$ramdisk_name}.img >> misc.bin
fi

if test -s misc.bin
then
mkimage -n 'Misc image' -A arm -O linux -T firmware -C none -d misc.bin misc.temp
else
echo "no valid files available to generate a image!"
echo
exit
fi

if
test -f encrypt.c
then
gcc encrypt.c
echo
echo "Start encrypting!"
./a.out
echo "Encrypting finished!"
echo
else
mv misc.temp misc.img
echo "The image file is generated but not encrypted!"
fi

exit

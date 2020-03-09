#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/dentos.kernel isodir/boot/dentos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "dentos" {
	multiboot /boot/dentos.kernel
}
EOF
grub-mkrescue -o dentos.iso isodir

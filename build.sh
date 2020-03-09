#!/bin/sh
set -e
if [[ "$1" = "clean" ]]; then
	echo "Cleaning projects"
	. ./config.sh

	for PROJECT in $PROJECTS; do
	  (cd $PROJECT && $MAKE clean)
	done

	rm -rf sysroot
	rm -rf isodir
	rm -rf myos.iso
else
	echo "Building projects"
	. ./headers.sh

	for PROJECT in $PROJECTS; do
    	(cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
	done
fi

#!/bin/sh
set -e
./scripts/clean.sh
. ./scripts/iso.sh

if [ "$1" = "debug" ]; then
	echo "Starting gdb server on localhost:1234"
	qemu-system-"$(./scripts/target-triplet-to-arch.sh "$HOST")" -s -S \
		-cdrom dentos.iso \
		-serial stdio \
		-m size=4096
	exit
elif [ "$1" = "host" ]; then
	echo "Starting QEMU with host passthrough"
	qemu-system-"$(./scripts/target-triplet-to-arch.sh "$HOST")" \
		-cpu host \
		-enable-kvm \
		-cdrom dentos.iso \
		-serial stdio \
		-m size=4096
else
	qemu-system-"$(./scripts/target-triplet-to-arch.sh "$HOST")" \
  		-cdrom dentos.iso \
  		-serial stdio \
  		-m size=4096
fi
#if [ "$1" = "clean" ]; then
  ./scripts/clean.sh
#fi

#!/bin/sh
set -e
./scripts/clean.sh
. ./scripts/iso.sh

if [ "$1" = "debug" ]; then
	qemu-system-"$(./scripts/target-triplet-to-arch.sh "$HOST")" -s -S \
		-cdrom dentos.iso \
		-serial stdio \
		-m size=4096
	exit
fi
qemu-system-"$(./scripts/target-triplet-to-arch.sh "$HOST")" \
  -cdrom dentos.iso \
  -serial stdio \
  -m size=4096

if [ "$1" = "clean" ]; then
  ./scripts/clean.sh
fi

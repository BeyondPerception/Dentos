#!/bin/sh
set -e
./scripts/clean.sh
. ./scripts/iso.sh

qemu-system-"$(./scripts/target-triplet-to-arch.sh "$HOST")" \
  -cdrom dentos.iso

if [ "$1" = "clean" ]; then
  ./scripts/clean.sh
fi

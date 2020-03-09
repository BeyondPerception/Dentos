#!/bin/sh
set -e

if [ "$1" = "clean" ]; then
  ./scripts/clean.sh
else
  ./scripts/clean.sh
  . ./scripts/iso.sh

  qemu-system-"$(./scripts/target-triplet-to-arch.sh "$HOST")" \
    -cdrom dentos.iso
fi

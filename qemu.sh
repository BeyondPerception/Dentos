#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./scripts/target-triplet-to-arch.sh $HOST) \
			-cdrom dentos.iso \
			-nographic

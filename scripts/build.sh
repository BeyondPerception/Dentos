#!/bin/sh
set -e
echo "Building projects"
. ./scripts/headers.sh

for PROJECT in $PROJECTS; do
  (cd "$PROJECT" && DESTDIR="$SYSROOT" $MAKE install)
done

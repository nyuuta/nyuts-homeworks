#!/bin/bash

MOUNT_POINT="/tmp/myfs"

mkdir -p "$MOUNT_POINT"
chmod 755 "$MOUNT_POINT"

./myfs "$MOUNT_POINT" -f -d

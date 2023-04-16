#!/bin/bash
CURRENT_PATH=$(pwd)
PROJECT_PATH="$CURRENT_PATH"/..
TARGET=$1

mkdir "$PROJECT_PATH"/mntdir
sudo mount "$PROJECT_PATH"/tizen-image/rootfs.img "$PROJECT_PATH"/mntdir
sudo cp  "$TARGET" "$PROJECT_PATH"/mntdir/root
sudo umount "$PROJECT_PATH"/mntdir
rm -rf "$PROJECT_PATH"/mntdir


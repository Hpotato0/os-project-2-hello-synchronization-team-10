#!/bin/bash
PROJECT_PATH="/home/hyeonseok/os_project/project-2-hello-synchronization-team-10"
TARGET=$1

mkdir "$PROJECT_PATH"/mntdir
sudo mount "$PROJECT_PATH"/tizen-image/rootfs.img "$PROJECT_PATH"/mntdir
sudo cp  "$TARGET" "$PROJECT_PATH"/mntdir/root
sudo umount "$PROJECT_PATH"/mntdir
rm -rf "$PROJECT_PATH"/mntdir


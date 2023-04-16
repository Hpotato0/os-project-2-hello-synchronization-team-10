#!/bin/bash
PROJECT_PATH="/home/hyeonseok/os_project/project-2-hello-synchronization-team-10"

for CODE in "student" "rotd" "professor"
do
  aarch64-linux-gnu-gcc ""$CODE".c" -o "$CODE" -static -lm
done

mkdir "$PROJECT_PATH"/mntdir
sudo mount "$PROJECT_PATH"/tizen-image/rootfs.img "$PROJECT_PATH"/mntdir
for CODE in "student" "rotd" "professor"
do
  sudo cp  "$CODE" "$PROJECT_PATH"/mntdir/root 
done
sudo umount "$PROJECT_PATH"/mntdir
rm -rf "$PROJECT_PATH"/mntdir
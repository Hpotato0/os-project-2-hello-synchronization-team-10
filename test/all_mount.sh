#!/bin/bash
CURRENT_PATH=$(pwd)
PROJECT_PATH="$CURRENT_PATH"/..

for CODE in "student" "rotd" "professor" "slow_rotd" "slow_readlock" "slow_writelock" "slow_unlock"
do
  aarch64-linux-gnu-gcc ""$CODE".c" -o "$CODE" -static -lm
done

mkdir "$PROJECT_PATH"/mntdir
sudo mount "$PROJECT_PATH"/tizen-image/rootfs.img "$PROJECT_PATH"/mntdir
for CODE in "student" "rotd" "professor" "slow_rotd" "slow_readlock" "slow_writelock" "slow_unlock"
do
  sudo cp  "$CODE" "$PROJECT_PATH"/mntdir/root 
done
sudo umount "$PROJECT_PATH"/mntdir
rm -rf "$PROJECT_PATH"/mntdir
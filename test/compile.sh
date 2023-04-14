#!/bin/bash
CODE=$1
aarch64-linux-gnu-gcc ""$CODE".c" -o "$CODE" -static -lm

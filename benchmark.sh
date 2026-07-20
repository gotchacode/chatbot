#!/bin/bash

set -eu

printf '%s\n' '==========================================' 'C23 Chatbot Build Benchmark' '=========================================='

cd c
make clean >/dev/null
start=$(date +%s%N)
make >/dev/null
end=$(date +%s%N)
time=$(( (end - start) / 1000000 ))
size=$(ls -lh chat | awk '{print $5}')

printf 'Build time:      %sms\n' "$time"
printf 'Executable size: %s\n' "$size"

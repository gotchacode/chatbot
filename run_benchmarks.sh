#!/bin/bash

set -eu

printf '%s\n' '==========================================' 'C23 Chatbot Runtime Test' '=========================================='

cd c
make clean >/dev/null
make >/dev/null
cd ..

start=$(date +%s%N)
./c/chat < test_inputs.txt > /tmp/chatbot_output.txt
end=$(date +%s%N)
time=$(( (end - start) / 1000000 ))

printf 'Execution time: %sms\n' "$time"
printf 'Output saved:   /tmp/chatbot_output.txt\n'

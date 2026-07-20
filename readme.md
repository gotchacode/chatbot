# Chatbot

A small keyword-based chatbot written in a deliberately conservative subset of C.

## Building

This project requires a compiler with C23 support. The build rejects older language
modes rather than silently falling back to a draft mode.

```bash
cd c
make
./chat
```

The compiler is invoked with `-std=c23` and strict warnings. The implementation
avoids type-punning, variable-length arrays, `calloc`, `memset`-based initialization,
and pointer arithmetic outside object bounds.

## Testing

Run the chatbot with representative input:

```bash
printf 'hi\npython\nexit\n' | ./c/chat
```

Run the build and runtime benchmark:

```bash
./benchmark.sh
./run_benchmarks.sh
```

## Design

The chatbot stores keyword/response pairs in a separately chained hash table. Every
allocation is checked, ownership is explicit, and cleanup walks each chain before
releasing the table.

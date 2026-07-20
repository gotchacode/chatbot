# Chatbot Benchmark

The project now has one implementation, compiled as C23 with strict warnings.

## Build configuration

```text
-std=c23 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wstrict-prototypes -O2
```

Run the build benchmark with `./benchmark.sh` and the runtime smoke test with
`./run_benchmarks.sh`. Measurements depend on the compiler, operating system, and
machine load, so they are intentionally generated locally instead of committed as
fixed comparison data.

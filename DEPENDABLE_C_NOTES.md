# Dependable C notes

Reviewed [Dependable C](https://dependablec.org/), especially its recommendations,
undefined-behaviour guidance, and initialization sections.

## Applied to this codebase

- Heap storage is allocated with `malloc`, then every pointer slot is initialized by
  assignment. This avoids using `calloc` or `memset` as a substitute for initializing
  typed objects.
- Strings are copied with `memcpy` only after allocation succeeds.
- The implementation does not type-pun, convert integers to pointers, use variable
  length arrays, perform arithmetic between unrelated objects, or use pointers after
  `free`.
- Allocation failures are checked during table creation and response setup.
- Input uses a fixed-size character array and bounded `fgets`, so input cannot grow a
  buffer dynamically or rely on a VLA.
- Ownership is explicit: the table owns each duplicated key and value, and destruction
  releases every chain before releasing the bucket array.

These rules reduce common undefined behaviour, but they do not make C memory-safe.
Runtime analysis with sanitizers and compiler warnings remains appropriate.

## C23-only support

Yes. The Makefile now has one language mode, `-std=c23`, and does not fall back to
`c2x` or build an older language mode. A compiler must therefore provide the C23
option. The source itself intentionally uses a conservative C subset, so it should
also be straightforward to audit and port between conforming C23 implementations.

C23 compiler support is implementation-dependent. Verify the target compiler before
building, and keep strict warnings enabled. GCC documents its C dialect selection in
[ GCC options for C ](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html), and
Clang documents its language-mode options in
[ Command-Line Argument Reference ](https://clang.llvm.org/docs/ClangCommandLineReference.html).

## Scope

The site is a practical coding guide, not a formal proof system. Some advice on the
site deliberately chooses a conservative engineering rule where the C standard and
compiler behaviour have historically been difficult to reason about. The rules above
are suitable for this small program, but a larger project should turn them into a
review checklist and add static analysis, sanitizers, and tests.

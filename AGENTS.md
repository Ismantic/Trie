# Repository Guidelines

## Project Structure & Module Organization

This repository contains three C++ trie implementations. `trie.h` and `trie.cc` define `CritbitTrie` and `DoubleArrayTrie`; `double_array.h` contains the header-only XOR-based `DoubleArray` template. `example.cc` demonstrates the compiled implementations, while `double_array_test.cc` is the automated test program for the header-only implementation. `README.md` links to the canonical conceptual chapters in the Text book; do not duplicate those chapters here. Keep new public APIs in the appropriate header and implementation details in `trie.cc` unless the code must remain templated.

## Build, Test, and Development Commands

The project requires GNU Make and a C++17-capable compiler (the Makefile defaults to `g++`).

- `make`: builds both `example` and `double_array_test` with warnings enabled.
- `make test`: builds and runs the automated test executable.
- `./example`: runs the sample and diagnostic code manually.
- `make clean`: removes binaries and object files.

Run `make clean && make test` before submitting changes that affect headers, build flags, or trie behavior.

## Coding Style & Naming Conventions

Follow the style of the surrounding C++ code: four-space indentation, braces on the same line, and C++17 standard-library facilities. Types and public methods use `PascalCase` (`CritbitTrie`, `GetValues`); local variables and free test functions use `snake_case`; private data members use a trailing underscore (`root_`, `base_`). Keep code inside the `trie` namespace and prefer RAII containers and smart pointers for new ownership. There is no configured formatter or linter, so compile with the Makefile's `-Wall -Wextra` flags and resolve new warnings.

## Testing Guidelines

Tests use a lightweight in-file `check` helper rather than an external framework. Add focused `test_<behavior>()` functions to `double_array_test.cc`, invoke them from `main`, and include success, failure, empty-input, and prefix-overlap cases where relevant. A test run must exit with status zero and report no failures. Changes to `CritbitTrie` or `DoubleArrayTrie` should also receive automated coverage; add a new test target to the Makefile if a separate test file is clearer.

## Commit & Pull Request Guidelines

Recent commits use short, imperative summaries such as `Restructure project for open-source release`. Keep each commit focused and describe the outcome, not the editing process. Pull requests should explain the behavior changed, identify affected implementation(s), and list verification commands. Link related issues when available; include output examples for user-visible API or documentation changes. Do not commit generated binaries (`example`, `double_array_test`) or object files.

# Project: C++20 Sensor Telemetry & Anomaly Processor

## Tech Stack & Mandatory Verification
- Language: Modern C++20 (`CMAKE_CXX_STANDARD 20`), CMake, GoogleTest (`FetchContent`).
- Compiler flags: `-Wall -Wextra -Werror`
- Mandatory verification command (MUST run and pass after every C++ change):
  (local macOS; ASan/TSan run later in a Linux Docker container)
  `rm -rf build && cmake -B build -DENABLE_SANITIZERS=OFF && cmake --build build -j && ctest --test-dir build --output-on-failure`

## Engineering Rules
- Use RAII everywhere; raw `new`, `delete`, and owning raw pointers are forbidden.
- Prefer `std::optional`, `std::span`, and `[[nodiscard]]` on getters/pop operations.
- Keep source and header files modular and under 200 lines.
- Every component must have GoogleTest unit tests covering normal flow, boundary conditions (empty/full), and invalid inputs.

## Workflow
- Be concise: list changed files and test results only.

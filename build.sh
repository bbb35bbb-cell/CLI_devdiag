#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

case "${1:-}" in
  build)
    cmake -S . -B build -DCMAKE_BUILD_TYPE="${BUILD_TYPE:-Release}"
    cmake --build build --parallel "${JOBS:-$(nproc)}"
    ;;
  test)
    cmake -S . -B build -DCMAKE_BUILD_TYPE="${BUILD_TYPE:-Debug}"
    cmake --build build --parallel "${JOBS:-$(nproc)}"
    ctest --test-dir build --output-on-failure
    ;;
  install)
    cmake -S . -B build -DCMAKE_BUILD_TYPE="${BUILD_TYPE:-Release}"
    cmake --build build --parallel "${JOBS:-$(nproc)}"
    cmake --install build --prefix "${PREFIX:-$SCRIPT_DIR/install}"
    ;;
  clean)
    rm -rf build
    ;;
  *)
    echo "Usage: $0 {build|clean}" >&2
    exit 1
    ;;
esac

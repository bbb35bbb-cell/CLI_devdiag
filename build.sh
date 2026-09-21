#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

case "${1:-}" in
  build)
    cmake -S . -B build
    cmake --build build
    ;;
  clean)
    rm -rf build
    ;;
  *)
    echo "Usage: $0 {build|clean}" >&2
    exit 1
    ;;
esac

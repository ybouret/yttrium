#!/bin/sh
ninja -C build-gcc rebuild_cache && ninja -C build-gcc   && \
ninja -C build-clang rebuild_cache && ninja -C build-clang && \
echo "Installing from clang" && \
(ninja -C build-clang install &> /dev/null)

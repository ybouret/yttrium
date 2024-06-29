#!/bin/sh
ninja -C build-gcc   && \
ninja -C build-clang && \
echo "Installing from clang" && \
(ninja -C build-clang install &> /dev/null)

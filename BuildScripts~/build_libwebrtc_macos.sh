#!/bin/bash

if [ ! -e "$(pwd)/depot_tools" ]
then
  git clone --depth 1 https://chromium.googlesource.com/chromium/tools/depot_tools.git
fi

export PATH="$(pwd)/depot_tools:$PATH"
export WEBRTC_VERSION=77
export OUTPUT_DIR=out

fetch webrtc

cd src
git config --system core.longpaths true
git branch -r
git checkout -b my_branch "refs/remotes/branch-heads/$(WEBRTC_VERSION)"
cd ..

gclient sync -f

gn gen "$(OUTPUT_DIR)" --root="src" --args="is_debug=false is_clang=false target_cpu=\"x64\" symbol_level=0 enable_iterator_debugging=false"
ninja -C "$(OUTPUT_DIR)"

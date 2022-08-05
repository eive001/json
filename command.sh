#!/bin/bash
mkdir code
cd code
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y --no-modify-path
git clone https://github.com/kata-containers/kata-containers.git
cd  kata-containers/src/agent
make

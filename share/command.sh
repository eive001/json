#!/bin/bash
cd .. && mkdir code
cd code
RUSTFLAGS='--cfg procmacro2_semver_exempt'
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y  --no-modify-path
echo  [source.tuna] >> /root/.cargo/config
echo  registry = \" https://mirrors.tuna.tsinghua.edu.cn/git/crates.io-index.git\" >> /root/.cargo/config
source "$HOME/.cargo/env"
export PATH=/root/.cargo/bin:$PATH 
rustup install 1.53.0 && rustup target add x86_64-unknown-linux-musl
git clone https://github.com/kata-containers/kata-containers.git
cd  kata-containers/src/agent
echo ==================ready to make=======================================
dettrace make
cd /home/rebuild
echo make finish ========== 
pwd
shasum /home/code/kata-containers/src/agent/target/x86_64-unknown-linux-musl/release/kata-agent >> checksum.txt
sha256sum /home/code/kata-containers/src/agent/target/x86_64-unknown-linux-musl/release/kata-agent >> checksum.txt
md5sum /home/code/kata-containers/src/agent/target/x86_64-unknown-linux-musl/release/kata-agent >> checksum.txt

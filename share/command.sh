#!/bin/bash
set -x
cd /home
mkdir code
cd code
export SSL_CERT_FILE=/etc/ssl/certs/ca-certificates.crt
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y  --no-modify-path
source "$HOME/.cargo/env"
rustup install 1.53.0 && rustup target add x86_64-unknown-linux-musl
git clone https://github.com/eive001/kata-containers.git
cd  kata-containers/src/agent
make fetch
dettrace --base_env host -- make build_offline
md5sum /home/code/kata-containers/src/agent/target/x86_64-unknown-linux-musl/release/kata-agent >> /home/code/checksum
shasum /home/code/kata-containers/src/agent/target/x86_64-unknown-linux-musl/release/kata-agent >> /home/code/checksum
sha256sum /home/code/kata-containers/src/agent/target/x86_64-unknown-linux-musl/release/kata-agent >> /home/code/checksum

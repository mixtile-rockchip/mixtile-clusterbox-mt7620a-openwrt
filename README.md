## Build

##### First install Linux system, recommended ubuntu:20.04

### 1. Install build dependencies

```bash
# Ubuntu / Debian
apt-get install -y build-essential clang flex g++ gawk gcc-multilib gettext git libncurses5-dev libssl-dev python python3-distutils rsync unzip zlib1g-dev file wget swig python3-pip
```

### 2. Download the source code, update feeds and select configuration

```bash
git clone -b v110 https://github.com/mixtile-rockchip/mixtile-clusterbox-mt7620a-openwrt.git

./build.sh feeds_install

./build.sh cluster-box-config

./build.sh all
```

### 3. Build output

```bash
bin/
├── packages
└── targets
    └── ramips
        └── mt7620
            ├── config.buildinfo
            ├── feeds.buildinfo
            ├── openwrt-ramips-mt7620-cluster-box-control-V100-initramfs-kernel.bin
            ├── openwrt-ramips-mt7620-cluster-box-control-v100.manifest
            ├── openwrt-ramips-mt7620-cluster-box-control-V100-squashfs-sysupgrade.bin
            ├── packages
            ├── profiles.json
            ├── sha256sums
            └── version.buildinfo


output/
└── image-release-clusterbox-openwrt22-xxxxx.bin
```
### 4. Upgrade Firmware
[`look here`](https://www.mixtile.com/docs/getting-started-with-cluster-box/)

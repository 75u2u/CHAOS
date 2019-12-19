# CHAOS
Common Hackable Adaptable Operating System

## Screenshots
![screenshot](https://github.com/75u2u/CHAOS/blob/master/img/screenshot.png)

## What's this ?
このレポジトリには2019年のSecHack365で開発しているOS（CHAOS）のコードが入っています．CHAOSはKL-01ライセンスなので自由に改変してください．

まだ開発途中ですが，拡張性に優れて自由に改造できる(ハッカブルな)組込みOSを目指します．
フルスクラッチでARM64bitで動作するOSというのが特徴でお手持ちのRaspberry Pi 3 model bで動作します．
またQEMUを使ったエミュレーションも可能です．

サポートしている機能
- UART
- GPIO
- RNG

This repository contains the code of the OS (CHAOS) developed by SecHack365 in 2019. CHAOS is a KL-01 license, so feel free to hack it.

Although it is still under development, it aims to be a (hackable) embedded OS that is highly extensible and can be freely modified.
CHAOS runs on your Raspberry Pi 3 model b, which is characterized by an OS that runs on ARM64bit with full scratches.
CHAOS can emulate on QEMU.  

Supported functions
- UART
- GPIO
- RNG

## Install
1. CHAOSをクローンします．  
clone CHAOS.
```
git clone https://github.com/75u2u/CHAOS
```
2. クロスコンパイル用の開発環境を構築するために必要なツールチェインをダウンロードします．  
Download the toolchain required to build a development environment for cross compilation.
```
wget https://ftpmirror.gnu.org/binutils/binutils-2.30.tar.gz
wget https://ftpmirror.gnu.org/gcc/gcc-8.1.0/gcc-8.1.0.tar.gz
wget https://ftpmirror.gnu.org/mpfr/mpfr-4.0.1.tar.gz
wget https://ftpmirror.gnu.org/gmp/gmp-6.1.2.tar.bz2
wget https://ftpmirror.gnu.org/mpc/mpc-1.1.0.tar.gz
wget https://gcc.gnu.org/pub/gcc/infrastructure/isl-0.18.tar.bz2
wget https://gcc.gnu.org/pub/gcc/infrastructure/cloog-0.18.1.tar.gz

sudo apt install g++-arm-linux-gnueabihf
sudo apt install qemu-user-binfmt
sudo apt install g++-aarch64-linux-gnu qemu-user-binfmt
sudo ln -s /usr/aarch64-linux-gnu/lib/ /lib/aarch64-linux-gnu
sudo ln -s /lib/aarch64-linux-gnu/ld-2.23.so /lib/ld-linux-aarch64.so.1

sudo apt-get install minicom
```
3. ダウンロードしたファイルを解凍します．  
Unzip the downloaded files.
```
for i in *.tar.gz; do tar -xzf $i; done
for i in *.tar.bz2; do tar -xjf $i; done
```
4. ツールチェインを使えるようにするためリンクします．  
Link to make the toolchain usable.
```
cd binutils-*
ln -s ../isl-* isl
cd ..
cd gcc-*
ln -s ../isl-* isl
ln -s ../mpfr-* mpfr
ln -s ../gmp-* gmp
ln -s ../mpc-* mpc
ln -s ../cloog-* cloog
cd ..
```
5. binutilsをビルドします．  
Build binutils.
```
mkdir aarch64-binutils
cd aarch64-binutils
../binutils-*/configure --prefix=/usr/local/cross-compiler --target=aarch64-elf \
--enable-shared --enable-threads=posix --enable-libmpx --with-system-zlib --with-isl --enable-__cxa_atexit \
--disable-libunwind-exceptions --enable-clocale=gnu --disable-libstdcxx-pch --disable-libssp --enable-plugin \
--disable-linker-build-id --enable-lto --enable-install-libiberty --with-linker-hash-style=gnu --with-gnu-ld\
--enable-gnu-indirect-function --disable-multilib --disable-werror --enable-checking=release --enable-default-pie \
--enable-default-ssp --enable-gnu-unique-object
make -j4
make install
cd ..
```
6. gccをビルドします．  
Build gcc.
```
mkdir aarch64-gcc
cd aarch64-gcc
../gcc-*/configure --prefix=/usr/local/cross-compiler --target=aarch64-elf --enable-languages=c \
--enable-shared --enable-threads=posix --enable-libmpx --with-system-zlib --with-isl --enable-__cxa_atexit \
--disable-libunwind-exceptions --enable-clocale=gnu --disable-libstdcxx-pch --disable-libssp --enable-plugin \
--disable-linker-build-id --enable-lto --enable-install-libiberty --with-linker-hash-style=gnu --with-gnu-ld\
--enable-gnu-indirect-function --disable-multilib --disable-werror --enable-checking=release --enable-default-pie \
--enable-default-ssp --enable-gnu-unique-object
make -j4 all-gcc
make install-gcc
cd ..
```
7. ビルドに成功すると以下のディレクトリに実行ファイルが展開されます．  
If the build is successful, the executable file will be expanded to the following directory.
```
$ ls /usr/local/cross-compiler/bin
aarch64-elf-addr2line  aarch64-elf-elfedit    aarch64-elf-gcc-ranlib  aarch64-elf-ld       aarch64-elf-ranlib
aarch64-elf-ar         aarch64-elf-gcc        aarch64-elf-gcov        aarch64-elf-ld.bfd   aarch64-elf-readelf
aarch64-elf-as         aarch64-elf-gcc-7.2.0  aarch64-elf-gcov-dump   aarch64-elf-nm       aarch64-elf-size
aarch64-elf-c++filt    aarch64-elf-gcc-ar     aarch64-elf-gcov-tool   aarch64-elf-objcopy  aarch64-elf-strings
aarch64-elf-cpp        aarch64-elf-gcc-nm     aarch64-elf-gprof       aarch64-elf-objdump  aarch64-elf-strip
```

## Usage

CHAOS のビルド  
Build CHAOS
```
make
```
QEMU上でエミュレーション  
Emulate on QEMU
```
make run
```
SDカードにCHAOSを書き込む  
Write CHAOS into SD card
```
cp kernel8.img /media/USERNAME/SD/.
cp bootcode.bin /media/USERNAME/SD/.
cp config.txt /media/USERNAME/SD/.
cp start.elf /media/USERNAME/SD.
```
ラズベリーパイ3と通信  
Communicate with Raspberry Pi 3
```
make com
```
詳細はMakefileを参照してください．  
Please check Makefile for more details.


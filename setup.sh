### CHAOS開発環境構築セットアップ ###

wget https://ftpmirror.gnu.org/binutils/binutils-2.30.tar.gz
wget https://ftpmirror.gnu.org/gcc/gcc-8.1.0/gcc-8.1.0.tar.gz
wget https://ftpmirror.gnu.org/mpfr/mpfr-4.0.1.tar.gz
wget https://ftpmirror.gnu.org/gmp/gmp-6.1.2.tar.bz2
wget https://ftpmirror.gnu.org/mpc/mpc-1.1.0.tar.gz
wget https://gcc.gnu.org/pub/gcc/infrastructure/isl-0.18.tar.bz2
wget https://gcc.gnu.org/pub/gcc/infrastructure/cloog-0.18.1.tar.gz

for i in *.tar.gz; do tar -xzf $i; done
for i in *.tar.bz2; do tar -xjf $i; done

cd /usr/local/src
sudo wget http://zlib.net/zlib-1.2.11.tar.gz
sudo tar xvzf zlib-1.2.11.tar.gz
cd zlib-1.2.11
sudo ./configure
sudo make
sudo make install

cd /usr/local/src
sudo wget https://gmplib.org/download/gmp/gmp-6.2.0.tar.lz
sudo tar --lzip -xvf gmp-6.2.0.tar.lz
sudo apt install lzip
sudo ./configure
sudo make
sudo make install

sudo apt install m4

sudo apt-get install build-essential libcairo2-dev libpango1.0-dev libjpeg-dev libgif-dev librsvg2-dev
sudo apt install minicom

cd binutils-2.30
ln -s ../isl-0.18 isl
cd ..
cd gcc-8.1.0
ln -s ../isl-0.18 isl
ln -s ../mpfr-4.0.1 mpfr
ln -s ../gmp-6.1.2 gmp
ln -s ../mpc-1.1.0 mpc
ln -s ../cloog-0.18.1 cloog
cd ..

mkdir aarch64-binutils
cd aarch64-binutils
../binutils-2.30/configure --prefix=/usr/local/cross-compiler --target=aarch64-elf \
--enable-shared --enable-threads=posix --enable-libmpx --with-system-zlib --with-isl --enable-__cxa_atexit \
--disable-libunwind-exceptions --enable-clocale=gnu --disable-libstdcxx-pch --disable-libssp --enable-plugin \
--disable-linker-build-id --enable-lto --enable-install-libiberty --with-linker-hash-style=gnu --with-gnu-ld\
--enable-gnu-indirect-function --disable-multilib --disable-werror --enable-checking=release --enable-default-pie \
--enable-default-ssp --enable-gnu-unique-object
sudo make
sudo make install
cd ..

mkdir aarch64-gcc
cd aarch64-gcc
../gcc-8.1.0/configure --prefix=/usr/local/cross-compiler --target=aarch64-elf --enable-languages=c \
--enable-shared --enable-threads=posix --enable-libmpx --with-system-zlib --with-isl --enable-__cxa_atexit \
--disable-libunwind-exceptions --enable-clocale=gnu --disable-libstdcxx-pch --disable-libssp --enable-plugin \
--disable-linker-build-id --enable-lto --enable-install-libiberty --with-linker-hash-style=gnu --with-gnu-ld\
--enable-gnu-indirect-function --disable-multilib --disable-werror --enable-checking=release --enable-default-pie \
--enable-default-ssp --enable-gnu-unique-object
sudo make all-gcc
sudo make install-gcc
cd ..

ls /usr/local/cross-compiler/bin

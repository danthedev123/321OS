# ------------------------------------
# Build GCC cross compiler for use with 321OS. For use on Debian/Ubuntu machine.
# ------------------------------------

mkdir /tmp/cross-build
cd /tmp/cross-build

echo ==== Installing dependencies ====

sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev

echo ==== Downloading sources ====

wget https://ftp.gnu.org/gnu/binutils/binutils-2.38.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz

echo ==== Extracting Binutils sources ====

tar xf binutils-2.38.tar.xz
tar xf gcc-11.2.0.tar.gz

echo ==== Preparing for binutils build ====

mkdir -p "$HOME/cross-compiler"

mkdir build-binutils

export PREFIX="$HOME/cross-compiler"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

cd build-binutils

../binutils-2.38/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

echo ==== Building binutils =====
make
make install

cd ..

echo ==== Preparing for GCC build ====

mkdir build-gcc
cd build-gcc

../gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

echo ==== Building GCC ====
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

echo ================================================================
echo Build finished. New cross compiler is located at ~/cross-compiler/bin
echo ================================================================

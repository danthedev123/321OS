<h1 align="center">321OS</h1>

<p align="center">
   A hobbyist operating system designed for x86_64 and written in C.
   <br />
   <a href="https://github.com/danthedev123/321OS/releases"><strong>⬇️ Releases »</strong></a>
   <br />
   <br />
   <a href="https://github.com/danthedev123/321os/issues">Report bug</a>
   ·
   <a href="https://github.com/danthedev123/321os/issues">Request feature</a>
  </p>

## Building a cross compiler
In order to build 321OS, you need a cross compiler to a generic x86_64 target. This is because by default GCC will assume our code will run on a Linux machine rather than bare metal. This can cause many weird issues and errors.

If you are running an Ubuntu, Mint or Debian machine simply run the cross compiler build script.


```
chmod +x tools/crosscompiler.sh
tools/crosscompiler.sh
```
It will automatically build and install the toolchain. It will take around 20-30 minutes. If you're running a different distro, change the package manager commands or use this tutorial by OS Dev. https://wiki.osdev.org/GCC_Cross-Compiler

## Docker (recommended)
### Coming soon!

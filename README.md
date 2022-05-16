<h1 align="center">321OS</h1>

<p align="center">
   A hobbyist operating system designed for x86_64 and written in C++. In the future, it could be ported to other architectures such as IA-32 (32-bit x86). The goal is to eventually reach userspace and allow for drivers in both kernel mode and also user mode to support malicious or buggy drivers.
   <br />
   <a href="https://github.com/danthedev123/321OS/releases"><strong>⬇️ Releases »</strong></a>
   <br />
   <br />
   <a href="https://github.com/danthedev123/321os/issues">Report bug</a>
   ·
   <a href="https://github.com/danthedev123/321os/issues">Request feature</a>
  </p>

![321OS screenshot](https://i.imgur.com/aasvcoE.png)


## Features
- Support for both gcc and clang compilers
- Stivale2 protocol, uses Limine bootloader.
- Custom GDT
- Interrupts
- Keyboard support
- Beeping using IBM PC Speaker


## Required tools
On Debian, Ubuntu or Mint use this command to install all the required tools
```
sudo apt install build-essential clang xorriso nasm
```

To install it on Arch or Manjaro, use this command
```
sudo pacman -S base-devel clang xorriso nasm
```

If you're using a Mac, I suggest using an Ubuntu docker image and installing these packages.

## Prebuilt Docker image (recommended for Windows or OS X users)
### Coming soon!



## Thanks to the following resources:
- osdev.org - Amazing wiki about OS development.
- Poncho's OS tutorial on YouTube
- Rust OS tutorial (older version) - https://os.phil-opp.com/multiboot-kernel
# CHIPBOX u-boot bootloader

This is the u-boot bootloader for Chipbox DVBS2 HD satellite receiver.

[What is "Chipbox"](https://www.drejo.com/blog/chipbox/)
[Some more details about chipbox](https://gitlab.com/stulluk/chipbox-pars)
[Even more data](https://www.drejo.com/blog/chipbox/)


## Prequisities & Building


- You need a toolchain. Chipbox has a very ancient toolchain here:
https://gitlab.com/stulluk/chipbox-pars/-/tree/main/crosstool/gcc-3.4.6-glibc-2.3.6/arm-linux
- Clone that repository and add this toolchain to your $PATH ( this will give you arm-linux-gcc )
- It is already configured for CSM1203 and 32Mbyte NOR Flash used by Chipbox
- There are two convenience shell scripts in this dir: ```ba``` (to build) and ```ca``` (to clean)
- This will create a file called ```su-boot.bin``` in the topdir, which is a secure version of u-boot.bin (I guess)
- .gitignore is up to date


## How to install it to Chipbox ?

There are two ways:

### Updating su-boot.bin via TFTP

TBE

### Installing via J-Link ARM JTAG

TBE

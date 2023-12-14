# Send message in C

This document was created on Debian based system.

## Perequesites

To confirm that everything is working, You need to compile [example from baresip's wiki](https://github.com/baresip/baresip/wiki/Using-baresip-as-a-library). It is already copied into this dir, to save You some typing. 

I assume that You know how to build and download both `baresip` and `libre`. If You do not check out [this article](https://github.com/KubaTaba1uga/python_baresip_bindings/blob/research_send_baresip_message_C/contributors/build_baresip_and_confirm_it_is_working/README.md). 

### Build libre
```
git clone -b 'v3.7.0' --single-branch --depth 1 https://github.com/baresip/re
cd re
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### Build baresip
```
git clone -b 'v3.7.0' --single-branch --depth 1 https://github.com/baresip/baresip.git
cd baresip
cmake -B build -DSTATIC=ON
cmake --build build -j
```

I'm using `taba1uga` as debian's username, both `libre` and `baresip` are already downloaded and built in my home directory.

Compilation command
```
gcc custom.c -Wl,--copy-dt-needed-entries -I/home/taba1uga/re/include -I/home/taba1uga/baresip/include -lbaresip -lre -L/home/taba1uga/re/build -L/home/taba1uga/baresip/build  -Wl,-rpath=/home/taba1uga/re/build -o custom.out
```

There may be an issue with shared libraries not being in `LD_LIBRARY_PATH`. Example shows how to fix missing path for `libbaresip.so.11`:
```
taba1uga@baresip:~/my_app$ gcc custom.c -I/home/taba1uga/re/include -I/home/taba1uga/baresip/include -lbaresip -lre -L/home/taba1uga/re/build -L/home/taba1uga/baresip/build  -Wl,-rpath=/home/taba1uga/re/build -o custom.out
taba1uga@baresip:~/my_app$ ./custom.out
./custom.out: error while loading shared libraries: libbaresip.so.11: cannot open shared object file: No such file or directory
taba1uga@baresip:~/my_app$ ldd custom.out
        linux-vdso.so.1 (0x00007ffe90f04000)
        libbaresip.so.11 => not found
        libre.so.19 => /home/taba1uga/re/build/libre.so.19 (0x00007f4bfafe2000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f4bfae01000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f4bfade2000)
        libssl.so.3 => /lib/x86_64-linux-gnu/libssl.so.3 (0x00007f4bfad38000)
        libcrypto.so.3 => /lib/x86_64-linux-gnu/libcrypto.so.3 (0x00007f4bfa800000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f4bfa721000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f4bfb0aa000)
taba1uga@baresip:~/my_app$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/taba1uga/baresip/build
taba1uga@baresip:~/my_app$ ldd custom.out
        linux-vdso.so.1 (0x00007ffe8dff7000)
        libbaresip.so.11 => /home/taba1uga/baresip/build/libbaresip.so.11 (0x00007f1cde145000)
        libre.so.19 => /home/taba1uga/re/build/libre.so.19 (0x00007f1cde089000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f1cddea3000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f1cdde84000)
        libssl.so.3 => /lib/x86_64-linux-gnu/libssl.so.3 (0x00007f1cdddda000)
        libcrypto.so.3 => /lib/x86_64-linux-gnu/libcrypto.so.3 (0x00007f1cdd800000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f1cddcf9000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f1cde19b000)
```

You need to fill all missing paths before running the exe.

If client started successfully, You can confirm that everything is working by pressing `h`, example:

![image](https://github.com/KubaTaba1uga/python_baresip_bindings/assets/73971628/21b87a5a-e2b6-44d6-b4d6-c9c2dd8fe448)

At this point We know how to compile custom `baresip` code, nice :)

## App to send and receive messages

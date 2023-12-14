# Send message in C

## Perequesites

To confirm that everything is working, You need to compile [example from baresip's wiki](https://github.com/baresip/baresip/wiki/Using-baresip-as-a-library). It is already copied into this dir, to save You some typing. 

Compilation command
```
gcc custom.c -I/root/re/include -I/root/baresip/include -lbaresip -lre -L/root/re/build -L/root/baresip/build  -Wl,-rpath=/root/re/build
```

There may be an issue with shared libraries not being in LD_LIBRARY_PATH. Example shows how to fix missing path for `libbaresip.so.11`:
```
./a.out: error while loading shared libraries: libbaresip.so.11: cannot open shared object file: No such file or directory
root@baresip:~/baresip/build# ldd ./a.out 
        linux-vdso.so.1 (0x00007fff745c6000)
        libbaresip.so.11 => not found
        libre.so.19 => /root/re/build/libre.so.19 (0x00007fe1fbdd6000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fe1fbbf5000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007fe1fbbd6000)
        libssl.so.3 => /lib/x86_64-linux-gnu/libssl.so.3 (0x00007fe1fbb2c000)
        libcrypto.so.3 => /lib/x86_64-linux-gnu/libcrypto.so.3 (0x00007fe1fb600000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fe1fb521000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fe1fbe9e000)
root@baresip:~/baresip/build# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/baresip/build
root@baresip:~/baresip/build# ldd ./a.out 
        linux-vdso.so.1 (0x00007ffd6cdae000)
        libbaresip.so.11 (0x00007f4938813000)
        libre.so.19 => /root/re/build/libre.so.19 (0x00007f4938757000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f4938571000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f4938552000)
        libssl.so.3 => /lib/x86_64-linux-gnu/libssl.so.3 (0x00007f49384a8000)
        libcrypto.so.3 => /lib/x86_64-linux-gnu/libcrypto.so.3 (0x00007f4938000000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f4937f21000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f4938869000)
```

You need to fill all missing paths before running the exe.

If client started successfully, You can confirm that essentials  modules loaded by pressing `h`, sth like below should appear:

![image](https://github.com/KubaTaba1uga/python_baresip_bindings/assets/73971628/21b87a5a-e2b6-44d6-b4d6-c9c2dd8fe448)

At this point We know how to compile custom `baresip` code, nice :)

## App to send and receive messages

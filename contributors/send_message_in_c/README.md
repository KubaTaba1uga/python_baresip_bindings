# Send message in C

To confirm that everything is working, You need to compile [example from baresip's wiki](https://github.com/baresip/baresip/wiki/Using-baresip-as-a-library). It is already copied into this dir, to save You some typing. 

Compilation command
```
gcc custom.c -I/root/re/include -I/root/baresip/include -lbaresip -lre -L/root/re/build -L/root/baresip/build  -Wl,-rpath=/root/re/build
```

To run exec file successfully You need to amke sure that shared libraries files exsist in `LD_LIBRARY_PATH` environment variable.

Adding libraries paths.
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/build
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/build
```

If command succeeded and the client is working You can proceed.

You can confirm that modules loaded by pressing `h`, sth like this should appear:
![image](https://github.com/KubaTaba1uga/python_baresip_bindings/assets/73971628/21b87a5a-e2b6-44d6-b4d6-c9c2dd8fe448)

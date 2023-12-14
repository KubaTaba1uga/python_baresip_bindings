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

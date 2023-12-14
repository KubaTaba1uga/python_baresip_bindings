# Send message in C

To confirm that everything is working, You need to compile [example from baresip's wiki](https://github.com/baresip/baresip/wiki/Using-baresip-as-a-library). It is already copied into this dir, to save You some typing. 

Compilation command
```
gcc custom.c -I./re/include -I./baresip/include -lbaresip -lre -L./re/build -L./baresip/build
```

If command succeeded and the client is working You can proceed.

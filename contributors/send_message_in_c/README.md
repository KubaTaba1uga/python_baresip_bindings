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
Static
```
git clone -b 'v3.7.0' --single-branch --depth 1 https://github.com/baresip/baresip.git
cd baresip
cmake -B build -DSTATIC=ON
cmake --build build -j
```

### Compile baresip's wiki example
I'm using `taba1uga` as debian's username, both `libre` and `baresip` are already downloaded and built in my home directory.

Compilation commands:

#### Static
```
gcc custom.c -Wl,--copy-dt-needed-entries -I/home/taba1uga/re/include -I/home/taba1uga/baresip/include -lbaresip -lre -L/home/taba1uga/re/build -L/home/taba1uga/baresip/build  -Wl,-rpath=/home/taba1uga/re/build -o custom.out
```

### Test 

If client started successfully, You can confirm that everything is working by pressing `h`, example:

![image](https://github.com/KubaTaba1uga/python_baresip_bindings/assets/73971628/21b87a5a-e2b6-44d6-b4d6-c9c2dd8fe448)

At this point We know how to compile custom `baresip` code, nice :)

## App to send and receive messages

The app's purpose is to show SIP p2p messaging. I chose this feature because of it's simplicity. 

The app is composed of one src file. To compile it You can use a script, ex.:
```
bash compile.sh /home/taba1uga/re /home/taba1uga/baresip
```

You need to start two instances of the app. One for server which will receive messages from the client.
The other one for a client which will send messages to the server.

Starting server, ex:
```
./client_server 'SERVER' '10.0.0.236:8080'
```

Starting client, ex:
```
./client_server 'CLIENT' '10.0.0.236:8080' '10.0.0.236:8181' 'i like flowers'
```

If everything is working messages should appear on server app:
![image](https://github.com/KubaTaba1uga/python_baresip_bindings/assets/73971628/9a279d50-ad76-475b-83e4-fd7f6c565536)

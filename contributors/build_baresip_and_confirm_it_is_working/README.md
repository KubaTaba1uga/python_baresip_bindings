# Send message via CLI

This example was created on Debian based system.

## Installation

### Re
Install re dependencies
```
apt-get install cmake openssl zlib1g zlib1g-dev libssl-dev
```
Clone repository
```
git clone https://github.com/baresip/re
```
Go into re
```
cd re
```
Setup re's build
```
cmake -B build
```
Build re
```
cmake --build build -j
```

### Baresip
Clone repository
```
git clone https://github.com/baresip/baresip.git
```
Install baresip dependencies
```
apt-get install cmake g++
```
Go into baresip
```
cd baresip
```
Setup baresip's build
```
cmake -B build -DSTATIC=ON
```
Build baresip
```
cmake --build build -j
```

TO-DO: is re working? shouldn't we include some path for cmake?

## Environment Configuration

 We need to specify few variables, which will be used further:
  - ip - ip of the machine where user_0's client and user_1's client will be started
  - user_0_port - port of the user_0's client
  - user_1_port - port of the user_1's client

There are two directories user_0 and user_1.
We need to edit some settings in each of them before we start.

### User 0
In user_0/config substitute 
```
sip_listen              <ip>:<user_0_port>
```
With real values, ex:
```
sip_listen              10.0.0.1:8080
```

In user_0/accounts substitute
```
<sip:user_0@<ip>:<user_0_port>>;regint=0
```
With real values, ex:
```
<sip:user_0@10.0.0.1:8080>;regint=0
```

In user_0/contacts substitute
```
"User 1" <sip:user_1@<ip>:<user_1_port>>
```
With real values, ex:
```
"User 1" <sip:user_1@10.0.0.1:8181>
```

### User 1
In user_1/config substitute 
```
sip_listen              <ip>:<user_1_port>
```
With real values, ex:
```
sip_listen              10.0.0.1:8181
```

In user_1/accounts substitute
```
<sip:user_1@<ip>:<user_1_port>>;regint=0
```
With real values, ex:
```
<sip:user_1@10.0.0.1:8181>;regint=0
```

In user_1/contacts substitute
```
"User 0" <sip:user_0@<ip>:<user_0_port>>
```
With real values, ex:
```
"User 0" <sip:user_0@10.0.0.1:8080>
```

## Sending message

We need two terminals' windows to perform next step.

### Start clients

On Terminal 0:
```
<baresip exe path> -f user_0/
```
Example:
```
 ~/baresip/build/baresip -f user_0/ 
```

On Terminal 1:
```
<baresip exe path> -f user_1/
```
Example:
```
 ~/baresip/build/baresip -f user_1/ 
```

### Send message
On (terminal 0 or terminal 1) Press `M`, type in the message, press `ENTER`.

The message should appear on the other client.
![image](https://github.com/KubaTaba1uga/python_baresip_bindings/assets/73971628/f0a0d225-6e2c-4f39-817d-1ee218302f95)



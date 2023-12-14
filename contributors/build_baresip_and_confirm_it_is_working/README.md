# Send message via CLI

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


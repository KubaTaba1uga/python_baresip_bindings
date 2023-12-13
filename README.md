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
cmake -B build
```
Build baresip
```
cmake --build build -j
```

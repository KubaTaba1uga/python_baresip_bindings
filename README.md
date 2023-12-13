# Send message via CLI

## Installation

Clone repositories
```
git clone https://github.com/baresip/baresip.git
git clone https://github.com/baresip/re
```
Install dependencies
```
apt-get install cmake openssl zlib1g zlib1g-dev libssl-dev
```
Go into re
```
cd re
```

Try to build re
```
cmake -B build
```

If error like this appeared
```
- Could NOT find OpenSSL, try to set the path to OpenSSL root folder in the system variable OPENSSL_ROOT_DIR (missing: OPENSSL_CRYPTO_LIBRARY OPENSSL_INCLUDE_DIR) (Required is at least version "1.1.1")
-- Could NOT find OpenSSL, try to set the path to OpenSSL root folder in the system variable OPENSSL_ROOT_DIR (missing: OPENSSL_CRYPTO_LIBRARY OPENSSL_INCLUDE_DIR) (Required is at least version "1.1.1")
...
```

Do
```

```

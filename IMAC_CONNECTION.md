# Remote Server Connection Details

## iMac (Local Network)

### Access
- **IP:** 192.168.18.184
- **SSH:** `ssh Anto@192.168.18.184` (port 22)
- **Password:** `1234`
- **Hostname:** antos-iMac.local
- **OS:** macOS Ventura (Darwin 22.6.0, x86_64)

### Build Tools
- **clang++:** Apple clang 14.0.3
- **make:** /usr/bin/make
- **cmake:** /Applications/CMake.app/Contents/bin/cmake (v3.28.3, installed manually)

### Benchmark (WiFi)
- **Throughput:** 784K rec/s (500K records in 0.69s)
- **Server port:** 6262 (or 16262 pre-existing)

---

## charleskwon.com (Remote Internet)

### Access
- **Host:** antonio.charleskwon.com
- **SSH:** `ssh -p 2222 antonio@antonio.charleskwon.com`
- **Password:** `antonio!@#$`
- **OS:** Ubuntu 24.04 Linux x86_64 (64 cores!)

### Build Tools
- **g++:** 12.4.0
- **cmake:** 3.28.3
- **make:** /usr/bin/make

### Notes
- Port 6262 is NOT accessible from outside (firewall)
- Use SSH tunnel: `ssh -f -N -L 16262:127.0.0.1:6262 -p 2222 antonio@antonio.charleskwon.com`
- Then connect: `tcp://127.0.0.1:16262//home/antonio/OpenADS/bench_data`
- FetchContent deps need pre-fetching: nlohmann_json, cpp-httplib, sqlite3

### Benchmark (SSH tunnel)
- **Throughput:** 676K rec/s (500K records in 0.74s)

---

## SSH Command Template
```powershell
# iMac
echo "1234" | ssh -o ConnectTimeout=10 Anto@192.168.18.184 "COMMAND"

# charleskwon (with tunnel)
ssh -f -N -L 16262:127.0.0.1:6262 -p 2222 antonio@antonio.charleskwon.com
# Password: antonio!@#$
```

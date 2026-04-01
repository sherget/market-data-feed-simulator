## Low-Latency Market Data Feed Simulator

A learning project exploring C++ systems programming through the lens of 
financial market data distribution. Inspired by how exchanges like Deutsche 
Börse stream real-time market data.

## Learning Goals
- C++ Fundamentals
- Socket Programming (TCP/UDP) in C++
- Multicast Networking
- Event-Driven Architecture transfer to C++
- Low-Latency Considerations
- CMake
- Google Test
- Kubernetes fundamentals (long shot via minikube)
- Streaming (Kafka)
- Time-Series Databases like InfluxDB

## Todo's:
- [x] Market Data Generator (Random Walk, 5 Ticker)
- [x] UDP Multicast Publisher
- [x] Measure latency (UDP)
- [ ] Kafka Integration
- [ ] Persistence (InfluxDB/TimescaleDB)
- [ ] Docker + Minikube

## Architecture:

- We simulate Data by generating Marketdata via a very simplified random walk method.
- We start the multicast broadcast
- The MarketDataGenerator has methods to add and remove symbols or rather tickers.
When MarketDataGenerator.generate is called it should iterate over our predifned default tickers and add them to the dynamic ticker vector (this should be a hashmap)
	- The moment a ticker is added it should spawn a selfcontained thread for that specific ticker and let it run indefinitely so we simulate constant price action
	- The Tickers should be held in a map so we know AAPL->thread1, GOOG->thread2 and if GOOG gets removed we can look into our hashmap and find the pointer to the thread and kill it
	- There is no need for locks or anything because the main focus currently is getting threads working in a manageable way. Since UDP or rather multicast doesn't care about correctness
	or order this approach should be fine. Apparently there are more reliable strategies or rather multicast "versions" like PGM(Pragmatic General Multicast), NORM(NACK-Oriented Reliable Multicast) and RMTP(Reliable Multicast Transport Protocol), that's something to keep in mind. But for now the main purpose is general understanding and getting a working version.
- We start a multicast subscriber and/or use socat to check that information flows
- Measure with perf before threads refactoring

## Perf usage:
Build with debug symbols:
```bash cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake --build build```
Run perf before a change (example) i just do 5 rounds (-r 5) and then take the average guess a bigger sample size would be better but for this project its plenty i think:
```bash perf stat -e cycles,instructions,cache-references,cache-misses,context-switches -r 5 ./build/market_data_feed_simulator publisher --ticks 10000```
Run same command again after whatever changes were implemented to compare the impact on performance.

To actually record perf.data instead of just printing output to terminal use:
```bash perf record -e cycles,instructions,cache-references,cache-misses,context-switches -F99 -g -- ./build/market_data_feed_simulator publisher --ticks 100000```

Go to the interactive report or use --stdio for standard output format:
perf report -i perf.data


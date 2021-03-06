# Proxy_Server_Cache
In this project, we have built a web proxy server was built, to measure and compare performance of different caching policies. Current set of experiments focus on maximizing the performance of a proxy cache that has a limited amount of cache space, and thus experiences a lot of contention. 
We have implemented five caching mechanisms Random, LRU, LFU, LIFO and FIFO compared their performance for varying types of workloads: uniformly distributed and random.

Cache Hit Rate, Latency and total number of requests are used to measure performance of these caching algorithms. We find that these algorithms perform variably depending on the request size and latency parameters but typically depends on the testing workload. 
The web proxy cache mainly consists of server and clients which communicate via Remote Procedure call based on Apache Thrift. Server handles the caching mechanism configurable through the Makefile. Cache size and capacity can be handled by constants in the gtcache header file.



---
Copyright (c) 2016 The persons identified as document authors. All rights reserved.

Code Components extracted from this document must include MIT License text.

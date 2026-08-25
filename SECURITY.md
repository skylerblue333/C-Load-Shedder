# Security

Sky Load Shedder is a local engineering-beta decision primitive.

The program parses bounded unsigned numeric CLI inputs, performs threshold arithmetic in `unsigned long long`, and does not process network traffic, secrets, files, or arbitrary code. CI runs strict compiler warnings plus AddressSanitizer and UndefinedBehaviorSanitizer contract tests.

The current product does not provide authentication, a network API, configuration signing, distributed policy coordination, live telemetry collection, or production SLA guarantees. Treat the output as one admission-control signal inside a larger verified system, not as a complete availability or DDoS-control layer.

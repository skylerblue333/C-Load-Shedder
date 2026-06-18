# C-Load-Shedder

## Overview
An adaptive load shedding system written in C that drops low-priority requests when the server is under heavy load, protecting system stability.

## Quick Start (1-Click Build)

```bash
git clone https://github.com/skylerblue333/C-Load-Shedder.git
cd C-Load-Shedder
mkdir build && cd build
cmake .. && make
./load_shedder
```

## Features
- Priority-based request queuing
- Adaptive threshold detection
- Zero-dependency implementation

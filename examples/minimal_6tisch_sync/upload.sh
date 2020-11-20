#!/bin/bash
make clean && make PORT=/dev/ttyUSB$(($1-1)) minimal_6tisch_sync.upload NODEID="$1" && make PORT=/dev/ttyUSB$(($1-1)) login
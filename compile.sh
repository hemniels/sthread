#!/bin/bash

# Erstelle den build Ordner falls nicht existent
mkdir -p build

# Kompiliere die Quellcode-Dateien
gcc -o build/scheduler main.c scheduler/scheduler.c datastruct/dll.c datastruct/processlist.c

# Kompiliere die Tests
gcc -o build/test_dll tests/test_dll.c datastruct/dll.c
gcc -o build/test_processlist tests/test_processlist.c datastruct/processlist.c
gcc -o build/test_scheduler tests/test_scheduler.c scheduler/scheduler.c datastruct/dll.c datastruct/processlist.c

echo "Build complete."

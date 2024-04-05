#!/bin/bash

# Define the starting directory for the search
start_dir=~/device_source

# Find .so files and check each for symbols, excluding those with "no symbols"
find "$start_dir" -type f -name 'libimp_control.so' | while read -r file; do
    nm_output=$(nm "$file" 2>&1)  # Capture both stdout and stderr
    if [[ ! $nm_output =~ "no symbols" ]] && [[ ! $nm_output =~ "file format not recognized" ]]; then
        echo "$file"
    fi
done

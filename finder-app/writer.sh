#!/bin/bash
writefile=$1
writestr=$2

# Check if arguments are provided
if [ -z "$writefile" ] || [ -z "$writestr" ]; then
    echo "Error: Missing arguments"
    echo "Usage: $0 <writefile> <writestr>"
    exit 1
elif
    [ -e "$writefile" ]; then
    echo "Error: $writefile already exists"
    exit 1
elif
    [ ! -d "$(dirname "$writefile")" ]; then
    echo "Error: Directory for $writefile does not exist, creating it"
    dirpath=$(dirname "$writefile")
    mkdir -p "$dirpath"
    echo "$writestr" > "$writefile"
    exit 1
fi

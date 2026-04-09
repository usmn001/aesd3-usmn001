#!/bin/sh
filesdir=$1
searchstr=$2

# Check if arguments are provided
if [ -z "$filesdir" ] || [ -z "$searchstr" ]; then
    echo "Error: Missing arguments"
    exit 1

elif [ ! -d "$filesdir" ]; then
    echo "Error: $filesdir is not a directory"
    exit 1
elif [ -d "$filesdir" ]; then 
    # Count number of files (recursively)
    file_count=$(find "$filesdir" -type f | wc -l)
    # Count number of matching lines (recursively)
    match_count=$(grep -r "$searchstr" "$filesdir" | wc -l)
fi

# Print result
echo "The number of files are $file_count and the number of matching lines are $match_count"

exit 0
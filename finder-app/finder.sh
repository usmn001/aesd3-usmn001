#!/bin/bash
filesdir=$1
searchstr=$2

# Check if arguments are provided
if [ -z "$filesdir" ] || [ -z "$searchstr" ]; then
    echo "Error: Missing arguments"
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi

# Check if filesdir is a directory
if [ ! -d "$filesdir" ]; then
    echo "Error: $filesdir is not a directory"
    exit 1
fi

# Count number of files (recursively)
file_count=$(find "$filesdir" -type f | wc -l)

# Count number of matching lines (recursively)
match_count=$(grep -r "$searchstr" "$filesdir" 2>/dev/null | wc -l)

# Print result
echo "The number of files are $file_count and the number of matching lines are $match_count"

exit 0
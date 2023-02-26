#!/bin/bash

# Recursively search for all files in the current directory and its subdirectories
find . -type f | while read file; do
  echo $(basename "$file")
  # Check if the filename contains at least one digit
  #if echo "$file" | grep -q '[0-9]'; then
    # Delete the file
   # rm "$file"
    #echo "Deleted file: $file"
  #fi
done

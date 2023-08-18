#!/bin/bash

# Run the command in the background
nc -c localhost 4444 &

# Sleep for a short duration to allow the background process to start
sleep 1

NICK=$1

# Send the input lines
echo "PASS 1337"
echo "NICK " + $NICK
echo "USER t t t t"

# Sleep again to give some time for the input to be processed
tail -f /dev/null

# Kill the background process

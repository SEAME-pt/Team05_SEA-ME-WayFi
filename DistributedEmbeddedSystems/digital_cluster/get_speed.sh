#!/bin/bash
end=$((SECONDS+20)) # Stop after 10 seconds
while [ $SECONDS -lt $end ]; do
    echo $((RANDOM % 60))
    sleep 0.5
done

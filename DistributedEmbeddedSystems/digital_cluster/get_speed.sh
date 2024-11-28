#!/bin/bash
end=$((SECONDS+20)) # Stop after 10 seconds
while [ $SECONDS -lt $end ]; do
    echo $((RANDOM % 200))
    sleep 0.5
done
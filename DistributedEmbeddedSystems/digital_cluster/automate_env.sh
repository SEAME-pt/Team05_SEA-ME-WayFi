#!/bin/bash
export $(grep -v '^#' .env | xargs)
cd build && cmake .. && make
./digital_cluster
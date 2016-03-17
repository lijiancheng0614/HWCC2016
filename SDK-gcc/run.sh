#!/bin/bash

cd build
cmake ../future_net
make
cd ../bin
./future_net topo.csv demand.csv result.csv

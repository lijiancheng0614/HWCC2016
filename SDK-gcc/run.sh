#!/bin/bash

cd build
cmake ../future_net
make
cd ../bin
./future_net ../../test-case/case0/topo.csv ../../test-case/case0/demand.csv result0.csv
./future_net ../../test-case/case1/topo.csv ../../test-case/case1/demand.csv result1.csv
./future_net ../../test-case/case2/topo.csv ../../test-case/case2/demand.csv result2.csv
./future_net ../../test-case/case3/topo.csv ../../test-case/case3/demand.csv result3.csv
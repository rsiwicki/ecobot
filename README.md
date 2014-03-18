ecobot
======

These are the base files for ecobot.

To build just run

git clone https://github.com/neuralnet/ecobot.git

make

Run the diagnostic with

./Agent

To run in the Arduino go to the Thresholds.h file and change the ARDUINO define to 1. Changing this setting will ensure that the compiler does not include code that the compile does not include superflous code such as the test diagnostic and data types are correct.

The MAXPOINTS define in Thresholds.h controls the number of data points that the ecobot will retain in working memory. It is planned that when this threshold is reached the ecobot runs a KNN algorithm over the data points in working memory to create a sparse representation of the its world. This is not yet implemenented. The KNN code is in KTree.cpp.

Currently the ecobot supports two strategies for navigating its world. The first is the COLLISION AVOIDANCE strategy. When using this strategy ecobot will attempt to detect an obstacle and any impending collision. The collision threshold is stored in Thresholds.h and is assumed to be in centimetres. The distance ecobot moves for each iteration is controlled by the STRIDE define, also in Thresholds.h. Ideally STRIDE should be smaller than the collision threshold. 

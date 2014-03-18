CC = g++
FILES = BPQ.h BPQPoint.h BPQ.cpp TreeNode.h KTree.h KTree.cpp LocF.h ActionTypes.h Vct.h Hn.h Action.h BMap.h BMap.cpp thresholds.h IStrategy.h StrategyTypes.h StrategyFactory.h StrategyFactory.cpp WorkingMemory.h WorkingMemory.cpp Vct.h Vct.cpp Utils.h Utils.cpp sensors.h sensors.c Agent.cpp YieldStrategy.h YieldStrategy.cpp CollisionAvoidanceStrategy.h CollisionAvoidanceStrategy.cpp
 
OUT_EXE = Agent

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)

#clean:
#        rm -f *.o core

#rebuild: clean build

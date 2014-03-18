#include "LocF.h"
#include "BMap.h"
#include "Vct.h"
#include "thresholds.h"
#include "Utils.h"

class WorkingMemory {
	private:
		LocF* _facts;
		int _size;

	public:
		WorkingMemory();
		~WorkingMemory() {delete [] _facts;}
		BMap* MaterializeWorld();
		LocF* GetLatestFact();	
		void AssertFact(LocF* fact);	
};

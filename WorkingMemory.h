#include "LocF.h"
#include "BMap.h"
#include "Vct.h"
#include "thresholds.h"
#include "Utils.h"
#include "Ring.h"

class WorkingMemory {
	private:
		//LocF* _facts;
		
		Ring<LocF>* _facts;
		
		int _size;
		Vct* rolledUpOrigin;
		Vct* currentVct;
	public:
		WorkingMemory();
		~WorkingMemory() {delete _facts;}
		BMap* MaterializeWorld();
		LocF* GetLatestFact();	
		void AssertFact(LocF* fact);	
		void UpdatePosition();
};

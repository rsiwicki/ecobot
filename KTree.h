#ifndef KTREE_H
#define KTREE_H

#include "TreeNode.h"
#include "BPQ.h"

class KTree {
	private:
		int _size;
		int _k;
		BPQ* _bpq;	
		TreeNode* _root;
	
	public:
		KTree(int k);
		~KTree();

		void insert(int x, int y, TreeNode* leaf, TreeNode* parent);
		void KNN(int x, int y, TreeNode* leaf, int forward);

		BPQ* getBPQ() {return _bpq;}
		void refreshBPQ();

		inline int kdround(float x) { return x>=0?(int)(x+0.5):(int)(x-0.5);}

};

#endif

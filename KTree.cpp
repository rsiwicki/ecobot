#include <math.h>
#include <iostream>
#include "KTree.h"

KTree::KTree(int k) {
	_k = k;

	_bpq = new BPQ(_k);
	_root = new TreeNode();

}

KTree::~KTree() {}

void KTree::refreshBPQ() {

	delete _bpq;

	_bpq = new BPQ(_k);

}


void KTree::insert(int x, int y, TreeNode *leaf, TreeNode *parent)
{
	if(leaf == NULL)
	{
		std::cout << "leaf is null so adding new node for " << x << "\n";

		leaf = new TreeNode();
		leaf->setX(x);
		leaf->setY(y);
		leaf->setLeft(0);    
		leaf->setRight(0);  
		leaf->setParent(parent);  
	
		// add to parent either left or right
		// DEBUG - must check y too
		if(x > parent->getX()) {
			parent->setRight(leaf);
		} else {
			parent->setLeft(leaf);
		}

	}
	else {
		if (0) { 
			if(y < leaf->getY()){
				insert(x,y, leaf->getLeft(), leaf);
			}
			else if(y > leaf->getY()){
				insert(x,y, leaf->getRight(), leaf);
			}
		} else {
			if(x < leaf->getX()){
				std::cout << "traversing left\n"; 
				std::cout << "x is " << x << " and leaf x is " << leaf->getX() << "so \n";
				insert(x,y, leaf->getLeft(), leaf);
			}
			else if(x > leaf->getX()){
				std::cout << "x is " << x << " and leaf x is " << leaf->getX() << "so \n";
				std::cout << "traversing right\n";
				insert(x,y, leaf->getRight(), leaf );
			}
		}
	}
}


void KTree::KNN(int x, int y, TreeNode* leaf, int forward) {

	std::cout << "finding nearest neighbour to x " <<  x << " " << y << "\n";

	if(leaf == NULL) {
	        std::cout << "exiting KNN at this level" << "\n";	
		return;
	}

	//if distance(curr, guess) < bestDist
 	//	bestDist = distance(curr, guess)
  	//	guess = curr

	int currentx = leaf->getX();
	int currenty = leaf->getY();
		
	int diffx = x - currentx;
	int diffy = y - currenty;

	float r = 0.0;
	r = sqrt((diffx * diffx) + (diffy * diffy)); // distance from currentGuess to target

	TreeNode* parent = leaf->getParent(); 	
	        	


	// for k > n need to keep a priority queue of NNs 
	//if(r < currentBestDist) {
	//	printf("setting best \n");
	//	currentBestDist = r;
	//	currentBest = leaf;
	//}

	BPQPoint* bpqp = new BPQPoint(); 
	bpqp->setX(currentx);
	bpqp->setY(currenty);
	bpqp->setP(kdround(r));

	_bpq->enqueue(bpqp);


	/* Recursively search the half of the tree that contains the test point. */
	//if ai < curri
	//	 recursively search the left subtree on the next axis
	//else
	//	 recursively search the right subtree on the next axis


	if (0) { 
		if(y < leaf->getY()){
			KNN(x,y, leaf->getLeft(),0);
		}
		else if(y > leaf->getY()){
			KNN(x,y, leaf->getRight(),1);
		}
	} else {
		if(x < leaf->getX()){
			KNN(x,y, leaf->getLeft(),0);
		}
		else if(x > leaf->getX()){
			KNN(x,y,leaf->getRight(),1);
		}
	}

	

	std::cout << "checking hypersphere  \n";

	//printf("parent is %d\n",parent);

	if(parent==NULL) {
		std::cout << "parent is null\n";
	} else {	


	std::cout << "parent x " << parent->getX() << "\n";
		/* If the candidate hypersphere crosses this splitting plane, look on the
 		*  * other side of the plane by examining the other subtree.
 		*   */
		// if |curri – ai| < bestDist
		//	  recursively search the other subtree on the next axis


		// is there a possibility that the best node is on the other side of the hyperplane
		float dist = 0.0;

		// get the nearest point on the hyperplane
		dist = parent->getSY() == 1 ? abs(parent->getY() - y) : abs(parent->getX() - x);  


		// if the radius around the piont touches the plane then need to search other side
		//if(dist <= currentBestDist) {
		//
		BPQPoint* lpp = _bpq->min_priority_elem(); // this is max distance so increases radius for k search
		
		std::cout << "lppp " << lpp->getP() << "\n";

		int lpp_dist = lpp->getP();

		if(dist <= lpp_dist || (!_bpq->is_full())) {
	
			// then need to search the of parent
			// on the opposite side - finish this
			std::cout << "nearest neighbour might be on other side\n";

			// I was searching right so search left
			// if the node is null then it is going to keep coming back in
		
			if(forward == 0) {
				std::cout << "getting parent right";
				KNN(x, y, parent->getRight(), 1);
			} else {
				std::cout << "getting parent left";
			    	KNN(x, y, parent->getLeft(), 0);
			}
		}
	}
}

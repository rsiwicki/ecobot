#include <iostream>
#include "KTree.h"


int main() {

	std::cout << "in main\n";

	TreeNode* testNode = new TreeNode();
	testNode->setX(7);

	std::cout << "getting testnode x " << testNode->getX()  << "\n";


	int k = 5;

	KTree* tree = new KTree(k);

	TreeNode* root = new TreeNode();

	tree->insert(5,2,root,NULL);
	tree->insert(2,2,root,NULL);
	tree->insert(7,2,root,NULL);
	tree->insert(6,2,root,NULL);
	tree->insert(27,2,root,NULL);
	tree->insert(1,2,root,NULL);
	tree->insert(77,2,root,NULL);
	tree->insert(87,2,root,NULL);
	tree->insert(187,2,root,NULL);
	tree->insert(160,2,root,NULL);
/*
	tree->refreshBPQ();
	tree->KNN(78,2, root,1);
	BPQ* thisbpq78 = tree->getBPQ();
	BPQPoint* point78 = thisbpq78->max_priority_elem();
	std::cout << "nearest from bpq is (77) " << point78->getX() << "\n";

	tree->refreshBPQ();
	tree->KNN(72,2, root,1);
	BPQ* thisbpq72 = tree->getBPQ();
	BPQPoint* point72 = thisbpq72->max_priority_elem();
	std::cout << "nearest from bpq is (77) " << point72->getX() << "\n";

	
	tree->refreshBPQ();
	tree->KNN(28,2, root,1);
	BPQ* thisbpq28 = tree->getBPQ();
	BPQPoint* point28 = thisbpq28->max_priority_elem();
	std::cout << "nearest from bpq is (27) " << point28->getX() << "\n";

	tree->refreshBPQ();
	tree->KNN(26,2, root,1);
	BPQ* thisbpq26 = tree->getBPQ();
	BPQPoint* point26 = thisbpq26->max_priority_elem();
	std::cout << "nearest from bpq is (27) " << point26->getX() << "\n";

*/
	tree->refreshBPQ();
	tree->KNN(14,2, root,1);
	BPQ* thisbpq14 = tree->getBPQ();
	BPQPoint* point14 = thisbpq14->max_priority_elem();
	std::cout << "nearest from bpq is (27) " << point14->getX() << "\n";
}

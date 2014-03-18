#include <stdio.h>

#define k 5
#define kdround(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))


struct BPQPoint {
	int x;
	int y;
	int p;
};

struct BPQ {
	int size;
	struct BPQPoint* values[k];	
};

struct node {
	struct node* left;
	struct node* right;
	struct node* parent;
	int x;
	int y;
	int sy; // 0 is x 1 is y
};

struct BPQ bpq;

struct BPQPoint* max_priority_elem_BPQ() {
	
		int min = 9999999;
		int pos = 0;

		int i;
		
		for(i=0; i<bpq.size;i++ ){
		
		printf("max p\n");
			int cp = bpq.values[i]->p;
			if(cp < min) {
				min = cp;
				pos = i;	
			}	
		}

		return bpq.values[pos];

}

struct BPQPoint* min_priority_elem_BPQ() {
	
		int min = -99999;
		int pos = 0;

		int i;
		
		for(i=0; i<bpq.size;i++ ){
			int cp = bpq.values[i]->p;
			if(cp > min) {
				min = cp;
				pos = i;	
			}	
		}


		return bpq.values[pos];

}

void enqueue_BPQ(struct BPQPoint* point) {
	if(bpq.size < k) {
		bpq.size++;
		bpq.values[bpq.size-1]=point;
	} else {

		printf("overflowing queue - adjust priority\n");
		//remove highest value n as it represents the lowest priority n
		int max = 0;
		int pos = -1;

		int i;
		
		for(i=0; i<bpq.size;i++){
			int cp = bpq.values[i]->p;
			if(cp > max) {
				max = cp;
				pos = i;	
			}	
		}

		// now pos is the minimum
		printf("max is %d at pos %d\n", max, pos);

		if(point->p < max && pos > -1) {
			printf("about to free pos %d", pos);
			free(bpq.values[pos]);
			bpq.values[pos] = point;
		}
	}
}

int is_full_BPQ() {
	return bpq.size == k;
}

void destroy_BPQ() {
	
	int i;
	
	for(i=0; i<bpq.size; i++) {
		free(bpq.values[i]);
	}

	bpq.size = 0;

}

void destroy_tree() {

}

void insert(int x, int y, struct node **leaf, struct node **parent)
{
	if( *leaf == 0 )
	{
		*leaf = (struct node*) malloc( sizeof( struct node ) );
		(*leaf)->x = x;
		(*leaf)->y = y;
		(*leaf)->left = 0;    
		(*leaf)->right = 0;  
		(*leaf)->parent = parent;  
	}
	else {
		if (0) { 
			if(y < (*leaf)->y){
				insert(x,y, &(*leaf)->left, leaf);
			}
			else if(y > (*leaf)->y){
				insert(x,y, &(*leaf)->right, leaf);
			}
		} else {
			if(x < (*leaf)->x){
				insert(x,y, &(*leaf)->left, leaf);
			}
			else if(x > (*leaf)->x){
				insert(x,y, &(*leaf)->right, leaf );
			}
		}
	}
}

void output_nn(int x, int y, struct node **leaf) {

}

struct node** currentGuess;
struct node** currentBest = NULL;
double currentBestDist = 999999;

struct node* KNN(int x, int y, struct node** leaf, int forward) {

	printf("finding nearest neighbour to x %d y %d \n", x, y);

	if(*leaf== 0) {
		// if I am here then the parent node has sub tree on this side of the tree
		// at the moment this is th best guess
		if(!currentBest) 
			currentBest = currentGuess;

		int currentx = (*currentGuess)->x;
		int currenty = (*currentGuess)->y;
		
		int diffx = x - currentx;
		int diffy = y - currenty;

		float r = 0.0;
		r = sqrt((diffx * diffx) + (diffy * diffy)); // distance from currentGuess to target

		// this is our current best guess
		currentBestDist = r;
		currentBest = currentGuess;

		struct node** parent = (*currentGuess)->parent; 	
	        	
		// is there a possibility that the best node is on the other side of the hyperplane
		float dist = 0.0;

		// get the nearest point on the hyperplane
		dist = (*parent)->sy == 1 ? abs((*parent)->y - y) : abs((*parent)->x - x);  

		printf("current x %d\n", (*currentGuess)->x);
		printf("current y %d\n", (*currentGuess)->y);
		printf("parent x %d\n", (*parent)->x);
		printf("parent y %d\n", (*parent)->y);

		printf("r %f, dist %f\n", r, dist);

		// if the radius around the piont touches the plane then need to search other side
		if(dist <= r) {
			// then need to search the of parent
			// on the opposite side - finish this
			printf("nearest neighbour might be on other side\n");

			// I was searching right so search left
			// if the node is null then it is going to keep coming back in
		
			if(forward == 0) {
    			    KNN(x, y, &(*parent)->right, 1);
			} else {
			    KNN(x, y, &(*parent)->left, 0);
			}
		} else {
			// no - the other node is not on the other side of the hyperplane
			// the current guess must be the best node
			currentBestDist = r;
			currentBest = currentGuess;
		}

		printf("the currentbest is x %d y %y\n", (*currentBest)->x, (*currentBest)->y);

	} else {	
		// traverse the tree to find leaf node alternating dimensions
		if (0) { 
			if(y < (*leaf)->y){
				currentGuess = leaf;
				KNN(x,y, &(*leaf)->left,0);
			}
			else if(y > (*leaf)->y){
				currentGuess = leaf;
				KNN(x,y, &(*leaf)->right,1);
			}
		} else {
			if(x < (*leaf)->x){
				currentGuess = leaf;
				KNN(x,y, &(*leaf)->left,0);
			}
			else if(x > (*leaf)->x){
				currentGuess = leaf;
				KNN(x,y, &(*leaf)->right,1);
			}
		}

	}



}



struct node* KNN2(int x, int y, struct node** leaf, int forward) {

	printf("finding nearest neighbour to x %d y %d \n", x, y);

	if(*leaf== 0) 
		return;

	//if distance(curr, guess) < bestDist
 	//	bestDist = distance(curr, guess)
  	//	guess = curr

	printf("calculating distances \n");	

	int currentx = (*leaf)->x;
	int currenty = (*leaf)->y;
		
	int diffx = x - currentx;
	int diffy = y - currenty;

	float r = 0.0;
	r = sqrt((diffx * diffx) + (diffy * diffy)); // distance from currentGuess to target

	struct node** parent = (*leaf)->parent; 	
	        	

	// for k > n need to keep a priority queue of NNs 
	//if(r < currentBestDist) {
	//	printf("setting best \n");
	//	currentBestDist = r;
	//	currentBest = leaf;
	//}

	struct BPQPoint* bpqp = (struct BPQPoint*) malloc(sizeof(struct BPQPoint)); 
	bpqp->x = currentx;
	bpqp->y = currenty;
	bpqp->p = kdround(r);

	enqueue_BPQ(bpqp);


	/* Recursively search the half of the tree that contains the test point. */
	//if ai < curri
	//	 recursively search the left subtree on the next axis
	//else
	//	 recursively search the right subtree on the next axis


	if (0) { 
		if(y < (*leaf)->y){
			KNN2(x,y, &(*leaf)->left,0);
		}
		else if(y > (*leaf)->y){
			KNN2(x,y, &(*leaf)->right,1);
		}
	} else {
		if(x < (*leaf)->x){
			KNN2(x,y, &(*leaf)->left,0);
		}
		else if(x > (*leaf)->x){
			KNN2(x,y, &(*leaf)->right,1);
		}
	}

	

		printf("checking hypersphere  \n");

	//printf("parent is %d\n",parent);

	if(parent==NULL) {
		printf("parent is null\n");
	} else {	
		/* If the candidate hypersphere crosses this splitting plane, look on the
 		*  * other side of the plane by examining the other subtree.
 		*   */
		// if |curri – ai| < bestDist
		//	  recursively search the other subtree on the next axis


		// is there a possibility that the best node is on the other side of the hyperplane
		float dist = 0.0;

		// get the nearest point on the hyperplane
		dist = (*parent)->sy == 1 ? abs((*parent)->y - y) : abs((*parent)->x - x);  

		printf("checking hypersphere  \n");
		printf("current x %d\n", (*leaf)->x);
		printf("checking hypersphere  \n");
		printf("current y %d\n", (*leaf)->y);
		printf("parent x %d\n", (*parent)->x);
		printf("parent y %d\n", (*parent)->y);

		printf("r %f, dist %f\n", r, dist);

		// if the radius around the piont touches the plane then need to search other side
		//if(dist <= currentBestDist) {
		//
		struct BPQPoint* lpp = min_priority_elem_BPQ(); // this is max distance so increases radius for k search
		
		printf("lppp %d\n",lpp->p);

		int lpp_dist = lpp->p;

		if(dist <= lpp_dist || (!is_full_BPQ())) {
	
			// then need to search the of parent
			// on the opposite side - finish this
			printf("nearest neighbour might be on other side\n");

			// I was searching right so search left
			// if the node is null then it is going to keep coming back in
		
			if(forward == 0) {
    			    KNN2(x, y, &(*parent)->right, 1);
			} else {
			    KNN2(x, y, &(*parent)->left, 0);
			}
		}
	}
}


void testAddPoints() {
	
	int i;

	for(i=100;i<(100+k);i++) {
		struct BPQPoint* p1 = (struct BPQPoint*) malloc(sizeof(struct BPQPoint*));
		p1->x = i;
		p1->y = 0;
		p1->p = i;
		enqueue_BPQ(p1);

	}

	struct BPQPoint* pout = max_priority_elem_BPQ();
	
	printf("highest priority elem should be nearest (100)  %d\n", pout->p);

	printf("now start overflowing the queue\n");

	// start at 7

	for(i=7;i<20;i++) {
		struct BPQPoint* p1 = (struct BPQPoint*) malloc(sizeof(struct BPQPoint*));
		p1->x = i;
		p1->y = 0;
		p1->p = i;
		enqueue_BPQ(p1);

	}

	struct BPQPoint* overflow_test_out_node = max_priority_elem_BPQ();
	
	printf("highest priority elem after overflowing (should be 7) %d\n", overflow_test_out_node->p);

	printf("now start overflowing the queue again\n");

	// start at 700

	for(i=2;i<800;i++) {
		struct BPQPoint* p1 = (struct BPQPoint*) malloc(sizeof(struct BPQPoint*));
		p1->x = i;
		p1->y = 0;
		p1->p = i;
		enqueue_BPQ(p1);

	}

	struct BPQPoint* overflow_test_out_node_major = max_priority_elem_BPQ();
	
	printf("highest priority ranking elem after overflowing (should be 2) %d\n", overflow_test_out_node_major->p);

	struct BPQPoint* overflow_test_out_node_minor = min_priority_elem_BPQ();

	printf("lowest priority ranking elem after overflowing (should be 7) %d\n", overflow_test_out_node_minor->p);

	destroy_BPQ();
}



int main() {

	struct node* root = 0;
	currentBest = 0;

	insert(5,2,&root, NULL); // should init
	insert(2,2,&root, NULL); // should left
	insert(7,2,&root, NULL); // should right
	insert(6,2,&root, NULL); // should right
	insert(27,2,&root, NULL); // should right
	insert(1,2,&root, NULL);
	insert(77,2,&root, NULL); // should right

	struct node* n;

	//n = KNN2(23,2,&root,1);
	//printf("The nearest neighbour to 23, 2 should be 27 it is %d\n", (*currentBest)->x );

	//n = KNN2(28,2,&root,1);
	//printf("The nearest neighbour to 28, 2 should be 27 it is %d\n", (*currentBest)->x );

	//n = KNN2(78,2,&root,1);
	//printf("The nearest neighbour to 78, 2 should be 77 it is %d\n", (*currentBest)->x );

	//n = KNN2(76,2,&root,1);
	//printf("The nearest neighbour to 76, 2 should be 77 it is %d\n", (*currentBest)->x );


	n = KNN2(78,2,&root,1);
	struct BPQPoint* max_priority_elem_78 = max_priority_elem_BPQ();
	printf("The nearest neighbour to 78, 2 should be 77 it is %d\n", max_priority_elem_78->x);
	destroy_BPQ();

	n = KNN2(26,2,&root,1);
	struct BPQPoint* max_priority_elem_26 = max_priority_elem_BPQ();
	printf("The nearest neighbour to 26, 2 should be 27 it is %d\n", max_priority_elem_26->x);
	destroy_BPQ();


	n = KNN2(28,2,&root,1);
	struct BPQPoint* max_priority_elem_28 = max_priority_elem_BPQ();
	printf("The nearest neighbour to 28, 2 should be 27 it is %d\n", max_priority_elem_28->x);
	destroy_BPQ();
	
	//output_tree(tree);
	
	//testAddPoints();


	return;	
}

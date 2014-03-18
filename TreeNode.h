#ifndef TREENODE_H
#define TREENODE_H


class TreeNode {
	private:
		TreeNode* _left;
		TreeNode* _right;
		TreeNode* _parent;
		int _x;
		int _y;
		int _sy;
	public:
		TreeNode* getLeft() {return _left;}
		TreeNode* getRight() {return _right;}
		TreeNode*  getParent() {return _parent;}
		
		void setLeft(TreeNode* left) {_left =left;} 
		void setRight(TreeNode* right) {_right =right;} 
		void setParent(TreeNode*  parent) {_parent =parent;} 

		int getX() {return _x;}
		int getY() {return _y;}
		int getSY() {return _sy;}

		void setX(int x) { _x =x;}
		void setY(int y) { _y =y;}
		void setSY(int sy) { _sy =sy;}
};

#endif

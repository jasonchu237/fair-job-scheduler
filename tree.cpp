#include "common.h"

Tree::Tree()
{
	root = NULL;
	nil = NULL;
	size = 0;
}

void Tree::leftRotate(TNode *x)
{
	if(x-> right != nil){                          
		TNode *y = x->right;                       /*y points to the right of x, y then becomes parent of x*/
		x->right=y->left;                          /*Left of y connects to right of x */
		if(y->left != nil)                         /*Disregard if node j is nil*/
			y->left->parent=x;                     /*Change the original parent of y of left to x*/
		y->parent=x->parent;                       /*Update parent of y*/
		if(x->parent == nil)                       /*If original x is root, y will become new root after rotate*/
			root = y;
		else
		{
			if(x == x->parent->left)               /*If original x is the left of its parent*/
				x->parent->left=y;                 /*y will become the left of parent of x after update*/
			else
				x->parent->right=y;                /*If original x is the right of its parent*/
		}                                          /*y will become the right of parent of x after update*/
		y->left = x;                               /*Lastly change y and x*/
		x->parent = y;
	}
}

void Tree::rightRotate(TNode *x)                   /*Same strategy for the right rotate, just swap the left and right*/
{
	if( x->left != nil )
	{
		TNode *y = x->left;
		x->left=y->right;
		if(y->right != nil)
			y->right->parent=x;
		y->parent = x->parent;
		if( x->parent == nil)
			root=y;
		else
		{
			if(x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
		}
		y->right=x;
		x->parent=y;
	}
}

void Tree::insertFixup(TNode *z)
{                                                           /*z: current, y: uncle*/
	TNode *y;                                               /*Case 0: not entering the loop if parent is black*/
	while(z->parent->color == RED)                          /*Enter the loop if parent is red*/
	{                                                       /*Top part: if parent is the left of grandparent*/
		if(z->parent->parent->left == z->parent)  
		{
			y=z->parent->parent->right;        
			if( y->color == RED )                           /*Case 1: if uncle is red*/   
			{                         
				y->color=BLACK;
				z->parent->color=BLACK;
				z->parent->parent->color=RED;               /*Change the grandparent to red*/
				z=z->parent->parent;
			}
			else                                            /*Case 2 and 3: if uncle is black*/
			{
				if( z == z->parent->right )                 /*Case 2*/
				{
					z = z->parent;
					leftRotate(z);
				}                                           /*Case 3*/
				z->parent->color=BLACK;                     /*Color the parent to black, and grandparent to red*/
				z->parent->parent->color=RED;    
				rightRotate(z->parent->parent);
			}
		}
		else                                                /*Bottom part: if parent is the right of grandparent*/ 
		{
			y=z->parent->parent->left;
			if(y->color == RED)                             /*Case 1: if uncle is red*/
			{
				z->parent->color=BLACK;
				y->color=BLACK;
				z->parent->parent->color=RED;               /*Change the grandparent to red*/
				z=z->parent->parent;
			}
			else                                            /*Case 2 and 3: if uncle is black*/
			{
				if( z == z->parent->left )                  /*Case 2*/
				{
					z=z->parent;
					rightRotate(z);
				}
				z->parent->color=BLACK;                     /*Case 3*/
				z->parent->parent->color=RED;
				leftRotate(z->parent->parent);
			}
		}
	}
	root->color=BLACK;                                      /*Make sure the root is black*/
}

TNode * Tree::insert(int jobID, int totalTime)   
{
	TNode *ret;
	if(root == NULL)                                        /*If original tree is empty*/
	{
		nil = new TNode();
		root = new TNode(jobID, totalTime,BLACK, nil, nil, nil);  /*Root must be black*/
		ret = root;
	}
	else       
	{
		TNode *x=root;       
		TNode *p=nil;
		while(x != nil)                                     /*Use nil to distiguish*/
		{
			p=x;
			if(jobID < x->jobID ) x=x->left;
			else if(jobID > x->jobID) x=x->right;
			else return x;
		}
		x = new TNode(jobID, totalTime, RED, nil, nil, p);  /*Set the new node original red*/
		if(jobID < p->jobID)                                /*Put in the left or right decision*/
			p->left = x;
		else 
			p->right = x;
		ret = x;
		insertFixup(x);                                     /*Fixup is needed in case 2 seqentially connected two red nodes*/
	}
	size++;
	return ret;
}

void Tree::removeFixup(TNode *x, TNode *parent)             /*x is current; w is sibling */
{
	while((x==nil || x->color == BLACK) && x!=root)         /*Case 0:      1.current is red, color it to black  */
	{                                                       /*             2.current is black, color it to black*/
		if( x == parent->left )   
		{
			TNode *w=parent->right;
			if( w->color == RED )                           /*Case 1: if siblig is red*/
			{                                               
				w->color=BLACK;                             
				parent->color=RED;
				leftRotate(parent);
				w=parent->right;
			}
			if((w->left==nil ||  w->left->color == BLACK) && /*           Entering to Case 2,3,4: sibling is black         */ 
				(w->right==nil || w->right->color == BLACK)) /*           <----- Case 2: both children of sibling are black*/		  
			{
				w->color=RED;
				x = parent;
				parent = x->parent;
			}
			else                                                 /*Case 3 and 4: one of the child of current is black*/
			{
				if((w->right==nil) || w->right->color == BLACK ) /*Case 3: if right of sibling is black*/      
				{
					w->color=RED;
					w->left->color=BLACK;
					rightRotate(w);
					w=parent->right;
				}                                                /*After Case 3 adjustment, it will become Case 4*/
				w->color = parent->color;                        /*Case 4: right child of sibling is red*/
				parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(parent);
				x = root;                                        /*x become root and jump out the loop*/
				break;
			}
		}
		else      
		{
			TNode *w = parent->left;
			if( w->color == RED )                                /*Case 1: if the color of sibling is red*/
			{
				w->color=BLACK;
				parent->color=RED;
				rightRotate(parent);
				w = parent->left;
			}
			if(((w->left==nil) || w->left->color == BLACK) &&    /*             Entering Case 2,3,4: sibling is black*/
				((w->right==nil) || w->right->color == BLACK))   /*   <-------- Case 2: both children of sibling are black*/
			{
				w->color=RED;
				x = parent;
				parent = x->parent;
			}
			else                                                 /*Case 3 and 4: just one child of current is black*/
			{
				if((w->left==nil)|| (w->left->color == BLACK))   /*Case 3: left child of sibling is black*/
			    {
					w->color=RED;
					w->right->color=BLACK;
					leftRotate(w);
					w=parent->left;
				}                                                /*After Case 3 adjustment, it will become Case 4*/
				w->color=parent->color;                          /*Case 4: left child of sibling is red*/
				parent->color=BLACK;
				w->left->color=BLACK;
				rightRotate(parent);		
				x=root;                                          /*x become root and jump out the loop*/
				break;
			}
		}
	}
	if(x)
		x->color=BLACK;
}

void Tree::remove(TNode *node)                                  /*Try to make the delete node have one child at most*/
{
	TNode *child, *parent;
	int color;
	
	if((node->left!=nil) && (node->right != nil))               /*When both children of delete node exist*/
	{
		TNode *replace = node;                                  /*Use the replace node to simply the delete process*/
		replace = replace->right;                               /*Keep searching the existance of the smallest of the right subtree of delete node, the to swap with delete node*/
		while(replace ->left != nil)                            /*If no left subtree of replace, it means the replace is already now the smallest*/
			replace = replace->left;
		if(node->parent != nil)	                                /*Check if it is original the root*/
		{
			if(node->parent->left == node)
				node->parent->left=replace;
			else
				node->parent->right = replace;
		}	
		else
			root = replace;
		child = replace->right;                                 /*Will not have left if it is the smallest in the right subtree, so only one child on right*/
		parent = replace->parent;
		color = replace->color;
		if(parent == node)                                      /*One step to find the smallest*/
			parent = replace;
		else                                                    /*The smallest may not be just one level below the delete node*/
		{
			if(child!=nil)                                      /*If node expected to swap has right child, need to change its pointer to new parent*/
				child->parent = parent;
			parent->left = child;
			replace->right = node->right;                       /*Swap*/
			node->right->parent = replace;
		}
		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;
		
		if(color == BLACK)                                      /*Need to do removeFixup if the color of delete node is black*/
			removeFixup(child,parent);
//		delete node;
		node = NULL;
		return;
	}

	if(node->left != nil)                                       /*When left of delete node exist*/
		child = node->left;
	else                                                        /*When right of delete node exist*/
		child = node->right;
	parent = node->parent;
	color = node->color;
	if(child!=nil)
		child->parent = parent;
	
	if(parent!=nil)	
	{
		if(parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root = child;
	if(color == BLACK)
		removeFixup(child,parent);
//	delete node;
	node = NULL;
}

TNode* Tree::search(TNode *r, int jobID)
{
	if(r == NULL || r == nil || r->jobID == jobID)            /*If found it, or not found it*/
		return r;
	if(jobID < r->jobID)                                      
		return search(r->left, jobID);                        /*Search for leftward*/
	else
		return search(r->right,jobID);                        /*Search for rightward*/
}

TNode* Tree::search(int jobID)
{
	return search(root, jobID);
}

void Tree::remove(int jobID)
{
	TNode *temp = search(jobID);
	if(temp == nil)                                          /*Make sure if the delete node is in the tree or not*/
		return;
	remove(temp);
	size--;
	if(size == 0)
		root = NULL;
}


TNode* Tree::successor(int jobID)                         /*Find the minimun of the right subtree*/
{
	TNode *tnode = search(jobID);                           
	if(tnode == nil || tnode == NULL)
		return NULL;
	if(tnode->right != nil)
		return minimum(tnode->right);
	TNode *y = tnode->parent;
	while(y != nil && tnode == y->right)
	{
		tnode = y;	
		y = y->parent;
	}
	return y;
}

TNode* Tree::predecessor(int jobID)                        /*Find the maximun of the left subtree*/
{
	TNode *tnode = search(jobID);
	if(tnode == nil || tnode == NULL)
		return NULL;
	if(tnode->left != nil)
		return maxmum(tnode->left);
	TNode *y = tnode->parent;
	while(y!=nil && tnode == y->parent)
	{
		tnode = y;
		y = y->parent;
	}
	return y;
}

TNode* Tree::minimum(TNode *x)                     /*Use for finding the minimum*/
{
	TNode *p = x;
	while(p->left != nil)
		p = p->left;
	return p;
}

TNode * Tree::maxmum(TNode *x)                     /*Use for finding the maximum*/
{
	TNode *p = x;
	while(p -> right != nil)
		p = p->right;
	return p;
}

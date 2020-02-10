#include "common.h"

HNode* Heap::insert(int execTime,TNode *l)                         /*Inseart new node into min-heap*/
{                                                                  /*h = HNode type of vector*/
	HNode *newnode = new HNode(execTime, l);
	h.push_back(newnode);
	int i = h.size()-1;
	while((i-1) >= 0 && h[(i-1)/2]->execTime > h[i]->execTime)     /*If child is i, its parent(father) = (i-1)/2*/
	{                                                              /*Adjust to maintain up level node is smaller than level below*/
		h[i] = h[(i-1)/2];
		h[(i-1)/2] = newnode;
		i = (i-1)/2;
	}
	return newnode;	
}

HNode *Heap::getMin()                                              /*Min locate at root*/
{
	if(h.size() > 0)
		return h.at(0);
	return NULL;
}

void Heap::deleteMin()
{
	if(h.size() == 0)
		return;
	h[0] = h[h.size()-1];                                         /*Place the last node to the root after deleting previous root*/
	h.pop_back();
	if(h.size() <= 0)
		return;
	siftdown(0);
}

void Heap::siftdown(int k)
{
	unsigned father = k;
	unsigned child = 2*k+1;
	HNode *temp = h[k];
	while(child < h.size())
	{
		if(child < h.size()-1 && h[child]->execTime > h[child+1]->execTime)  /*Compare which child is much smaller*/
			child++;
		if(temp->execTime <= h[child]->execTime)                             /*Break, if father is smaller than the child*/
			break;
		h[father] = h[child];
		father = child;
		child = 2*father + 1;
	}
	h[father] = temp;
}

void Heap::siftDown()
{
	siftdown(0);
}

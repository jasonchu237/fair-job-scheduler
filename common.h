#ifndef __COMMON_H__
#define __COMMON_H__

#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;


enum Color{RED,BLACK};

struct HNode;

struct TNode
{
	int jobID;				/*key*/
	int totalTime;
	HNode *link;			/*The link to the heap*/

	int color;				/*RED or BLACK*/
	TNode *left, *right, *parent;

	TNode(){color = BLACK;}
	TNode(int id, int time, int c, TNode *l, TNode *r, TNode *p)
	:jobID(id),totalTime(time), link(NULL), color(c),
	 left(l), right(r), parent(p){}
};

struct Tree
{
private:
	TNode *root;
	int size;
public:
	TNode *nil;
	Tree();
	TNode *insert(int jobID, int totalTime);
	TNode *search(int jobID);
	TNode *successor(int jobID);
	TNode *predecessor(int jobID);
	void remove(int jobID);
private:
	TNode *minimum(TNode *r);
	TNode *maxmum(TNode *r);
	void leftRotate(TNode *x);
	void rightRotate(TNode *x);	
	void insertFixup(TNode *node);
	void remove(TNode *node);
	void removeFixup(TNode *node, TNode *parent);
	TNode *search(TNode *r, int jobID);
};

struct HNode
{
	int execTime;
	TNode *link;
	HNode(int time, TNode *l)
	{
		execTime = time;
		link = l;
	}
};

struct Heap
{
private:
	int size;
	vector<HNode *> h;
public:
	Heap():size(0){}
	HNode* insert(int execTime, TNode *l);
	HNode* getMin();
	void deleteMin();
	void siftDown();
	bool empty() {return h.empty();}
private:
	void siftup(int start);
	void siftdown(int start);
};

/*equivalent to a true instruction in computer system 
  time_appear ==> address
  command ==> instruction
  Red Black Tree and Min-Heap are memories.*/
struct Command
{
	int time_appear;
	string command;
};

struct Scheduler
{
private:
	vector<Command> commands; 		/*code segment ...*/
	unsigned eip;					/*next command to execute(INSTRUCTION POINTER)*/
	int sys_time;					/*system time count*/
	HNode *current;					/*current executed job*/
	bool busy;						/*system state*/
	int left_time;					/*time slice*/
public:
	Scheduler(char *filename);
	void run();						/*start our system*/
private:
	void nextCommand();
};

#endif

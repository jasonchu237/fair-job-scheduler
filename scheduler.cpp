#include "common.h"
#include <string.h>
#include <limits.h>

extern Tree tree;
extern Heap heap;
extern ofstream out;

void Insert(string operand)                                                    /*Insert into tree and heap*/
{                                                                              /*Get the ID and total time*/ 
	int comma = operand.find(",");
	int jobID = stoi(operand.substr(0, comma));
	int totalTime = stoi(operand.substr(comma+1, operand.length()-comma));
	TNode *tnode = tree.insert(jobID, totalTime);
	HNode *hnode = heap.insert(0, tnode);
	tnode->link = hnode;                                                       /*link between heap and tree*/
}

void PrintJob(string operand)                                                  /*Print JobID and range JobID*/
{
	int comma = operand.find(",");
	int low, high;
	if(comma == -1)                                                            /*For one key*/
		low = high = stoi(operand);
	else                                                                       /*For keys in a range, set the high/low edges*/
	{
		low = stoi(operand.substr(0, comma));
		high = stoi(operand.substr(comma+1, operand.length()-comma));
	}
	struct triple{
		int jobID,execTime,totalTime;
		triple(int a1,int b1,int c1):jobID(a1),execTime(b1),totalTime(c1){}
	};
	vector<triple> ans;                                                        /*Create a vector to store the found triplet for later print out*/
	for(int i = low; i <= high; i++)
	{
		TNode *tnode = tree.search(i);
		// if not found, the search will return nil
		if(tnode != tree.nil)	
		{
			HNode *hnode = tnode->link;
			ans.push_back(triple(tnode->jobID, hnode->execTime, tnode->totalTime));
		}
	}
	if(ans.size()==0)                                                          /*Empty print (0,0,0)*/
		out << "(0,0,0)";
	else                                                                       /*Print out the triplets*/
	{
		for(unsigned  i = 0; i < ans.size();i++)
		{
			out << "(" << ans[i].jobID <<","<<ans[i].execTime<<","<<ans[i].totalTime << ")";
			if(i!=ans.size()-1)
				out << ",";
		}
	}
	out << endl;
}

void NextJob(string operand)                                                   /*Print out the sccessor*/
{
	int jobID = stoi(operand);
	TNode *tnode = tree.successor(jobID);
	if(tnode == tree.nil || tnode == NULL)
		out << "(0,0,0)" << endl;
	else
	{
		HNode *hnode = tnode->link;
		out << "(" << tnode->jobID << "," << hnode->execTime << "," << tnode->totalTime << ")" << endl;
	}
}

void PreviousJob(string operand)                                               /*Print out the predecessor*/
{
	int jobID = stoi(operand);
	TNode *tnode = tree.predecessor(jobID);
	if(tnode == tree.nil || tnode == NULL)
		out << "(0,0,0)"<<endl;
	else
	{
		HNode *hnode = tnode->link;
		out << "(" << tnode->jobID << "," << hnode->execTime << "," << tnode->totalTime << ")" << endl;
	}
}

/*             
This array is similar to Intel's Instruction set. On the one hand it specifies what 
instructions our system can execute, Insert, PrintJob, NextJob...those operation are similar 
to mov add sub...True computer system use opcode to mark each operation, here we use it's 
name could be ok. On the other hand, it point out how to execute each instructions:function 
pointer, each instruction has an associated function handler.
 */
struct{
	string op;
	void (*function)(string operand);
}OpTable[4] = {
	{"Insert", Insert},
	{"PrintJob", PrintJob},
	{"NextJob", NextJob},
	{"PreviousJob", PreviousJob}
};


/*
When creating a Scheduler object from main function, this constructed function will be 
called. it will do some initialization:
1.read all the commands from file, after our system was started, read file every time
  will cost much time
2.initialize system state:
  a. busy flag: if there are any job is executed now.
  b. eip: this is similar to the true instruction pointer register in computer system, 
	it pointa to the next command's index in the array
*/
Scheduler::Scheduler(char *filename)
{
	ifstream in(filename);
	if(!in)                                                                   /*Unsuccessful reading a file*/
	{
		exit(EXIT_FAILURE);
	}
	string str;
	while(getline(in, str))
	{                                                                         /*Use vector <command> to store a set of command  with*/
		int index = str.find(":");                                            /*each has time_appear and conmand instruction.       */
		Command command = {                                                   /*command, code segment ...                           */
			.time_appear = atoi(str.substr(0, index).c_str()),                /*Sample input format:  90: Insert(30,300),           */ 
			.command = string(str.substr(index+2, str.length()-(index+2)))    /*                     200: PrintJob(19)              */
		};
		commands.push_back(command);
	}
	busy = false;
	eip = 0;
	sys_time = 0;
}

void Scheduler::run()
{
	for(; sys_time < INT_MAX; sys_time++)
	{
		if(busy == true)		                                             /*This means there is one job is executed, we should update*/
		{						                                             /*it's execute time, if execute time = total time, this job*/
			current->execTime++;                                             /*is completed, remove it from both data structures.       */
			left_time--;
			if(current->execTime >= current->link->totalTime)
			{
				tree.remove(current->link->jobID);
				heap.deleteMin();
				busy = false;
			}
			else if(left_time == 0)	                                         /*The time slice is out of use, we need to request our system */
			{						                                         /*for schedule. So set the busy flag, mean our system is free.*/
				heap.siftDown();	                                         /*The current job is not completed, but it's key in min-heap  */
				busy = false;		                                         /*has changed, we need to update the heap structure.          */
			}
		}
		nextCommand();				                                         /*Check whether there is a command's appear time equal to global   */
		if(busy == false)			                                         /*time. If any, execute it.                                        */
		{							
									                                         /*If no command left and the heap is empty, all jobs has been done,*/
									                                         /*return to the main function	                                    */
			if(heap.empty() && eip >= commands.size())
			{
				out.close();
				break;
			}						                                         /*If heap is not empty, then we find a job with smallest execute time*/
			if(!heap.empty())		                                         /*And set busy flag                                                */
			{
				current = heap.getMin();
				busy = true;                                                 /*Assign left time depends on two conditions                       */
				left_time = ((current->link->totalTime - current->execTime)<=5)?(current->link->totalTime-current->execTime):5;
			}
		}
	}		
}

void Scheduler::nextCommand()
{
	if(eip >= commands.size())
		return;
	if(sys_time == commands.at(eip).time_appear)
	{
		string com = commands.at(eip).command;                         /*com = CommandType(int, int)*/ 
		int lp = com.find("(");
		int rp = com.find(")");
		string op = com.substr(0, lp);		
		for(int i = 0; i < 4; i++)                                     /*To distiquish what command shoud nextCommand execute*/
			if(strcmp(OpTable[i].op.c_str(), op.c_str()) == 0)           
				OpTable[i].function(com.substr(lp+1, rp-lp-1));
		eip++;
	}
}

#include "common.h"

Tree tree = Tree();
Heap heap = Heap();
ofstream out;

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		cerr << "Usage: ./jobscheduler file_name" << endl;
		exit(EXIT_FAILURE);
	}
	out.open("output_file.txt",ios::out);
	Scheduler scheduler = Scheduler(argv[1]);
	scheduler.run();	/*Our operating system is starting from here*/
	return 0;
}


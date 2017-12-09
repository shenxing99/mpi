#include"mpi.h"
#include<iostream>
#include<stdlib.h>
#define MASTER 0
using namespace std;
int main(int argc, char* argv[]){
    int numtasks;
    int taskid;
    int len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Get_processor_name(hostname, &len);
    cout<<"hello from task " <<taskid<<" on "<<hostname<<endl;
    if(taskid==MASTER){
	cout<<"MASTER: The total number of MPI task is "<<numtasks<<endl;

    }
    MPI_Finalize();
    return 0;
}

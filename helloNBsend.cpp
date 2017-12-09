#include"mpi.h"
#include<iostream>
#include<stdlib.h>
#define MASTER 0
using namespace std;
int main(int argc, char* argv[]){
    int numtask, taskid, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int partner, message;
    MPI_Status stats[2];
    MPI_Request reqs[2];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);
    if(numtask%2!=0){
        if(taskid==MASTER){
 	    cout<<"quitting, need an even number of tasks: numtask = "<<numtask<<endl;
	 }
    }
    else{
	MPI_Get_processor_name(hostname, &len);
	cout<<"hello from task "<<taskid<<" on "<<hostname<<endl;
	if(taskid==MASTER){
	    cout<<"MASTER: number of MPI task is "<< numtask<<endl;
	}
	if(taskid<numtask/2){
	    partner = numtask/2+taskid;
	}
	else if(taskid>=numtask/2){
	    partner = taskid - numtask/2;
        }
	
	MPI_Irecv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &reqs[0]);
	MPI_Isend(&taskid, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &reqs[1]);

	MPI_Waitall(2, reqs, stats);

	cout<<"task"<<taskid<<"<--->"<<partner<<endl;
    }
    MPI_Finalize();
    return 0;
}



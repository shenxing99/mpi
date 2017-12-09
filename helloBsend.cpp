#include "mpi.h"
#include<iostream>
#include<stdlib.h>
#define MASTER 0

using namespace std;

int main(int argc, char* argv[]){
    int numtasks;
    int taskid;
    int len;
    int partner;
    int message;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
cout<<"print testing"<<endl;
    if(numtasks%2!=0){
	//is a odd number
	if(taskid==MASTER){
	    cout<<"quiting, neew an even number of task. task number is "<< taskid<<" now"<<endl;  
	}
	MPI_Get_processor_name(hostname,&len);
	cout<<"now is task "<<taskid<< " on "<<hostname<<endl;
	if(taskid<numtasks/2){
	    partner = numtasks/2+taskid;
	    MPI_Send(&taskid, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
	    MPI_Recv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
	}
	else if(taskid>=numtasks/2){
	    partner = taskid-numtasks/2;
	    MPI_Recv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
	    MPI_Send(&taskid, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
	}
	cout<<"taskid"<<taskid<<"<--->"<<"partnerid"<<partner<<endl;
    }
    MPI_Finalize();
    return 0;
}


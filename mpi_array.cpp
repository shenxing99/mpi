#include"mpi.h"
#include<iostream>
#include<stdlib.h>
#define MASTER 0
#define ARRAYSIZE 16000000

using namespace std;

float data[ARRAYSIZE];

int main(int argc, char* argv[]){
    int numtask, taskid, rc, dest, offset, i, j, tag1, tag2, source, chunksize;
    float mySum, sum;
    float update(int myoffset, int chunk, int myid);
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);
    if(numtask%4!=0){
	cout<<"quiting: number of MPI tasks must be divided by 4."<<endl;
	MPI_Abort(MPI_COMM_WORLD,rc);
	exit(0);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    cout<<"MPI task "<<taskid<<" has started."<<endl;
    chunksize = (ARRAYSIZE/numtask);
    tag2 = 1;
    tag1 = 2;
    
    if(taskid==MASTER){
	sum = 0;
	for(i=0; i<ARRAYSIZE; i++){
	    data[i] = i*1.0;
	    sum = sum+data[i];
	}
	cout<<"initialized array sum is "<< sum<<endl;
	offset = chunksize;
	for(dest=1; dest<numtask; dest++){
	    MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
	    MPI_Send(&data[offset], chunksize, MPI_FLOAT, dest, tag2, MPI_COMM_WORLD);
	    cout<<"Sent "<<chunksize<<" elements to task "<< taskid<<" and offset is "<<offset<<endl;
	    offset = offset+chunksize;
	}
	offset = 0;
	mySum = update(offset, chunksize, taskid);
	for(i=1; i<numtask; i++){
	    source = i;
	    MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
	    MPI_Recv(&data[offset], chunksize, MPI_FLOAT, source, tag2, MPI_COMM_WORLD, &status);
	}

	MPI_Reduce(&mySum, &sum, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);
	cout<<"sample results:"<<endl;
	offset = 0;
	for(i=0; i<numtask; i++){
	    for(j=0; j<5; j++){
	        cout<<data[offset+j]<<endl;
		offset = offset+chunksize;
	    }
	    cout<<"final sum is :"<<sum<<endl;
	}
    }

	if(taskid>MASTER){
	    source = MASTER;
	    MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
	    MPI_Recv(&data[offset], chunksize, MPI_FLOAT, source, tag2, MPI_COMM_WORLD, &status);
	    mySum = update(offset, chunksize, taskid);
	    dest = MASTER;
	    MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
	    MPI_Send(&data[offset], chunksize, MPI_FLOAT, MASTER, tag2, MPI_COMM_WORLD);
	    MPI_Reduce(&mySum, &sum, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);
	}
    MPI_Finalize();
}

float update(int myoffset, int chunk, int myid){
    int i;
    float mySum = 0;
    for(i=myoffset; i<myoffset+chunk; i++){
	data[i]+=i*1.0;
	mySum+=data[i];
    }
    cout<<"task is "<<myid<<" mySum is "<<mySum<<endl;
    return(mySum);
}









    

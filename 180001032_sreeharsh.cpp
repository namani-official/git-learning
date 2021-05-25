#include <mpi.h>
#include <stdio.h>
#include<bits/stdc++.h>
#include<time.h>
using namespace std;

int main() {
	int n;	
	int *a;
	int *disp;
	MPI_Init(NULL,NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	if(world_rank==0){
		cout<<"Enter the number of elements you want in each processor\n";
		cin>>n;
		for(int i=1;i<=world_size-1;i++) {
			MPI_Send(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);//section for broadcasting the number of elements in each process
		}
	}
	else{
		MPI_Recv(&n, 1, MPI_INT,0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	a=(int*)malloc(n*sizeof(int));
	srand(time(0)*world_rank);
	for(int p=0;p<=n-1;p++) a[p]=rand()%60;// local array creation in each process
	sort(a,a+n);//sorting locally  populated arrays
	disp=(int*)malloc(n*world_size*(sizeof(int)));
	MPI_Gather(a,n,MPI_INT,disp,n,MPI_INT,0,MPI_COMM_WORLD);// gathering the local arrays in the 0th process
	if(world_rank==0){
		for(int i=0;i<=world_size-1;i++){
			cout<<"Printing from Process  "<<i<<" and partial array  is :- \n ";			
			for(int j=0;j<=n-1;j++) {
				cout<<disp[i*n+j]<<" ";
			}
			cout<<endl;	
		}	
	}	
	MPI_Barrier(MPI_COMM_WORLD);
	int u_l=log2(world_size);
	long long x=2;
	bool flag=true;
	for(int t=0;t<=u_l-1;t++){// Execution of  tree communication architecture
		int tr=n*x;		
		if(world_rank%x!=0&&flag) {
			MPI_Send(a,tr/2,MPI_INT,world_rank-x/2,0,MPI_COMM_WORLD);			
			flag=false;
		}
		else if(world_rank%x==0){
			int* temp=(int*)malloc((tr/2)*sizeof(int)),*temp2=(int*)malloc(tr*sizeof(int));
			MPI_Status status;
			MPI_Recv(temp,tr/2,MPI_INT,world_rank+x/2,0, MPI_COMM_WORLD,&status);
			merge(a,a+tr/2,temp,temp+tr/2,temp2);// stl merge function 
			a=temp2;
		}
		x=x*(long long int)2;
		MPI_Barrier(MPI_COMM_WORLD);		
	}// Ending of tree communiication structure
	MPI_Barrier(MPI_COMM_WORLD);
	if(world_rank==0) {
		cout<<"Your final sorted array is :-  ";		
		for(int p=0;p<=n*world_size-1;p++) cout<<a[p]<<"  ";
		cout<<"\n";
	}// printing global  array
	MPI_Finalize();
}

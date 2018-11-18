#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
   
	MPI_Init(NULL, NULL);
	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int n = size;
	int a[n][n],b[n],x[n];
	int i,j,sum;
	if(rank == 0)
	{
	printf("Enter the values in the matrix:\n");
	}
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				if(rank == 0)
				{
					scanf("%d",&a[i][j]);
					
				}
				MPI_Bcast(&a[i][j], 1, MPI_INT, 0, MPI_COMM_WORLD);
			}
				
		}
	if(rank == 0)
	{
		printf("\nEnter the values of b vector:\n");
	}
		for(i=0;i<n;i++)
		{
			if(rank == 0)
			{
				scanf("%d",&b[i]);
			}
			
			MPI_Bcast(&b[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
		}

	if(rank == 0)
	{
		x[0]=b[0]/a[0][0];
		printf("x[0] = %d\n",x[0]);
		MPI_Send(&x[0],1,MPI_INT,1,0,MPI_COMM_WORLD);
	}
	else if(rank > 0 && rank < (size-1)) 
	{
		sum = 0;
		i=rank ;
		for(j=0;j<i;j++)
		{
			MPI_Recv(&x[j],1,MPI_INT,i-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Send(&x[j],1,MPI_INT,i+1,0,MPI_COMM_WORLD);
			sum = sum + a[i][j]*x[j];	
		}
		x[i] = (b[i] - sum)/(a[i][i]*1);
		printf("x[%d] = %d\n",rank,x[rank]);
		MPI_Send(&x[i],1,MPI_INT,i+1,0,MPI_COMM_WORLD);
	}
	else
    {
	for(j=0;j<rank;j++)
	{
		MPI_Recv(&x[j], 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum = sum + a[rank][j]*x[j];
	}
	x[rank] = (b[rank] - sum)/(a[rank][rank]*1.0);
	printf("x[%d] = %d\n",rank,x[rank]);
    }
	MPI_Finalize();
}

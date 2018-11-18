#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 

struct bucket 
{
    int count;
    int* value;
};
 
int result(const void* a, const void* b)
{
    int x = *((int*)a), y =  *((int*)b);
    if (x == y)
        return 0;
    else if (x < y)
        return -1;
    else
        return 1;
}
 
void bucketSort(int arr[],int n)
{
    struct bucket bkts[3];
    int i, j, k;
    for (i = 0; i < 3; i++)
    {
        bkts[i].count = 0;
        bkts[i].value = (int*)malloc(sizeof(int) * n);
    }    
    for (i = 0; i < n; i++)
    {
        if (arr[i] < 0)
        {
            bkts[0].value[bkts[0].count++] = arr[i];
        }
        else if (arr[i] > 10)
        {
            bkts[2].value[bkts[2].count++] = arr[i];
        }
        else
        {
            bkts[1].value[bkts[1].count++] = arr[i];
        }
    }
    for (k = 0, i = 0; i < 3; i++)
    {
        qsort(bkts[i].value, bkts[i].count, sizeof(int), &result);
        for (j = 0; j < bkts[i].count; j++)
        {
            arr[k + j] = bkts[i].value[j];
        }
        k += bkts[i].count;
        free(bkts[i].value);
    }
}
 
int main(int argc,char *arg[]) {
    int proc,id,no,i=12,j,k,n,c;
    n=i;
    int arr[100]; //= { 5, -34, 10, 1, -42, 123, 2, 395, 5, 4, 1234, 7 };
    MPI_Init(NULL,NULL);
    MPI_Status status; 
    MPI_Comm_size(MPI_COMM_WORLD, &proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
	if(id == 0)
	{
		printf("Enter elements for sorting\n");
	}
	for(c=0;c<n;c++)
		{
			if(id == 0)
			{	
				scanf("%d",&arr[c]);
			}
			
			MPI_Bcast(&arr[c], 1, MPI_INT, 0, MPI_COMM_WORLD);
		}
   	 if(id==0){
	    
	    printf("\n");
	    MPI_Send(&arr,1,MPI_INT,1,1,MPI_COMM_WORLD);
   		 }
   	 else{
		MPI_Recv(&arr,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
    		bucketSort(arr, n); 
    		printf("\n After Sorting\n");
    		for (k = 0; k<i; k++)
        	printf("%d ", arr[k]);   
  		  }
    printf("\n");
    MPI_Finalize();
    return 0;
}

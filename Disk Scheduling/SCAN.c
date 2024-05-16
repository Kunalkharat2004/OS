// C program to implement SCAN Disk Scheduling algorithm

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int comparator(const void * a, const void *b)
{
   int x = *(int *)a;
   int y = *(int *)b;
   if(x < y)
     return -1;  // No sorting
   else if(x >= y) // = is for stable sort
    return 1;    // Sort
} 

int min_element(int request_queue[], int n)
{
    int min = INT_MAX;
    for(int i = 0; i < n; i++)
    {
       if(request_queue[i] < min)
          min = request_queue[i];
    }
    return min;  
}

int max_element(int request_queue[], int n)
{
   int max = INT_MIN;
   for(int i = 0; i < n; i++)
   {
      if(request_queue[i] > max)
          max = request_queue[i];
   }
    return max;  
}

int applySCANAlgo(int total_cylinders, int request_queue[], int initial_pos, int seek_sequence[], int direction, int *sequence_size, int n)
{
    int total_head_movement = 0, j = 0, k = 0;
    int left[n + 1], right[n + 1];  //in worst case(corner cases), size will be n+1

    // appending end values
    // which has to be visited
    // before reversing the direction
    if(direction == 0)
    {
        if(initial_pos > min_element(request_queue, n))
            right[j++] = total_cylinders - 1;
        right[j++] = initial_pos;
    }
    else if(direction == 1)
    {
        if(initial_pos < max_element(request_queue, n))
            left[k++] = 0;  //here  0 is initial cylinder of HDD
        left[k++] = initial_pos;
    }

    for (int i = 0; i < n; i++) {
        if (request_queue[i] < initial_pos)
            left[k++] = request_queue[i];
        if (request_queue[i] > initial_pos)
            right[j++] = request_queue[i];
    }

    // sorting left and right arrays
    qsort((void *)left, k, sizeof(int), comparator);
    qsort((void *)right, j, sizeof(int), comparator);

    // run the while loop two times one by one scanning right and left of the head
    int completed = 2;
    while (completed--)
    {
        if (direction == 0) 
        {
            for (int i = 0; i < j; i++) 
            {
                // calculate absolute distance
                total_head_movement += abs(initial_pos - right[i]);    
                initial_pos = right[i];
                // appending current track to seek sequence
                seek_sequence[*sequence_size] = right[i]; 
                (*sequence_size)++;
            }
            direction = 1;
        }
        else if (direction == 1)
        {
            for (int i = k - 1; i >= 0; i--) 
            {
                // calculate absolute distance
                total_head_movement +=  abs(initial_pos - left[i]);    
                initial_pos = left[i];
                // appending current track to seek sequence
                seek_sequence[*sequence_size] = left[i]; 
                (*sequence_size)++;
            }
            direction = 0;
        }
    }  //end of while
    return total_head_movement;
}  //end of applySCANALgo()

int main()
{
    int total_cylinders = 200;
    int initial_pos = 50;
    int n = 7;
    int direction = 0;
    int request_queue[] = {82, 170, 43, 140, 24, 16, 190};
    
    int total_head_movement = 0;
    int seek_sequence[n + 10];   // i.e take somewhat bigger size than n
    int sequence_size = 0;
    
    total_head_movement = applySCANAlgo(total_cylinders, request_queue, initial_pos, seek_sequence, direction, &sequence_size, n);
    
    // *********** OUTPUT ********** 
    printf("\n\n*********** OUTPUT **********");
    printf("\nSeek Sequence: ");
    for(int i = 0; i < sequence_size; i++)
        printf("%d ", seek_sequence[i]);

    printf("\nTotal No. of Head Movements = %d\n", total_head_movement);
    printf("\nAverage head movements = %.2f\n\n", (float)total_head_movement / n);

    return 0;
}

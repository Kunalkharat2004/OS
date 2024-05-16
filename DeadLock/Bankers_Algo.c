#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESS 5
#define MAX_RESOURCES 3

struct process_info
{
    int max[MAX_RESOURCES];
    int allocated[MAX_RESOURCES];
    int need[MAX_RESOURCES];
};

int no_of_process = MAX_PROCESS;
int no_of_resources = MAX_RESOURCES;

// Take the input 
void input(struct process_info process[MAX_PROCESS], int available[MAX_RESOURCES])
{
    // Hardcoded values for processes' information
    int max[MAX_PROCESS][MAX_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    int allocated[MAX_PROCESS][MAX_RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    // Fill array of Structure
    for (int i = 0; i < no_of_process; i++)
    {
        for (int j = 0; j < no_of_resources; j++)
        {
            process[i].max[j] = max[i][j];
            process[i].allocated[j] = allocated[i][j];
            // Calculate need/future need 
            process[i].need[j] = process[i].max[j] - process[i].allocated[j];
        }
    }

    // Hardcoded values for available resources
    int available_resources[MAX_RESOURCES] = {3, 3, 2};
    for (int i = 0; i < no_of_resources; i++)
    {
        available[i] = available_resources[i];
    }
}

// Print the Info in Tabular Form
void showTheInfo(struct process_info process[MAX_PROCESS])
{
    printf("\nPID\tMaximum\t\tAllocated\tNeed\n");
    for (int i = 0; i < no_of_process; i++)
    {
        printf("P[%d]\t", i);
        for (int j = 0; j < no_of_resources; j++)
            printf("%d ", process[i].max[j]);
        printf("\t\t");
        for (int j = 0; j < no_of_resources; j++)
            printf("%d ", process[i].allocated[j]);
        printf("\t\t");
        for (int j = 0; j < no_of_resources; j++)
            printf("%d ", process[i].need[j]);
        printf("\n");
    }
}

// Apply safety algo
bool applySafetyAlgo(struct process_info process[MAX_PROCESS], int available[MAX_RESOURCES], int safeSequence[MAX_PROCESS])
{
    bool finish[MAX_PROCESS];
    int work[MAX_RESOURCES];
    for (int i = 0; i < no_of_resources; i++)
    {
        work[i] = available[i];
    }
    for (int i = 0; i < no_of_process; i++)
        finish[i] = false;
    bool proceed = true;
    int k = 0;
    while (proceed)
    {
        proceed = false;
        for (int i = 0; i < no_of_process; i++)
        {
            bool flag = true;
            // Find Index i
            if (finish[i] == false)
            {
                for (int j = 0; j < no_of_resources; j++)
                {
                    // if Need <= Work
                    if (process[i].need[j] <= work[j])
                    {
                        continue;
                    }
                    else
                    {
                        flag = false; // implies that the current process need > work
                        break;
                    }
                }
                if (flag == false)
                    continue; // check for next process

                // If we get Index i(or process i), update work  
                for (int j = 0; j < no_of_resources; j++)
                    work[j] = work[j] + process[i].allocated[j];
                finish[i] = true;
                safeSequence[k++] = i;
                proceed = true; // tells that we got at least one process in a safe state, we can proceed
            }
        }
    }

    // check finish array
    int i;
    for (i = 0; i < no_of_process && finish[i] == true; i++)
    {
        continue;
    }
    // If all processes are completed, then return true
    if (i == no_of_process)
        return true;
    else
        return false;
}

// Checks if we State is safe or not
bool isSafeState(struct process_info process[MAX_PROCESS], int available[MAX_RESOURCES], int safeSequence[MAX_PROCESS])
{
    if (applySafetyAlgo(process, available, safeSequence) == true)
        return true;
    return false;
}

int main()
{
    int available[MAX_RESOURCES];
    int safeSequence[MAX_PROCESS];
    // Create Array of Structure to store Processes' Informations
    struct process_info process[MAX_PROCESS];

    printf("***********Enter details of processes*************\n");
    // Take the Input
    input(process, available);

    // Print the Info in Tabular Form 
    showTheInfo(process);

    if (isSafeState(process, available, safeSequence))
    {
        printf("\nSystem is in SAFE State\n");
        printf("Safe Sequence is: ");
        for (int i = 0; i < no_of_process; i++)
            printf("P[%d] ", safeSequence[i]);
        printf("\n");
    }
    else
          printf("System is NOT in SAFE State\n");
    
    return 0;
}


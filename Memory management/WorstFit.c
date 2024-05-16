#include <stdio.h>

void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocate[n]; // Initializing allocate list
    for (int i = 0; i < n; i++) {
        allocate[i] = -1;
    }

    // select the worst memory block that can be allocated to a process
    for (int i = 0; i < n; i++) {
        int worstIndex = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIndex == -1) {
                    worstIndex = j;
                } else if (blockSize[worstIndex] < blockSize[j]) {
                    worstIndex = j;
                }
            }
        }
        if (worstIndex != -1) {
            allocate[i] = worstIndex;
            blockSize[worstIndex] -= processSize[i];
        }
    }

    // Display the processes with the blocks that are allocated to a respective process
    printf("\nFor Worst Fit Algorithm\n");
    printf(" Process No\t Process Size \t Block Number\n");
    for (int i = 0; i < n; i++) {
        printf(" %d\t\t %d\t\t", i + 1, processSize[i]);
        if (allocate[i] != -1) {
            printf("%d\n", allocate[i] + 1);
        } else {
            printf("Not Allocated\n");
        }
    }
}

int main() {
     int blockSize[] = {100, 500, 200, 300, 600};
    int processSize[] = {212, 417, 112, 426};
    int m = 5; // Number of blocks
    int n = 4; // Number of processes

    // Call the function
    worstFit(blockSize, m, processSize, n);

    return 0;
}

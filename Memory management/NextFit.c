#include <stdio.h>

void nextFit(int blockSize[], int m, int processSize[], int n) {
    int allocate[n]; // Initializing allocate list
    for (int i = 0; i < n; i++) {
        allocate[i] = -1;
    }

    int lastIndex = 0; // Initialize the index of the last block allocated

    for (int i = 0; i < n; i++) {
        for (int j = lastIndex; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocate[i] = j;
                blockSize[j] -= processSize[i];
                lastIndex = j; // Update the index of the last block allocated
                break;
            }
        }
        // If no block is found in the remaining blocks, start searching from the beginning
        
            for (int j = 0; j < lastIndex; j++) {
                if (blockSize[j] >= processSize[i]) {
                    allocate[i] = j;
                    blockSize[j] -= processSize[i];
                    lastIndex = j; // Update the index of the last block allocated
                    break;
                }
            }
    }

    // Display the processes with the blocks that are allocated to a respective process
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
    nextFit(blockSize, m, processSize, n);

    return 0;
}

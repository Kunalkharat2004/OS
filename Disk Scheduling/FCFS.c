#include <stdio.h>
#include <stdlib.h>

int main() {
    int total_head_movement = 0;
    int initial_pos = 50; 
    int n = 7; 
    int request_queue[] = {82, 170, 43, 140, 24, 16, 190}; 

    printf("Seek Sequence: %d ", initial_pos);  // Print the initial head position as part of the seek sequence

    for (int i = 0; i < n; i++) {
        total_head_movement += abs(initial_pos - request_queue[i]);
        initial_pos = request_queue[i];
        printf("%d ", initial_pos);  // Print each position as it is processed
    }

    printf("\nTotal No. of Head Movements = %d\n", total_head_movement);
    printf("Average head movements = %.2f\n", (float)total_head_movement / n);

    return 0;
}

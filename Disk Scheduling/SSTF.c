#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
  int queue[7] = {82, 170, 43, 140, 24, 16, 190};
  int q_size = 7;
  int head = 50;
  int seek = 0, temp;
  float avg;

  int current_head = head; // Current head position for calculations

  printf("Seek Sequence: ");
  
  for (int i = 0; i < q_size; i++) {
    // Find the closest request
    int min_distance = abs(current_head - queue[i]);
    int min_index = i;

    for (int j = i + 1; j < q_size; j++) {
      int distance = abs(current_head - queue[j]);
      if (distance < min_distance) {
        min_distance = distance;
        min_index = j;
      }
    }

    // Swap the found minimum distance request with the current request
    temp = queue[i];
    queue[i] = queue[min_index];
    queue[min_index] = temp;

    // Update the seek time and the current head position
    seek += abs(current_head - queue[i]);
    current_head = queue[i];

    // Print the seek sequence
    printf("%d ", queue[i]);
  }

  printf("\nTotal seek time is %d\n", seek);
  avg = (float)seek / q_size;
  printf("Average seek time is %.2f\n", avg);

  return 0;
}

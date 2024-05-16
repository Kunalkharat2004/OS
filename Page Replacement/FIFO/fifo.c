#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct PageTable {
    int frame_no;
    bool valid;
};

// Function to check if referenced/asked page is already present in frame[] or not
// Returns true if page is already present else returns false
bool isPagePresent(struct PageTable PT[], int page, int n) {
    return PT[page].valid;
}

// Function to update the page table
// Return Nothing
void updatePageTable(struct PageTable PT[], int page, int fr_no, int status) {
    PT[page].valid = status;
    PT[page].frame_no = fr_no;
}

// Function to print the frame contents
// Return nothing
void printFrameContents(int frame[], int no_of_frames) {
    for (int i = 0; i < no_of_frames; i++)
        printf("%d ", frame[i]);
    printf("\n");
}

int main() {
    int n = 20;
    int no_of_frames = 3;
    int page_fault = 0, current = 0;
    bool flag = false;

    int reference_string[20] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    
    // Create frame array to store the pages at different point of times
    int frame[no_of_frames];
    memset(frame, -1, no_of_frames * sizeof(int));

    struct PageTable PT[50]; // Assume page table can have entries for page 0 to 49
    for (int i = 0; i < 50; i++)
        PT[i].valid = 0;
    
    printf("\n****The Contents inside the Frame array at different time:****\n");
    for (int i = 0; i < n; i++) {
        // Search the ith page in all allocated frames
        if (!isPagePresent(PT, reference_string[i], n)) {
            page_fault++; // Increase the count of page fault
            if (flag == false && current < no_of_frames) {
                frame[current] = reference_string[i];
                printFrameContents(frame, no_of_frames);
                updatePageTable(PT, reference_string[i], current, 1);
                current = current + 1;
                if (current == no_of_frames) {
                    current = 0;
                    flag = true; // So that we do not come to this if block again
                }
            } else { // Frames are full, APPLY FIFO
                // Find the FIFO page (victim page) to replace
                // The page pointed by current is FIFO page (victim page), so need to find it :)
                // Mark that page as INVALID as in Page Table
                // Set invalid frame no as -1 or anything (as function needs this parameter)
                updatePageTable(PT, frame[current], -1, 0);
                frame[current] = reference_string[i];
                printFrameContents(frame, no_of_frames);
                updatePageTable(PT, reference_string[i], current, 1);
                current = (current + 1) % no_of_frames;
            }
        }
    }

    printf("\nTotal No. of Page Faults = %d\n", page_fault);
    printf("\nPage Fault ratio = %.2f\n", (float)page_fault / n);
    printf("\nPage Hit Ratio = %.2f\n", (float)(n - page_fault) / n);

    return 0;
}

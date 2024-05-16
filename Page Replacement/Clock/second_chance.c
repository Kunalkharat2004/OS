#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<limits.h>

struct PageTable
{
    int frame_no;
    bool valid;
    bool second_chance;
};

bool isPagePresent(struct PageTable PT[], int page)
{
    return PT[page].valid;
}

void updatePageTable(struct PageTable PT[], int page, int fr_no, bool status, bool sc)
{
    PT[page].valid = status;
    PT[page].second_chance = sc;
    PT[page].frame_no = fr_no;
}

void printFrameContents(int frame[], int no_of_frames)
{
    for(int i = 0; i < no_of_frames; i++)
        printf("%d ", frame[i]);
    printf("\n");  
}

void searchSecondChancePage(struct PageTable PT[], int frame[], int no_of_frames, int *sc_page_index)
{
    while (true) {
        if (!PT[frame[*sc_page_index]].second_chance) {
            return;
        }
        PT[frame[*sc_page_index]].second_chance = false;
        *sc_page_index = (*sc_page_index + 1) % no_of_frames;
    }
}

int main()
{
    int n = 20;
    int no_of_frames = 3;
    int page_fault = 0, current = 0, sc_page_index = 0;
    bool flag = false;

    int reference_string[20] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};

    int frame[no_of_frames];
    memset(frame, -1, no_of_frames * sizeof(int));
    struct PageTable PT[50];
    for(int i = 0; i < 50; i++)
        PT[i].valid = false;
    
    printf("\n****The Contents inside the Frame array at different times:****\n");
    for(int i = 0; i < n; i++) 
    {
        if(!(isPagePresent(PT, reference_string[i])))
        {
            page_fault++;
            if(flag == false && current < no_of_frames)
            {   
                frame[current] = reference_string[i];
                printFrameContents(frame, no_of_frames);
                updatePageTable(PT, reference_string[i], current, true, false);
                current = current + 1;
                if(current == no_of_frames)
                    flag = true;         
            }
            else
            {
                searchSecondChancePage(PT, frame, no_of_frames, &sc_page_index);
                updatePageTable(PT, frame[sc_page_index], -1, false, false); 
                frame[sc_page_index] = reference_string[i];
                printFrameContents(frame, no_of_frames);
                updatePageTable(PT, reference_string[i], sc_page_index, true, false); 
            }
        }
        PT[reference_string[i]].second_chance = true;
    }  
    printf("\nTotal Number of Page Faults = %d\n", page_fault);
    printf("\nPage Fault ratio = %.2f\n", (float)page_fault/n);
    printf("\nPage Hit Ratio = %.2f\n", (float)(n - page_fault)/n);
    return 0;
}

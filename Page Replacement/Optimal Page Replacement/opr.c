#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<limits.h>

struct PageTable
{
    int frame_no;
    int last_time_of_access;
    bool valid;
};

bool isPagePresent(struct PageTable PT[], int page)
{
    return PT[page].valid;
}

void updatePageTable(struct PageTable PT[], int page, int fr_no, int status, int access_time)
{
    PT[page].valid = status;
    if(status == 1)  
    {  
      PT[page].last_time_of_access = access_time;
      PT[page].frame_no = fr_no;
    }
}

void printFrameContents(int frame[], int no_of_frames)
{
    for(int i = 0; i < no_of_frames; i++)
        printf("%d ", frame[i]);
    printf("\n");  
}

int searchOptimalPage(struct PageTable PT[], int reference_string[], int n, int frame[], int no_of_frames, int start)
{
    int farthest = start;
    int victim_page_index = -1;
    for(int i = 0; i < no_of_frames; i++)
    {
        int j;
        for(j = start + 1; j < n; j++)
        {
            if(frame[i] == reference_string[j])
            {
                if(j > farthest)
                {
                    farthest = j;
                    victim_page_index = i;
                }
                break;
            }
        }
        if(j == n)
            return i;
    }
    if(victim_page_index == -1)
        return 0;
    return victim_page_index;
}

int main()
{
    int n = 20;
    int no_of_frames = 3;
    int page_fault = 0, current = 0;
    bool flag = false;

    int reference_string[20] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};

    int frame[no_of_frames];
    memset(frame, -1, no_of_frames * sizeof(int));
    struct PageTable PT[50];
    for(int i = 0; i < 50; i++)
        PT[i].valid = 0;
    
    printf("\n****The Contents inside the Frame array at different time:****\n");
    for(int i = 0; i < n; i++) 
    {
        if(!(isPagePresent(PT, reference_string[i])))
        {
            page_fault++;
            if(flag == false && current < no_of_frames)
            {   
                frame[current] = reference_string[i];
                printFrameContents(frame, no_of_frames);
                updatePageTable(PT, reference_string[i], current, 1, i);
                current = current + 1;
                if(current == no_of_frames)
                    flag = true;         
            }
            else
            {
                int victim_page_index = searchOptimalPage(PT, reference_string, n, frame, no_of_frames, i);
                updatePageTable(PT, frame[victim_page_index], -1, 0, i); 
                frame[victim_page_index] = reference_string[i];
                printFrameContents(frame, no_of_frames);
                updatePageTable(PT, reference_string[i], victim_page_index, 1, i); 
            }
        }
        PT[reference_string[i]].last_time_of_access =  i;
    }  
    printf("\nTotal No. of Page Faults = %d\n", page_fault);
    printf("\nPage Fault ratio = %.2f\n", (float)page_fault/n);
    printf("\nPage Hit Ratio = %.2f\n", (float)(n - page_fault)/n);
    return 0;
}

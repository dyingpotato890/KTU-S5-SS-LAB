#include <stdio.h>
#include <stdlib.h>

int hitCount = 0, missCount = 0;
int MAX_FRAMES;
int frames[100];
int counter[100];

void initialize() {
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i] = -1;
        counter[i] = 0;
    }
}

void displayFrames() {
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i] != -1)
            printf("%d ", frames[i]);
        else
            printf("- ");
    }
    printf("\n");
}

// Function to find the least frequently used frame
int findLFU() {
    int min = counter[0];
    int lfu_frame = 0;

    for (int i = 1; i < MAX_FRAMES; i++) {
        if (counter[i] < min) {
            min = counter[i];
            lfu_frame = i;
        }
    }

    return lfu_frame;
}

void LFU(int pages[], int n) {
    int page_faults = 0;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        // Check if page already exists in frames
        for (int j = 0; j < MAX_FRAMES; j++) {
            if (frames[j] == page) {
                found = 1;
                counter[j]++;
                hitCount++;
                printf("page %d is already in memory\n",page);
                break;
            }
        }

        if (!found) {
            int lfu_frame = findLFU();
            frames[lfu_frame] = page;
            counter[lfu_frame] = 1;
            page_faults++;
            missCount++;
            printf("page %d is loaded in frame %d\n",page,lfu_frame);
        }

        displayFrames();
    }

    printf("Total Page Faults: %d\n", page_faults);
    printf("Hit Count: %d\n", hitCount);
    printf("Miss Count: %d\n", missCount);
    printf("\nHit Ratio = %f", (float)hitCount / (float)(hitCount + missCount));
    printf("\nMiss Ratio = %f", (float)missCount / (float)(hitCount + missCount));
}

int main() {
    int n;
    printf("Enter The Number of Pages: ");
    scanf("%d", &n);
    int pages[n];

    printf("Enter The Page Reference Sequence: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter The Number of Frames: ");
    scanf("%d", &MAX_FRAMES);

    initialize();
    LFU(pages, n);

    return 0;
}
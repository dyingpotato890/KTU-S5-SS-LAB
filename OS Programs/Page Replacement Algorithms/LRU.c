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

// Function to find the least recently used frame
int findLRU() {
    int max = counter[0];
    int lru_frame = 0;

    for (int i = 1; i < MAX_FRAMES; i++) {
        if (counter[i] > max) {
            max = counter[i];
            lru_frame = i;
        }
    }

    return lru_frame;
}

void LRU(int pages[], int n) {
    int page_faults = 0;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        // Check if page already exists in frames
        for (int j = 0; j < MAX_FRAMES; j++) {
            if (frames[j] == page) {
                found = 1;
                printf("Page %d is Already Inserted \n",page);
                hitCount++;
                break;
            }
        }

        if (!found) {
            int lru_frame = findLRU();
            printf("Page %d is Loaded in Frame %d\n",page,lru_frame);
            frames[lru_frame] = page;
            counter[lru_frame] = 0;
            page_faults++;
            missCount++;
        }

        // Increment counter for all frames
        for (int j = 0; j < MAX_FRAMES; j++) {
            counter[j]++;
        }

        // Reset counter for the used frame
        for (int j = 0; j < MAX_FRAMES; j++) {
            if (frames[j] == page) {
                counter[j] = 0;
                break;
            }
        }

        displayFrames();
    }

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
    LRU(pages, n);

    return 0;
}

#include <stdio.h>
#include <string.h>

struct Book {
    int id;
    int popularity;
    int lastAccessed;   // smaller value = older access
};

int findBook(struct Book shelf[], int size, int id) {
    for (int i = 0; i < size; i++) {
        if (shelf[i].id == id)
            return i;
    }
    return -1;
}

int findLeastRecentlyAccessed(struct Book shelf[], int size) {
    int index = 0;
    for (int i = 1; i < size; i++) {
        if (shelf[i].lastAccessed < shelf[index].lastAccessed) {
            index = i;
        }
    }
    return index;
}

int main() {
    int capacity, Q;
    scanf("%d %d", &capacity, &Q);

    struct Book shelf[capacity];
    int currentSize = 0;
    int timer = 1;  // increases every access/add

    for (int i = 0; i < capacity; i++) {
        shelf[i].id = -1; // mark empty
    }

    while (Q--) {
        char op[10];
        scanf("%s", op);

        if (strcmp(op, "ADD") == 0) {
            int x, y;
            scanf("%d %d", &x, &y);

            int idx = findBook(shelf, currentSize, x);

            if (idx != -1) {
                // Book exists ? update popularity & last accessed
                shelf[idx].popularity = y;
                shelf[idx].lastAccessed = timer++;
            } 
            else {
                // Book does not exist
                if (currentSize < capacity) {
                    // place in next empty slot
                    shelf[currentSize].id = x;
                    shelf[currentSize].popularity = y;
                    shelf[currentSize].lastAccessed = timer++;
                    currentSize++;
                } 
                else {
                    // Shelf full ? remove LRA book
                    int removeIndex = findLeastRecentlyAccessed(shelf, currentSize);

                    // Replace removed book with new book
                    shelf[removeIndex].id = x;
                    shelf[removeIndex].popularity = y;
                    shelf[removeIndex].lastAccessed = timer++;
                }
            }
        }

        else if (strcmp(op, "ACCESS") == 0) {
            int x;
            scanf("%d", &x);

            int idx = findBook(shelf, currentSize, x);

            if (idx != -1) {
                printf("%d\n", shelf[idx].popularity);
                shelf[idx].lastAccessed = timer++;
            } else {
                printf("-1\n");
            }
        }
    }

    return 0;
}
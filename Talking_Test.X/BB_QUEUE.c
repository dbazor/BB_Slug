/*
   File:   BB_QUEUE.c
   Author: Pavlo

   Created on November 10, 2011, 8:43 AM
 */
#include "BB_QUEUE.h"
#include "BB_BOARD.h"
#include <stdbool.h>

/*******************************************************************************
 * PRIVATE DATATYPES                                                           *
 ******************************************************************************/
static char queue[MAX_SIZE];
int first = 0;
int last = -1;
int charCount = 0;

char GetFQueue() {
    return queue[first];
}

bool IsQueueEmpty() {
    return charCount == 0;
}

bool IsQueueFull() {
    return charCount == MAX_SIZE;
}

int queueSize() {
    return charCount;
}

void insert(int data) {

    if (!IsQueueFull()) {
        if (last == MAX_SIZE - 1) {
            last = -1;
        }
        queue[++last] = data;
        charCount++;
    }
}

int removeData() {
    int data = queue[first++];
    if (first == MAX_SIZE) {
        first = 0;
    }
    charCount--;
    return data;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

const unsigned long long minimumNoItemsForNewThread = 1000000;
const bool debug = false;

struct Range {
    unsigned long long start;
    unsigned long long end;
};

unsigned long long sumWithOneThread(unsigned long long start, unsigned long long end);
unsigned long long sumWithMultipleThreads(int noThreads, unsigned long long start, unsigned long long end);
void writeToFile(unsigned long long result);

char fileName[] = "output.txt";
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
auto *toBeSummedUp = new vector<unsigned long long>();
unsigned long long noItemsToBeSummedUp = 0;
unsigned int noWorkerThreads = 0;

int main(int argc, char* argv[]) {
    const unsigned int noThreads = atoi(argv[1]);
    const unsigned long long start = strtoull(argv[2], 0, 10);
    const unsigned long long end = strtoull(argv[3], 0, 10);

    unsigned long long result;

    printf("Threads: %d, Start: %llu, End %llu\n", noThreads, start, end);

    if (noThreads == 1) {
        result = sumWithOneThread(start, end);
    } else {
        result = sumWithMultipleThreads(noThreads, start, end);
    }

    printf("Result: %llu\n", result);
    writeToFile(result);
    printf("Wrote result to %s\n", fileName);

    exit(0);
}

void writeToFile(unsigned long long result) {
    ofstream output;
    remove(fileName);
    output.open(fileName);
    output << result;
    output.close();
}

unsigned long long rangeSum(unsigned long long start, unsigned long long end) {
    unsigned long long result = 0;
    for (; start <= end; start++) {
        result += start;
    }
    return result;
}

unsigned long long itemSum(vector<unsigned long long> *items) {
    unsigned long long result = 0;
    for (unsigned long long item : *items) {
        result += item;
    }
    return result;
}

unsigned long long sumWithOneThread(unsigned long long start, unsigned long long end) {
    return rangeSum(start, end);
}

void finishedSummingUp(unsigned long long result) {
    pthread_mutex_lock(&mutex);
    noWorkerThreads--;
    noItemsToBeSummedUp++;
    toBeSummedUp->push_back(result);
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
}

void *rangeSumThread(void *args) {
    struct Range *range = (Range *) args;
    auto result = rangeSum(range->start, range->end);
    if (debug) printf("Sum of %llu-%llu: %llu\n", range->start, range->end, result);
    finishedSummingUp(result);
    return NULL;
}

void *itemSumThread(void *args) {
    auto *items = (vector<unsigned long long> *) args;
    auto result = itemSum(items);
    ostringstream itemString;
    for (unsigned long long item : *items) {
        itemString << item << ",";
    }
    if (debug) printf("Sum of %s: %llu\n", itemString.str().c_str(), result);
    finishedSummingUp(result);
    return NULL;
}

unsigned long long sumWithMultipleThreads(int noThreads, unsigned long long start, unsigned long long end) {
    // one thread is already running (main)
    noThreads--;

    unsigned long long noItems = 1 + end - start;
    unsigned long long initialRangeSizePerThread = noItems / noThreads;
    if (noItems % noThreads != 0) {
        initialRangeSizePerThread++;
    }

    pthread_mutex_lock(&mutex);
    unsigned long long current = start;
    for (int i = 0; i < noThreads; i++) {
        struct Range *range = new Range;
        range->start = current;
        current += initialRangeSizePerThread;
        range->end = current - 1;
        if (range->start > end) {
            break;
        }
        if (range->end > end) {
            range->end = end;
        }

        //spawn thread
        pthread_t thread;
        pthread_create(&thread, NULL, rangeSumThread, range);
        pthread_detach(thread);
        noWorkerThreads++;
    }
    pthread_mutex_unlock(&mutex);

    unsigned int localNoWorkerThreads;
    unsigned long long localNoItemsToBeSummedUp;

    do {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&condition, &mutex);

        //new item has been added
        if (noItemsToBeSummedUp > minimumNoItemsForNewThread ||
                (noWorkerThreads == 0 && noItemsToBeSummedUp > 1)) {
            //spawn thread with *toBeSummedUp
            pthread_t thread;
            pthread_create(&thread, NULL, itemSumThread, toBeSummedUp);
            pthread_detach(thread);
            noWorkerThreads++;

            noItemsToBeSummedUp = 0;
            toBeSummedUp = new vector<unsigned long long>();
        }

        localNoWorkerThreads = noWorkerThreads;
        localNoItemsToBeSummedUp = noItemsToBeSummedUp;
        pthread_mutex_unlock(&mutex);
    } while (localNoWorkerThreads != 0 || localNoItemsToBeSummedUp != 1);

    return toBeSummedUp->at(0);
}

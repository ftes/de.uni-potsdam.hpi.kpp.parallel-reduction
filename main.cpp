#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <fstream>

#include "RangeSum.h"

using namespace std;

char fileName[] = "output.txt";
unsigned long long sumWithOneThread(unsigned long long start, unsigned long long end);
void writeToFile(unsigned long long result);

int main(int argc, char* argv[]) {
    const unsigned int noThreads = atoi(argv[1]);
    const unsigned long long start = strtoull(argv[2], 0, 10);
    const unsigned long long end = strtoull(argv[3], 0, 10);

    unsigned long long result;

    printf("Threads: %d, Start: %llu, End %llu\n", noThreads, start, end);

    //    if (noThreads == 1) {
    result = sumWithOneThread(start, end);
    //    }

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

unsigned long long sumWithOneThread(unsigned long long start, unsigned long long end) {
    RangeSum *sum = new RangeSum(start, end);
    return sum->getResult();
}
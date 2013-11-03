#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

using namespace std;

void *print_message_function( void *ptr );

int main(int argc, char* argv[])
{
    const unsigned int noThreads = atoi(argv[1]);
    const unsigned long long startIndex = strtoull(argv[2], 0, 10);
    const unsigned long long endIndex = strtoull(argv[3], 0, 10);
    
    printf("Threads: %d, Start: %llu, End %llu\n", noThreads, startIndex, endIndex);
    
    cout << "test\n";

    exit(0);
}
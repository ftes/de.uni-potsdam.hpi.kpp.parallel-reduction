/* 
 * File:   RangeSum.cpp
 * Author: fredrik
 * 
 * Created on November 3, 2013, 8:16 PM
 */

#include "RangeSum.h"

RangeSum::RangeSum(unsigned long long start, unsigned long long end) {
    result = 0;
    for (int i=start; i<=end; i++) {
        result += i;
    }
}

unsigned long long RangeSum::getResult() {
    return result;
}
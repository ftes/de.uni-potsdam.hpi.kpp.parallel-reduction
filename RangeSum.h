/* 
 * File:   RangeSum.h
 * Author: fredrik
 *
 * Created on November 3, 2013, 8:12 PM
 */

#ifndef RANGESUM_H
#define	RANGESUM_H

class RangeSum {
private:
    unsigned long long result;
public:
    RangeSum(unsigned long long start, unsigned long long end);
    unsigned long long getResult();
};

#endif	/* RANGESUM_H */


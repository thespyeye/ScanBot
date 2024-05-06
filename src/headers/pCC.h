
#ifndef PCC_H
#define PCC_hh

// Definitions for the functions in pointCloudConverter.c

inline double d_abs(double x);

inline double getSlope(double* first, double* last);

double*** pointsToLinePairs(double** restrict targ, int size);

#endif

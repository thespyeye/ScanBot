
#ifndef PCC_H
#define PCC_hh

// Definitions for the functions in pointCloudConverter.c

inline double getDistBetweenPoints(double* first, double* last);

inline double d_abs(double x);

inline double getSlope(double* first, double* last);

double*** unsortedPointsToLinePairs(double** targ, int size, int* retSize);

#endif

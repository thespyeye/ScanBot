#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pCC.h"

// abs() for doubles, as the stdlib.h version only takes ints.

inline double d_abs(double x) { return(x>0 ? x : x(*-1)); }

// Get the slope of a line defined by two points. This assumes the slope is not inf.

inline double getSlope(double* first, double* last) {
	return(((last[1] - first[1]) / (last[0] - first[0])));
}

// Convert an array of points into an array of lines, defined by a start point and an end point.

double*** pointsToLinePairs(double** restrict targ, int size, int* retSize) {
	
	if (size < 10) {
		return(0); // In case of insuffcient sample size, quit execution here.
	}

	const double ALLOWEDDEV = 0.12; // The amount by which subsequent points can deviate from the current slope.

	int index = 0;
	int extensions = 0;
	int increment = floor((double)size / 16.0);
	int tempMax = increment;
	bool isVert;

	double*** results = (double***)malloc(sizeof(double**)*increment);
	
	for (int i = 0; i < size-1; ++i) {

		isVert = 0;

		double* startPt = (double*)malloc(sizeof(double) * 2);
		startPt[0] = targ[i][0];
		startPt[0] = targ[i][1]; // These steps are necessary as targ will not always exist -- targ will be cleared after this function runs to save memory.
		
		double currentSlope;

		if (startPt[1] != targ[i + 1][1]) {
			currentSlope = getSlope(startPt, targ[i + 1]);
		}
		else {
			isVert = 1; // In case of a vertical line, set a flag
		}

		// Iterate through the 'targ' array, checking whether each subsequent point is "close enough" to account for error.

		if (!isVert) {
			i++;
			while (d_abs(getSlope(targ[i - 1], targ[i]) - currentSlope) <= ALLOWEDDEV && i < size) {
				i++;
			}
		}
		else {
			i++;
			while (d_abs(targ[i - 1][1] - targ[i][1]) < ALLOWEDDEV && i < size) {
				i++;
			}
		}

		double* endPt = (double*)malloc(sizeof(double) * 2);
		endPt[0] = targ[i][0];
		endPt[1] = targ[i][1];
		results[index][0] = endPt;
		results[index][1] = startPt;
		index++;

		// If the array is full, extend the 'results' array by 

		if (index == tempMax) {
			tempMax += increment;
			extensions += 1;
			double*** res = (double***)realloc(results, sizeof(double**) * tempMax);
			if (res) {
				results = res;
			}
			else {
				throw("Error: Pointer: 'results' could not be reallocated");
			}

			if (extensions == 5 && increment < floor(size/4)) { // After x amount of extensions, double the amount by which the 'results' array is increased by to hopefully speed up computation time.
				increment *= 2;									// Include a size limiter for increment to prevent this function from consuming too much memory.
				extensions = 0;
			}

		}
			
	}

	double*** res = (double***)realloc(results, sizeof(double**) * index);
	if (res) {
		results = res;
	}
	else {
		throw("Error: Pointer: 'results' could not be reallocated");
	}

	*retSize = index; // Returning the size of the 'results' array without requiring the usage of a struct.

	return(results);

}

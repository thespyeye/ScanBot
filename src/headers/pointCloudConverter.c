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

// Get the distance between two points, defined by first and last.

inline double getDistBetweenPoints(double* first, double* last) {
	return(sqrt(pow(d_abs(first[0] - last[0]), 2) + pow(d_abs(first[1] - last[1]), 2)));
}


int compare(double* pointA, double* pointB, double currentSlope, const double ALLOWEDDEV, const double ALLOWEDDIST) {

	double slope;
	double altSlope;

	if (d_abs(pointA[1] - pointB[1]) < 0.01) {
		slope = 9999.99;
		altSlope = -9999.99;
	}
	else {
		slope = getSlope(pointA, pointB);
		altSlope = getSlope(pointB, pointA)
	}

	double percentDifference = d_abs((currentSlope - slope) / currentSlope);

	double altPercentDifference = d_abs((currentSlope - altSlope) / currentSlope);

	if ((percentDifference <= ALLOWEDDEV || altPercentDifference <= ALLOWEDDEV) && getDistBetweenPoints(pointA, pointB) < ALLOWEDDIST) {
		return(1);
	}

	return(0);
}


// Convert an unsorted array of points into an array of lines, defined by a start point and an end point.

double*** unsortedPointsToLinePairs(double** targ, int size, int* retSize) {
	
	if (size < 10) {
		return(0); // In case of insuffcient sample size, quit execution here.
	}

	const double ALLOWEDDEV = 0.05; // The percent by which subsequent points can deviate from the current slope.
	const double ALLOWEDDIST = 0.2; // The distance points can be from one another. Set this low for high resolution data sets, and high for low resolution datasets.
	const int EXTENSIONSTARGET = 2; // The amount of times the results array is reallocated until the increment variable is doubled.

	int index = 0; // The index of the results array that will be modified when a line is calculated.
	int extensions = 0; // The amount of times the results array has been extended.
	int increment = floor((double)size / 16.0); // The amount by which the results array is extended by.
	int tempMax = increment; // The current maximum size of the results array.
	bool isVert; // A flag to track whether the current line is vertical or not.

	double*** results = malloc(sizeof(double**) * increment);

	while (size > 0) {

		

		double currentSlope; // The slope of the current line.

		double smallestDist = 1000;
		int index = 1;

		for (int i = 1; i < size; ++i) {
			double calcDist = getDistBetweenPoints(targ[0], targ[i]);
			if (calcDist < smallestDist) {
				index = i;
			}
		}
		
		if (d_abs(targ[i][1] - targ[index][1]) < 0.01) {
			currentSlope = 9999.99;
		}
		else {
			currentSlope = getSlope(targ[i], targ[index]);
		}

		int* thisLineIndexes = malloc(sizeof(int) * size);
		int thisLineIndex = 2;
		thisLineIndexes[0] = 0;
		thisLineIndexes[1] = index;

		for (int i = 2; i < size; ++i) {
			for (int j = 0; j < thisLineIndex; ++j) {
				if (i == thisLineIndexes[j]) {
					goto loopEnd;
				}
			}

			for (int j = 0; j < thisLineIndexes; ++j) {
				if (compare(targ[thisLineIndexes[j]], targ[i], currentSlope, ALLOWEDDEV, ALLOWEDDIST)) {
					thisLineIndexes[thisLineIndex] = i;
					thisLineIndex++;
				}
			}
			

			loopEnd:
		}

		double largestDiff = 0;
		int endIndex = 1;

		for (int i = 1; i < thisLineIndex; ++i) {
			double dist = getDistBetweenPoints(targ[thisLineIndexes[0]], targ[thisLineIndexes[i]]);

			if (dist > largestDiff) {
				largestDiff = dist;
				endIndex = i;
			}

		}

		largestDiff = 0;
		int startIndex = endIndex;

		for (int i = 0; i < thisLineIndex; ++i) {
			if (i == endIndex) {
				continue;
			}

			double dist = getDistBetweenPoints(targ[thisLineIndexes[endIndex]], targ[thisLineIndexes[i]]);

			if (dist > largestDiff) {
				largestDiff = dist;
				startIndex = i;
			}

		}

		double* startPt = malloc(sizeof(double) * 2);
		startPt[0] = targ[thisLineIndexes[startIndex]][0];
		startPt[1] = targ[thisLineIndexes[startIndex]][1]; // These steps are necessary as targ will not always exist -- targ will be cleared after this function runs to save memory.

		double* endPt = malloc(sizeof(double) * 2);
		endPt[0] = targ[thisLineIndexes[endIndex]][0];
		endPt[1] = targ[thisLineIndexes[endIndex]][1];

		results[index][0] = endPt;
		results[index][1] = startPt;

		index++;

		// Remove all points that were used for the current line, and free thisLineIndexes.

		for (int i = 0; i < thisLineIndex; ++i) {
			for (int j = thisLineIndexes[i]; j < size-1; ++j) {
				targ[j] = targ[j + 1];
			}
			size--;
		}

		free(thisLineIndexes);

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

			if (extensions == 2 && increment < floor(size / 4)) { // After x (3) amount of extensions, double the amount by which the 'results' array is increased by to hopefully speed up computation time.
				increment *= 2;									// Include a size limiter for increment to prevent this function from consuming too much memory when it reallocates the results array.
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

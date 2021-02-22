#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class Image{
private:

	int numRows;
	int numCols;
	int minValue;
	int maxValue;
	int maxHeight; // the largest histogramArray[i] of the given portion of the histogram
	int offset;
	int thresholdValue;

public:

	int* histogramArray;
	int** histogramGraph;
	int* gaussianArray;
	int** gaussianGraph;
	int** gapGraph;

public:
	Image(ifstream&);

	int getNumRows();

	void setNumRows(int);

	int getNumCols();

	void setNumCols(int);

	int getMinValue();

	void setMinValue(int);

	int getMaxValue();

	void setMaxValue(int);

	int getMaxHeight();

	void setMaxHeight(int);

	int getOffset();

	void setOffset(int);

	int getThresholdValue();

	void setThresholdValue(int);

	void initArrays(int);

	void loadHistogram(ifstream&, int*);

	void plotHistogramGraph();

	void prettyPrint(int**, ofstream&);

	int biMeanGaussian(int, ofstream&, ofstream&);

	void reset1DArray(int*);

	void reset2DArray(int**);

	double fitGaussian(int, int);

	double computeMean(int, int, int);

	double computeVariance(int, int, double);

	void plotGaps();
	
	double modifiedGaussian(int, double, double, int);

	void bestThresholdPlot(int);
};

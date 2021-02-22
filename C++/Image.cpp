#include "Image.h"

Image::Image(ifstream& input){
	if (input.good()) {
		input >> this->numRows;
		input >> this->numCols;
		input >> this->minValue;
		input >> this->maxValue;
	}
	this->offset = (this->maxValue - this->minValue) / 10;
	this->thresholdValue = offset;

	initArrays(1);
}

int Image::getNumRows() {
	return numRows;
}

void Image::setNumRows(int numRows) {
	this->numRows = numRows;
}

int Image::getNumCols() {
	return numCols;
}

void Image::setNumCols(int numCols) {
	this->numCols = numCols;
}

int Image::getMinValue() {
	return minValue;
}

void Image::setMinValue(int minValue) {
	this->minValue = minValue;
}

int Image::getMaxValue() {
	return maxValue;
}

void Image::setMaxValue(int maxValue) {
	this->maxValue = maxValue;
}

int Image::getMaxHeight() {
	return maxHeight;
}

void Image::setMaxHeight(int maxHeight) {
	this->maxHeight = maxHeight;
}

int Image::getOffset() {
	return offset;
}

void Image::setOffset(int offset) {
	this->offset = offset;
}

int Image::getThresholdValue() {
	return thresholdValue;
}

void Image::setThresholdValue(int thresholdValue) {
	this->thresholdValue = thresholdValue;
}

void Image::initArrays(int passId){
	if (passId == 1) {
		histogramArray = new int[maxValue + 1];
		gaussianArray = new int[maxValue + 1];

		for (int i = 0; i <= maxValue; i++) {
			histogramArray[i] = 0;
			gaussianArray[i] = 0;
		}

	}
	else if (passId == 2) {

		histogramGraph = new int*[maxValue + 1];
		gaussianGraph = new int*[maxValue + 1];
		gapGraph = new int*[maxValue + 1];

		for (int i = 0; i <= maxValue; i++) {
			histogramGraph [i] = new int[maxHeight + 1];
			gaussianGraph[i] = new int[maxHeight + 1];
			gapGraph[i] = new int[maxHeight + 1];
			for (int j = 0; j <= maxHeight; j++) {
				histogramGraph[i][j] = 0;
				gaussianGraph[i][j] = 0;
				gapGraph[i][j] = 0;
			}
		}
	}
}
void Image::loadHistogram(ifstream& input, int* array){
	int index = 0;
	while (!input.eof()) {
		input >> index;
		input >> array[index];
		if (this->getMaxHeight() < array[index]) {
			this->setMaxHeight(array[index]);
		}
	}
	initArrays(2);
}
void Image::plotHistogramGraph(){
	for (int i = 0; i <= maxValue; i++) {
		histogramGraph[i][histogramArray[i]] = 1;
	}
}
void Image::prettyPrint(int** graph, ofstream& output){
	try {

		for (int i = 0; i <= maxValue; i++) {
			for (int j = 0; j <= maxHeight; j++) {
				if (graph[i][j] <= 0) {
					output << " ";
				}
				else {
					output << "*";
				}
			}
			output << endl;
		}

		output << endl;
	}
	catch (exception e) {
		cout << "There is an error: " << e.what() << endl;
	}
}
int Image::biMeanGaussian(int threshold, ofstream& output1, ofstream& output2){
	double sum1 = 0.0;
	double sum2 = 0.0;
	double totalSum = 0.0;
	double minSumDifference = 999999.0;
	int bestThreshold = threshold;

	while (threshold < (this->getMaxValue() - this->getOffset())) {
		reset1DArray(this->gaussianArray);
		reset2DArray(this->gaussianGraph);
		reset2DArray(this->gapGraph);

		sum1 = fitGaussian(0, threshold);
		sum2 = fitGaussian(threshold, this->getMaxValue());
		totalSum = sum1 + sum2;

		if (totalSum < minSumDifference) {
			minSumDifference = totalSum;
			bestThreshold = threshold;
		}
		threshold++;
		prettyPrint(gaussianGraph, output1);
		plotGaps();
		prettyPrint(gapGraph, output2);
	}
	return bestThreshold;
}
void Image::reset1DArray(int* array){
	for (int i = 0; i <= this->getMaxValue(); i++) {
		array[i] = 0;
	}
}
void Image::reset2DArray(int** array){
	for (int i = 0; i <= this->getMaxValue(); i++) {
		for (int j = 0; j <= this->getMaxHeight(); j++) {
			array[i][j] = 0;
		}
	}
}

double Image::fitGaussian(int leftIndex, int rightIndex){
	double sum = 0.0;
	double gaussianValue;
	double maxGaussianValue;
	double mean = computeMean(leftIndex, rightIndex, this->getMaxHeight());
	double variance = computeVariance(leftIndex, rightIndex, mean);

	for (int index = leftIndex; index <= rightIndex; index++) {
		gaussianValue = modifiedGaussian(index, mean, variance, this->getMaxHeight());
		sum += abs(gaussianValue - (double)histogramArray[index]);
		gaussianArray[index] = (int)gaussianValue;
		gaussianGraph[index][(int)gaussianValue] = 1;
	}
	return sum;
}

double Image::computeMean(int leftIndex, int rightIndex, int maxHeight){
	maxHeight = 0;
	int sum = 0;
	int pixelCount = 0;

	for (int index = leftIndex; index < rightIndex; index++) {
		sum += (histogramArray[index] * index);
		pixelCount += histogramArray[index];

		if (histogramArray[index] > maxHeight) {
			maxHeight = histogramArray[index];
		}
	}
	return ((double)sum / (double)pixelCount);
}

double Image::computeVariance(int leftIndex, int rightIndex, double mean){
	double sum = 0.0;
	int pixelCount = 0;

	for (int index = leftIndex; index < rightIndex; index++) {
		sum += (double)histogramArray[index] * pow((double)index - mean, 2);
		pixelCount += histogramArray[index];
	}
	return ((double)sum / (double)pixelCount);
}

void Image::plotGaps(){
	int first = 0;
	int last = 0;
	for (int index = this->getMinValue(); index < this->getMaxValue(); index++) {
		first = min(histogramArray[index], gaussianArray[index]);
		last = max(histogramArray[index], gaussianArray[index]);

		while (first < last) {
			gapGraph[index][first] = 1;
			first++;
		}
	}
}

double Image::modifiedGaussian(int x, double mean, double variance, int maxHeight){
	return (double)(maxHeight * exp(-(pow(x - mean, 2) / (2 * variance))));
}

void Image::bestThresholdPlot(int bestThrVal){
	double sum1 = 0.0;
	double sum2 = 0.0;

	reset1DArray(gaussianArray);
	reset2DArray(gaussianGraph);
	reset2DArray(gapGraph);

	sum1 = fitGaussian(0, bestThrVal);
	sum2 = fitGaussian(bestThrVal, this->getMaxValue());

	plotGaps();
}

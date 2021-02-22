#include <iostream>
#include <fstream>
#include <string>

#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	string inputFile;
	string outputFile1;
	string outputFile2;
	string outputFile3;

	if (argc != 5) {
		cout << "Invalid number of arguments. Try again" << endl;
		return 0;
	}
	try {
		inputFile = argv[1];
		outputFile1 = argv[2];
		outputFile2 = argv[3];
		outputFile3 = argv[4];

		ifstream input;
		ofstream output1;
		ofstream output2;
		ofstream output3;

		input.open(inputFile);
		output1.open(outputFile1);
		output2.open(outputFile2);
		output3.open(outputFile3);

		Image* image = new Image(input);
		image->loadHistogram(input, image->histogramArray);
		image->plotHistogramGraph();
		output1 << "Result of Histrogram Graph:" << endl;
		image->prettyPrint(image->histogramGraph, output1);

		int bestThreshold = image->biMeanGaussian(image->getThresholdValue(), output2, output3);
		output1 << "\nBest Threshold Value: " << bestThreshold << endl;

		image->bestThresholdPlot(bestThreshold);
		output1 << "Pretty Print of Gaussian Graph:" << endl;
		image->prettyPrint(image->gaussianGraph, output1);

		output1 << "Pretty Print of GapGraph:" << endl;
		image->prettyPrint(image->gapGraph, output1);

		input.close();
		output1.close();
		output2.close();
		output3.close();
	}
	catch (exception e) {
		cout << "There is an error: " << e.what() << endl;
	}
	return 0;
}
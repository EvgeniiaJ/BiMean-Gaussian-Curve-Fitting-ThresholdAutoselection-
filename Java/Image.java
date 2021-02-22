import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Image {

	private int numRows;
	private int numCols;
	private int minValue;
	private int maxValue;
	private int maxHeight; // the largest histogramArray[i] of the given portion of the histogram
	private int offset;
	private int thresholdValue;

	public int[] histogramArray;
	public int[][] histogramGraph;
	public int[] gaussianArray;
	public int[][] gaussianGraph;
	public int[][] gapGraph;

	Image(Scanner input) {

		if (input.hasNext()) {
			numRows = input.nextInt();
		}
		if (input.hasNext()) {
			numCols = input.nextInt();
		}
		if (input.hasNext()) {
			minValue = input.nextInt();
		}
		if (input.hasNext()) {
			maxValue = input.nextInt();
		}

		this.offset = (maxValue - minValue) / 10;
		this.thresholdValue = offset;

		initArrays(1);
	}

	public int getNumRows() {
		return numRows;
	}

	public void setNumRows(int numRows) {
		this.numRows = numRows;
	}

	public int getNumCols() {
		return numCols;
	}

	public void setNumCols(int numCols) {
		this.numCols = numCols;
	}

	public int getMinValue() {
		return minValue;
	}

	public void setMinValue(int minValue) {
		this.minValue = minValue;
	}

	public int getMaxValue() {
		return maxValue;
	}

	public void setMaxValue(int maxValue) {
		this.maxValue = maxValue;
	}

	public int getMaxHeight() {
		return maxHeight;
	}

	public void setMaxHeight(int maxHeight) {
		this.maxHeight = maxHeight;
	}

	public int getOffset() {
		return offset;
	}

	public void setOffset(int offset) {
		this.offset = offset;
	}

	public int getThresholdValue() {
		return thresholdValue;
	}

	public void setThresholdValue(int thresholdValue) {
		this.thresholdValue = thresholdValue;
	}

	public void initArrays(int passId) {
		
		if (passId == 1) {
			
			histogramArray = new int[maxValue + 1];
			gaussianArray = new int[maxValue + 1];
			for (int i = 0; i <= maxValue; i++) {
				histogramArray[i] = 0;
				gaussianArray[i] = 0;
			}
			
		} else if (passId == 2) {
			
			histogramGraph = new int[maxValue + 1][maxHeight + 1];
			gaussianGraph = new int[maxValue + 1][maxHeight + 1];
			gapGraph = new int[maxValue + 1][maxHeight + 1];

			for (int i = 0; i <= maxValue; i++) {
				for (int j = 0; j <= maxHeight; j++) {
					histogramGraph[i][j] = 0;
					gaussianGraph[i][j] = 0;
					gapGraph[i][j] = 0;
				}
			}
			
		}
	}

	public void loadHistogram(Scanner input, int[] array) {
		int index = 0;
		while (input.hasNext()) {
			index = input.nextInt();
			array[index] = input.nextInt();
			if (this.getMaxHeight() < array[index]) {
				this.setMaxHeight(array[index]);
			}
		}
		initArrays(2);
	}

	public void plotHistogramGraph() {
		for (int i = 0; i <= maxValue; i++) {
			histogramGraph[i][histogramArray[i]] = 1;
		}
	}

	public void prettyPrint(int[][] graph, FileWriter output) {
		
		try {
			
			for (int i = 0; i <= maxValue; i++) {
				for (int j = 0; j <= maxHeight; j++) {
					if (graph[i][j] <= 0) {
						output.write(" ");
					} else {
						output.write("*");
					}
				}
				output.write("\n");
			}
			
			output.write("\n");
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

	public int biMeanGaussian(int threshold, FileWriter output1, FileWriter output2) {
		double sum1;
		double sum2;
		double totalSum;
		double minSumDifference = 999999.0;
		int bestThreshold = threshold;

		while (threshold < (this.getMaxValue() - this.getOffset())) {
			reset1DArray(this.gaussianArray);
			reset2DArray(this.gaussianGraph);
			reset2DArray(this.gapGraph);

			sum1 = fitGaussian(0, threshold);
			sum2 = fitGaussian(threshold, this.getMaxValue());
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

	public void reset1DArray(int[] array) {
		for (int i = 0; i <= this.getMaxValue(); i++) {
			array[i] = 0;
		}
	}

	public void reset2DArray(int[][] array) {
		for (int i = 0; i <= this.getMaxValue(); i++) {
			for (int j = 0; j <= this.getMaxHeight(); j++) {
				array[i][j] = 0;
			}
		}
	}

	public double fitGaussian(int leftIndex, int rightIndex) {

		double sum = 0.0;
		double gaussianValue;
		double maxGaussianValue;
		double mean = computeMean(leftIndex, rightIndex, this.getMaxHeight());
		double variance = computeVariance(leftIndex, rightIndex, mean);

		for (int index = leftIndex; index <= rightIndex; index++) {
			gaussianValue = modifiedGaussian(index, mean, variance, this.getMaxHeight());
			sum += Math.abs(gaussianValue - (double) histogramArray[index]);
			gaussianArray[index] = (int) gaussianValue;
			gaussianGraph[index][(int) gaussianValue] = 1;
		}
		return sum;
	}

	public double computeMean(int leftIndex, int rightIndex, int maxHeight) {
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
		return ((double) sum / (double) pixelCount);
	}

	public double computeVariance(int leftIndex, int rightIndex, double mean) {
		double sum = 0.0;
		int pixelCount = 0;

		for (int index = leftIndex; index < rightIndex; index++) {
			sum += (double) histogramArray[index] * Math.pow((double) index - mean, 2);
			pixelCount += histogramArray[index];
		}
		return ((double) sum / (double) pixelCount);
	}

	public void plotGaps() {
		int first, last;
		for (int index = this.getMinValue(); index < this.getMaxValue(); index++) {
			first = Math.min(histogramArray[index], gaussianArray[index]);
			last = Math.max(histogramArray[index], gaussianArray[index]);

			while (first < last) {
				gapGraph[index][first] = 1;
				first++;
			}
		}
	}

	public double modifiedGaussian(int x, double mean, double variance, int maxHeight) {
		return (double) (maxHeight * Math.exp(-(Math.pow(x - mean, 2) / (2 * variance))));
	}

	public void bestThresholdPlot(int bestThrVal) {
		double sum1, sum2;

		reset1DArray(gaussianArray);
		reset2DArray(gaussianGraph);
		reset2DArray(gapGraph);

		sum1 = fitGaussian(0, bestThrVal);
		sum2 = fitGaussian(bestThrVal, this.getMaxValue());

		plotGaps();

	}

}

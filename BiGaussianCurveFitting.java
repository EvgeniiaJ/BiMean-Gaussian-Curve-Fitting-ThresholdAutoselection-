import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class BiGaussianCurveFitting {

	public static void main(String[] args) {
		String inputFileName = args[0];
		String outputFileName1 = args[1];
		String outputFileName2 = args[2];
		String outputFileName3 = args[3];

		try {
			
			Scanner input = new Scanner(new File(inputFileName));
			FileWriter output1 = new FileWriter(new File(outputFileName1));
			FileWriter output2 = new FileWriter(new File(outputFileName2));
			FileWriter output3 = new FileWriter(new File(outputFileName3));
			
			Image image = new Image(input);
			image.loadHistogram(input, image.histogramArray);
			image.plotHistogramGraph();
			output1.write("Result of Histogram Graph:\n");
			image.prettyPrint(image.histogramGraph, output1);
			
			int bestThreshold = image.biMeanGaussian(image.getThresholdValue(), output2, output3);
			output1.write("\nBest Threshold Value: " + bestThreshold + "\n");

			image.bestThresholdPlot(bestThreshold);
			output1.write("\nPrettyPrint of GaussGraph:\n");
			image.prettyPrint(image.gaussianGraph, output1);

			output1.write("\nPrettyPrint of GapGraph:\n");
			image.prettyPrint(image.gapGraph, output1);
			
			input.close();
			output1.close();
			output2.close();
			output3.close();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}

}

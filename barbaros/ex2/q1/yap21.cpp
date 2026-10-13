#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <ctime>

void printArray(std::vector<std::vector<uint8_t>>& imageArray){
	int sizeOfVectori = imageArray.size();
	int sizeOfVectorj = imageArray[0].size();
	
	for(int i=0; i<sizeOfVectori; i++){
		for(int j=0; j<sizeOfVectorj; j++){
			std::cout<< (int)imageArray[i][j];
		}
		std::cout << std::endl;
	}
}

void addPadding(int kernelHalf, int paddingH, int paddingW, std::vector<std::vector<uint8_t>>& paddedArray, std::vector<std::vector<uint8_t>>& imageArray){
	for(int i=0;i<paddingH;i++){
		for(int j=0;j<paddingW;j++){
			if(i<kernelHalf || i>=(paddingH-kernelHalf) || j<kernelHalf || j>=(paddingW-kernelHalf)){
				paddedArray[i][j]=0;}
			else { paddedArray[i][j] = imageArray[i-kernelHalf][j-kernelHalf]; }
		}
	}
}

int convolutionOfAPixel(int kernelHalf, int locationA, int locationB, std::vector<std::vector<uint8_t>>& paddedArray, std::vector<std::vector<float>>& kernel){
	float sum = 0;
	for(int a=-kernelHalf;a<kernelHalf+1;a++){
		for(int b=-kernelHalf;b<kernelHalf+1;b++){
			sum += paddedArray[locationA+a][locationB+b] * kernel[a+kernelHalf][b+kernelHalf];
		}
	}
	return (int)sum;
}

void makeConvolution(int kernelHalf, int paddingStart, int paddingW, std::vector<std::vector<uint8_t>>& paddedArray, std::vector<std::vector<float>>& kernel, std::vector<std::vector<uint8_t>>& outputArray){
	for(int i=kernelHalf;i<paddingStart;i++){
		for(int j=kernelHalf;j<paddingW;j++){
			outputArray[i-kernelHalf][j-kernelHalf] = convolutionOfAPixel(kernelHalf, i, j, paddedArray, kernel);
		}
	}
}

int main(int argc, char* argv[]) {
	int kernelSize = atoi(argv[1]);	
	float kernelWeight = 1.0f / (kernelSize*kernelSize);
	std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize, kernelWeight));
	
	if(kernelSize==3){kernel =
        {{1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };}
	
	const cv::Mat image = cv::imread("huge.jpg", cv::IMREAD_GRAYSCALE);
    
	int kernelHalf = ((kernelSize-1)/2);
	int paddingH = image.rows + 2*kernelHalf;
	int paddingW = image.cols + 2*kernelHalf;

	std::vector<std::vector<uint8_t>> imageArray(image.rows, std::vector<uint8_t>(image.cols));
	std::vector<std::vector<uint8_t>> paddedArray(paddingH, std::vector<uint8_t>(paddingW, 0));
	std::vector<std::vector<uint8_t>> outputArray(image.rows, std::vector<uint8_t>(image.cols));
	    
	//imageToArray
	for(int i=0;i<image.rows;i++){
		const uint8_t* imageRowNumber = image.ptr<uint8_t>(i);
		for(int j=0;j<image.cols;j++){
			uint8_t pixel = imageRowNumber[j];
			imageArray[i][j] = pixel;
		}
	}
	
	addPadding(kernelHalf, paddingH, paddingW, paddedArray, imageArray);
		
	auto start = std::chrono::system_clock::now();
	
	makeConvolution(kernelHalf, paddingH-kernelHalf, paddingW-kernelHalf, paddedArray, kernel, outputArray);
	
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Zaman: " << elapsed_seconds.count() << "\n";

	//arrayToImage
	cv::Mat outputImage(image.rows, image.cols, CV_8UC1);
    for (int i = 0; i < image.rows; i++) {
        uint8_t* outputRow = outputImage.ptr<uint8_t>(i);
        for (int j = 0; j < image.cols; j++) {
            outputRow[j] = outputArray[i][j];
        }
    }

    cv::imshow("Emre", image);
    cv::imshow("Convoluted Emre", outputImage);
	cv::waitKey(0);
	
    return 0;
}

#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <ctime>

//Transpose ile
// ./yap23 image1.jpg image2.jpg

void printArray(std::vector<std::vector<float>>& imageArray){
	int sizeOfVectori = imageArray.size();
	int sizeOfVectorj = imageArray[0].size();
	
	for(int i=0; i<sizeOfVectori; i++){
		for(int j=0; j<sizeOfVectorj; j++){
			std::cout<< (int)imageArray[i][j];
		}
		std::cout << std::endl;
	}
}

void makeTranspose(std::vector<std::vector<float>>& imageArray, std::vector<std::vector<float>>& transposeArray){
	int rowCount = imageArray.size();
	int columnCount = imageArray[0].size();
	for(int i=0;i<rowCount;i++){
		for(int j=0;j<columnCount;j++){
			transposeArray[j][i] = imageArray[i][j];
		}
	}
}

float calculateOneCell(int rowNumberOne, int rowNumberTwo, std::vector<std::vector<float>>& arrayOne, std::vector<std::vector<float>>& arrayTwo){
	int columnCount = arrayOne[0].size();
	float sum = 0;
	for(int i=0;i<columnCount;i++){
			sum = sum + arrayOne[rowNumberOne][i] * arrayTwo[rowNumberTwo][i];
	}
	return (float) sum;
}

void multiplyMatrices(std::vector<std::vector<float>>& arrayOne, std::vector<std::vector<float>>& arrayTwo, std::vector<std::vector<float>>& outputArray){
	int rowCount = outputArray.size();
	int columnCount = outputArray[0].size();
	for(int a=0;a<rowCount;a++){
		for(int b=0;b<columnCount;b++){
			outputArray[a][b] = calculateOneCell(a,b,arrayOne, arrayTwo);
		}
	}
}

void imageToArray(const cv::Mat *image, std::vector<std::vector<float>>& imageArray){
	for(int i=0;i<(*image).rows;i++){
		const uint8_t* imageRowNumber = (*image).ptr<uint8_t>(i);
		for(int j=0;j<(*image).cols;j++){
			float pixel = static_cast<float>(imageRowNumber[j]);
			imageArray[i][j] = pixel;
		}
	}
}

int main(int argc, char* argv[]) {
	char* path1 = argv[1];
	char* path2 = argv[2];
	const cv::Mat imageOne = cv::imread(path1, cv::IMREAD_GRAYSCALE);
	const cv::Mat imageTwo = cv::imread(path2, cv::IMREAD_GRAYSCALE);
	
	if(imageOne.cols!=imageTwo.rows){
		std::cout << "Hatalı Boyutta Resimler";
		perror("AAAA");
	}
	
	std::vector<std::vector<float>> arrayOne(imageOne.rows, std::vector<float>(imageOne.cols));
	std::vector<std::vector<float>> arrayTwo(imageTwo.rows, std::vector<float>(imageTwo.cols));
	std::vector<std::vector<float>> transposeArray(imageTwo.cols, std::vector<float>(imageTwo.rows));
	std::vector<std::vector<float>> outputArray(imageOne.rows, std::vector<float>(imageTwo.cols));
	    
	imageToArray(&imageOne, arrayOne);
	imageToArray(&imageTwo, arrayTwo);
	
	makeTranspose(arrayTwo, transposeArray);
			
	auto start = std::chrono::system_clock::now();
	
	multiplyMatrices(arrayOne, transposeArray, outputArray);
	
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Zaman: " << elapsed_seconds.count() << "\n";

	//arrayToImage
	cv::Mat outputImage(imageOne.rows, imageTwo.cols, CV_32FC1);
    for (int i = 0; i < imageOne.rows; i++) {
        float* outputRow = outputImage.ptr<float>(i);
        for (int j = 0; j < imageTwo.cols; j++) {
            outputRow[j] = outputArray[i][j];
        }
    }
    
    cv::Mat displayImage;
    cv::normalize(outputImage, displayImage, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    cv::imshow("Emre1", imageOne);
    cv::imshow("Emre2", imageTwo);
    cv::imshow("Multiplied Emre", displayImage);
	cv::waitKey(0);
	
    return 0;
}

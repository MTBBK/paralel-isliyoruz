#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

//Transpose yok
// ./yap26 <BlockSize> <NumberOfThreads>

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

void multiplyMatrices(int startX, int endX, int Bl, std::vector<std::vector<float>>& arrayOne, std::vector<std::vector<float>>& arrayTwo, std::vector<std::vector<float>>& outputArray){
	int rowCount = outputArray.size();
	int columnCount = outputArray[0].size();
	int arrayOneColumnCount = arrayOne[0].size();
	for(int x=startX;x<endX;x=std::min(x+Bl,endX)){
		for(int y=0;y<columnCount;y=std::min(y+Bl,columnCount)){
			for(int z=0;z<arrayOneColumnCount;z=std::min(z+Bl,arrayOneColumnCount)){
				for(int i=0;i<std::min(Bl, rowCount - x);i++){
					for(int k=0;k<std::min(Bl, arrayOneColumnCount - z);k++){
						for(int j=0;j<std::min(Bl, columnCount - y);j++){
							outputArray[x+i][y+j] += arrayOne[x+i][z+k] * arrayTwo[z+k][y+j];
						}	
					}
				}
			}
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
	int blockSize = atoi(argv[1]);
	int numberOfThreads = atoi(argv[2]);
	
	/*
	char* path1 = argv[2];
	char* path2 = argv[3];
	*/

	const cv::Mat imageOne = cv::imread("dene2.jpg", cv::IMREAD_GRAYSCALE);
	const cv::Mat imageTwo = cv::imread("dene2.jpg", cv::IMREAD_GRAYSCALE);
	
	if(imageOne.cols!=imageTwo.rows){
		std::cout << "Hatalı Boyutta Resimler";
		perror("AAAA");
	}
		
	std::vector<std::vector<float>> arrayOne(imageOne.rows, std::vector<float>(imageOne.cols));
	std::vector<std::vector<float>> arrayTwo(imageTwo.rows, std::vector<float>(imageTwo.cols));
	std::vector<std::vector<float>> outputArray(imageOne.rows, std::vector<float>(imageTwo.cols, 0.0f));
	    
	imageToArray(&imageOne, arrayOne);
	imageToArray(&imageTwo, arrayTwo);
	
	auto start = std::chrono::system_clock::now();
	
	std::thread** threadArray = new std::thread* [numberOfThreads];

	int rowCount = outputArray.size();
	int rowsPerThread = (rowCount/blockSize) / numberOfThreads;

	for (int i = 0; i < numberOfThreads; i++)
	{
		int startX = i * rowsPerThread * blockSize;
		int endX = (i == numberOfThreads - 1) ? rowCount : (i + 1) * rowsPerThread * blockSize;
		
		threadArray[i] = new std::thread(multiplyMatrices, startX, endX, blockSize, std::ref(arrayOne), std::ref(arrayTwo), std::ref(outputArray));
	}

	for (int i = 0; i < numberOfThreads; i++) {
		threadArray[i]->join();
		delete threadArray[i];}

	delete[] threadArray;
		
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Blok Boyutu: " << blockSize << " Thread Sayısı: " << numberOfThreads << " Zaman: " << elapsed_seconds.count() << "\n";

	//arrayToImage
	cv::Mat outputImage(imageOne.rows, imageTwo.cols, CV_32FC1);
    for (int i = 0; i < imageOne.rows; i++) {
        float* outputRow = outputImage.ptr<float>(i);
        for (int j = 0; j < imageTwo.cols; j++) {
            outputRow[j] = outputArray[i][j];
        }
    }
    
    /*
    cv::Mat displayImage;
    cv::normalize(outputImage, displayImage, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    cv::imshow("Emre1", imageOne);
    cv::imshow("Emre2", imageTwo);
    cv::imshow("Multiplied Emre", displayImage);
	cv::waitKey(0);
	*/
	
    return 0;
}

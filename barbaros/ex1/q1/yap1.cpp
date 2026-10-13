#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <ctime>

void printHistogram(int histogramArray[256]){
  for(int i=0; i<256; i++)
    std::cout<< i << ": " << histogramArray[i] << std::endl;
}

void calculateHistogram (cv::Mat image, int histogramArray[256]){
	for(int i=0; i<image.rows; i++){
		for(int j=0; j<image.cols; j++){
			int pixel = image.at<uint8_t>(i,j);
			histogramArray[pixel]++;
		}
	}
}

int main() {
	auto start = std::chrono::system_clock::now();
	const cv::Mat image = cv::imread("lena_grayscale_hq.jpg", cv::IMREAD_GRAYSCALE);
    std::cout << "Emre Karaduman" << "\n";

	std::cout<<"Img width: "<< image.cols << std::endl;
	std::cout<<"Img height: "<< image.rows << std::endl;
	std::cout<<"Img number of channels: "<< image.channels() << std::endl;
	
	int histogramArray [256] = {0};
	calculateHistogram(image, histogramArray);
	printHistogram(histogramArray);
    
    
    
    auto end = std::chrono::system_clock::now();

    
    cv::imshow("Osman", image);
	cv::waitKey(0);
    
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Zaman: " << elapsed_seconds.count() << "\n";
    return 0;
}

#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <ctime>

void printT(double threadsArray[512]){
  for(int i=0; i<512; i++)
    std::cout<< i+1 << ": " << threadsArray[i] << std::endl;
}

void yap(const cv::Mat *image, std::array<std::atomic<std::size_t>, 256>& histogramArray, int size, int k)
{
	int max = size*(k+1) > 512 ? 512 : size*k;
	for(int i=size*(k-1); i<max; i++){
		for(int j=0; j<512; j++){
			int pixel = (*image).at<uint8_t>(i,j);
			histogramArray[pixel].fetch_add(1, std::memory_order_relaxed);
		}
	}
}

int main(int argc, char* argv[]) {
	//int numberOfThreads = atoi(argv[1]);
	
	// Full Test
	if (atoi(argv[1]) == 0){
		int testCount = atoi(argv[2]);
		double testArray [512][testCount];
		for(int a = 1; a<513; a++){
			int b = testCount;
			while(b>0){
				int numberOfThreads = a;
				
				auto start = std::chrono::system_clock::now();
				const cv::Mat image = cv::imread("lena_grayscale_hq.jpg", cv::IMREAD_GRAYSCALE);
				//std::cout << "Emre Karaduman" << "\n";

				std::array<std::atomic<std::size_t>, 256> histogramArray;
				for(auto&x:histogramArray) std::atomic_init(&x,std::size_t(0));	
				
					
				std::thread** threadArray = new std::thread* [numberOfThreads];

				for (int i = 0; i < numberOfThreads; i++)
				{
					threadArray[i] = new std::thread(yap, &image, std::ref(histogramArray), 512/numberOfThreads, i+1);
				}
				
				
				
				for (int i = 0; i < numberOfThreads; i++) {
					threadArray[i]->join();
					delete threadArray[i];}

				delete[] threadArray;
			   
				//printHistogram(histogramArray);

					
				auto end = std::chrono::system_clock::now();
				//cv::imshow("Osman", image);
				//cv::waitKey(0);
				std::chrono::duration<double> elapsed_seconds = end-start;
				
				testArray[a-1][testCount-b] = elapsed_seconds.count();
				b--;
			}
		}
		
		double threadTimes [512];
		double bestThreadTime = 1000;
		int bestThread = 0;
		for(int y=0; y<512; y++){
			double toplam = 0;
			for(int z=0; z<testCount; z++){
				toplam += testArray[y][z];	
			}
			double ortalama = toplam/testCount;
			if (ortalama < bestThreadTime){
				bestThreadTime = ortalama;
				bestThread = y+1;
			}
			threadTimes[y]=ortalama;
		}
		
		if(atoi(argv[3])==0){
			std::cout << "Zaman: " << bestThreadTime << " Emre: " << bestThread << "\n";
		}
		else if (atoi(argv[3]) == 1){
			printT(threadTimes);
		}
	}
	
	// Thread Select
	else if(atoi(argv[1]) == 1){
				int numberOfThreads = atoi(argv[2]);
				
				auto start = std::chrono::system_clock::now();
				const cv::Mat image = cv::imread("lena_grayscale_hq.jpg", cv::IMREAD_GRAYSCALE);
				//std::cout << "Emre Karaduman" << "\n";

				std::array<std::atomic<std::size_t>, 256> histogramArray;
				for(auto&x:histogramArray) std::atomic_init(&x,std::size_t(0));	

				std::thread** threadArray = new std::thread* [numberOfThreads];

				for (int i = 0; i < numberOfThreads; i++)
				{
					threadArray[i] = new std::thread(yap, &image, std::ref(histogramArray), 512/numberOfThreads, i+1);
				}
				
				
				
				for (int i = 0; i < numberOfThreads; i++) {
					threadArray[i]->join();
					delete threadArray[i];}

				delete[] threadArray;
			   
				//printHistogram(histogramArray);

					
				auto end = std::chrono::system_clock::now();
				//cv::imshow("Osman", image);
				//cv::waitKey(0);
				std::chrono::duration<double> elapsed_seconds = end-start;
				
				std::cout << "Zaman: " << elapsed_seconds.count() << " Emre: " << numberOfThreads << "\n";
	}


    
    return 0;
}

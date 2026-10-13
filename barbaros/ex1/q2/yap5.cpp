#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>

//./yap5 numberOfThreads doPrint
void generateRandomArray(int* arr, int size, int min_val = 1, int max_val = 1000) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(46); 
        seeded = true;
    }
    for(int i = 0; i < size; i++) {
        arr[i] = min_val + (std::rand() % (max_val - min_val + 1));
    }
}

void printT(int ** integralArray, int numberOfArrays, int size){
    for (int i = 0; i < numberOfArrays; ++i) {
        std::cout << "Array " << i + 1 << " integral: [";
        for (int j = 0; j < size; ++j) {
            std::cout << integralArray[i][j] << (j + 1 < size ? ", " : "");
        }
        std::cout << "]\n";
    }
}

void yap (int start, int end, int ** src, int ** dest, int size){
    for(int i = start; i < end; i++){
        dest[i][0] = src[i][0];
        for(int j = 1; j < size; j++){
            dest[i][j] = dest[i][j-1] + src[i][j];
        }
    }
}

int main(int argc, char* argv[]) {
	int numberOfThreads = atoi(argv[1]);
    //std::cout << "Emre Karaduman" << "\n";

	const int numberOfArrays = 1000;
	const int size = 65536;
	
	int ** arrayArray = new int*[numberOfArrays];
	int ** integralArray = new int*[numberOfArrays];	
	// rastgele array oluşturma
	for(int i=0;i<numberOfArrays;i++){
		arrayArray[i] = new int[size];
		integralArray[i] = new int[size];
        generateRandomArray(arrayArray[i], size);
	}
	
	//Başlangıç
	auto start = std::chrono::system_clock::now();
	
	std::thread** threadArray = new std::thread* [numberOfThreads];

	for (int i = 0; i < numberOfThreads; i++){
		int start_idx = (i * numberOfArrays) / numberOfThreads;
		int end_idx = ((i + 1) * numberOfArrays) / numberOfThreads;
		
		threadArray[i] = new std::thread(yap, start_idx, end_idx, arrayArray, integralArray, size);
	}
	
	for (int i = 0; i < numberOfThreads; i++) {
		threadArray[i]->join();
		delete threadArray[i];}

	delete[] threadArray;
    
    auto end = std::chrono::system_clock::now();

	if(atoi(argv[2])==1){printT(integralArray, numberOfArrays, size);}

    for (int i = 0; i < numberOfArrays; ++i) {
        delete[] arrayArray[i];
        delete[] integralArray[i];
    }
    delete[] arrayArray;
    delete[] integralArray;
    
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Zaman: " << elapsed_seconds.count() << "\n";
    return 0;
}

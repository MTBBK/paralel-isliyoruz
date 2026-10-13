#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <ctime>
#include <array>
#include <cstdlib>

//./yap6 numberOfThreads doPrint
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

void printT(int ** integralArray, int numberOfArrays, const int size){
    for (int i = 0; i < numberOfArrays; ++i) {
        std::cout << "Array " << i + 1 << " integral: [";
        for (int j = 0; j < size; ++j) {
            std::cout << integralArray[i][j] << (j + 1 < size ? ", " : "");
        }
        std::cout << "]\n";
    }
}

void yap (int start, int end, int ** src, int ** dest, std::atomic<std::int32_t>* atomInt, std::atomic<std::size_t>* atomBool, int i, int l, int numberOfThreads){
	while(true){
		if(l==0){
			dest[0][start] = src[0][start];
			for(int j = start+1; j < end; j++){
				dest[0][j] = dest[0][j-1] + src[0][j];
			}
			atomInt[i].fetch_add(dest[0][end-1], std::memory_order_relaxed);
			atomBool[i].fetch_add(1, std::memory_order_relaxed);
			l=2;
		}
		else if(l==1){
			int b = 0;
			for(int j = 0; j < i; j++){
				b = b+atomInt[j];
			}

			for(int j = start; j < end; j++){
				dest[0][j] = dest[0][j] + b;
			}
			break;
		}
		
		bool flag = true;
		for(int i=0;i<numberOfThreads;i++){
			if(atomBool[i]==0){
			flag = false;
			}
		}
		if(flag){l=1;}
	}
}

int main(int argc, char* argv[]) {
	int numberOfThreads = atoi(argv[1]);
    //std::cout << "Emre Karaduman" << "\n";

	const int numberOfArrays = 1000;
	//const int size = atoi(argv[3]);
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
	
	std::atomic<std::int32_t>* atomInt = new std::atomic<std::int32_t>[numberOfThreads];
	std::atomic<std::size_t>* atomBool = new std::atomic<std::size_t>[numberOfThreads];
	
	for(int i = 0; i < numberOfThreads; i++) {
		std::atomic_init(&atomInt[i], 0);	
		std::atomic_init(&atomBool[i], std::size_t(0));	
	}
				
	std::thread** threadArray = new std::thread* [numberOfThreads];

	for (int i = 0; i < numberOfThreads; i++){
		int start_idx = (i * size) / numberOfThreads;
		int end_idx = ((i + 1) * size) / numberOfThreads;
		
		threadArray[i] = new std::thread(yap, start_idx, end_idx, arrayArray, integralArray, atomInt, atomBool, i, 0, numberOfThreads);
	}
			
	for (int i = 0; i < numberOfThreads; i++) {
		threadArray[i]->join();
		delete threadArray[i];
	}

	delete[] threadArray;
    
    auto end = std::chrono::system_clock::now();

	if(atoi(argv[2])==1){printT(integralArray, numberOfArrays, size);}

    for (int i = 0; i < numberOfArrays; ++i) {
        delete[] arrayArray[i];
        delete[] integralArray[i];
    }
    delete[] arrayArray;
    delete[] integralArray;
    delete[] atomInt;
	delete[] atomBool;
	
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Zaman: " << elapsed_seconds.count() << "\n";
    return 0;
}

#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>

//./yap4 doPrint
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

void calculateArray (int * src, int * dest, int size){
	dest[0] = src[0];
	for(int i=1;i<size;i++){
		dest[i]=dest[i-1]+src[i];
	}		
}

int main(int argc, char* argv[]) {
    //std::cout << "Emre Karaduman" << "\n";

	int numberOfArrays = 1000;
	int size = {65536};

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
	
	for(int i=0;i<numberOfArrays;i++){
		calculateArray(arrayArray[i], integralArray[i], size);
	}
    
    auto end = std::chrono::system_clock::now();

	if(atoi(argv[1])==1){printT(integralArray, numberOfArrays, size);}

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

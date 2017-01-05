#include <iostream>
#include <ctime>
#include "MuskMemoryManager.hpp"

using namespace std;

/***************************** WARNING ********************************/
/* The default make command may not work with this -- remove all optimization flags */

int main(){
    
    std::clock_t start, stop;
    
    start = std::clock();
    
    MuskMemoryManager& mm = MuskMemoryManager::getInstance();
    void* ptr1 = NULL;
    
    for (unsigned long i = 0; i < 20000; ++i) {
        ptr1 = (int*) mm.mm_allocate(sizeof(int));
    }
    
    stop = std::clock();
    
    std::cout << "MuskMemoryManager Time: " << (stop - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    start = std::clock();
    
    for (unsigned long i = 0; i < 20000; ++i) {
        ptr1 = (int*) malloc(sizeof(int));
    }
    
    stop = std::clock();

    std::cout << "malloc Time: " << (stop - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    return 0;
}

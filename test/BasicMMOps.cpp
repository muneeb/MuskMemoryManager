#include "MuskMemoryManager.hpp"

int main(){
    
    MuskMemoryManager& mm = MuskMemoryManager::getInstance();
    
    void* ptr1 = (void*) mm.mm_allocate(5 * sizeof(int));
    
    void* ptr2 = (void*) mm.mm_allocate(5 * sizeof(int));
    
    cout << "ptr1 " << ptr1 << endl;
    cout << "ptr2 " << ptr2 << endl;
    
    mm.displayInfo();
    
    mm.mm_free(ptr1);
    mm.mm_free(ptr2);
    
    return 0;
}

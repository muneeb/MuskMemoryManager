#include <iostream>
#include "MuskMemoryManager.hpp"

using namespace std;


int main(){
    
    int rows = 10, cols = 10;
    
    MuskMemoryManager& mm = MuskMemoryManager::getInstance();
    
    float* m_a = (float*) mm.mm_allocate( rows * cols * sizeof(float));
    float* m_b = (float*) mm.mm_allocate( rows * cols * sizeof(float));
    float* m_c = (float*) mm.mm_allocate( rows * cols * sizeof(float));

    float sum = 0;
    
    for(int i = 0; i < (rows*cols); ++i)
    	m_a[i] = m_b[i] = 3.14;
    
    //Multiplication Logic
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < cols; k++)
                m_c[i * rows + j] += m_a[i * rows + k] * m_b[k * rows + j];
            sum += m_c[i * rows + j];
        }
    }
    
    //mm.displayInfo();
    
    mm.mm_free(m_a);
    mm.mm_free(m_b);
    mm.mm_free(m_c);
    
    cout << "sum: " << sum << endl;
    
    m_a = (float*) malloc( rows * cols * sizeof(float));
    m_b = (float*) malloc( rows * cols * sizeof(float));
    m_c = (float*) malloc( rows * cols * sizeof(float));
    
    sum = 0;
    
    for(int i = 0; i < (rows*cols); ++i)
    m_a[i] = m_b[i] = 3.14;
    
    //Multiplication Logic
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < cols; k++)
            m_c[i * rows + j] += m_a[i * rows + k] * m_b[k * rows + j];
            sum += m_c[i * rows + j];
        }
    }
    
    free(m_a);
    free(m_b);
    free(m_c);
    
    cout << "sum: " << sum << endl;
    
    return 0;
    
}

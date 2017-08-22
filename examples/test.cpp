#include "approx_counter.h"
#include <iostream>

using namespace std;

int main(){
    // Declare the counter as an integer counter
    approx_counter<int> ctr;
    
    int incr_count = 64;
    for (int i =0; i < incr_count; i++)
    {
        ++ctr; // Increment the counter.
    }

    // Print the counter value. 
    // Use the () operator for accessing the value.
    cout << "Counter value is " << ctr() << endl;
    return 0;
}

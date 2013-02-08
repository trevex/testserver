/*
 (c) Niklas K. Voss
*/

#include "ivy/routine.hpp"

#include <iostream>
#include <thread>


int test(void)
{
    return -18;
}

int main(void)
{
    using namespace std;
    
    cout << "ivy - Routine/Actor MT Tests" << endl;

    cout << "Queue is ";
    if (!ivy::mt::isLockFreeQueue())
        cout << "not ";
    cout << "lockfree" << endl;

    std::thread t0(ivy::RT);
    std::thread t1(ivy::RT);

    int i = -12;
    int j = -14;
    ivy::CR([&]()->int { return i; });
    ivy::CR([&]()->int { 
        int k = j+1;
        ivy::CR([=]()->int {
            return k+3;
        });
        return k;
    });
    ivy::CR(&test);


    std::chrono::milliseconds duration(500);
    std::this_thread::sleep_for(duration);

    ivy::mt::stop();

    t0.join();
    t1.join();

    cout << "Tests done!" << endl; 

    cout << "There were still " << ivy::mt::flush() << " routines queued." << endl;
    
    return EXIT_SUCCESS;
}   

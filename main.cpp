/*
 * (c) Niklas K. Voss
 */


#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "test.hpp"

TQueue<std::string, 5> queue;

int test(void)
{
    std::string* my_item = new std::string("TEST!");

    int i = 1000000;
    while(i--) queue.enqueue(my_item);

    return 0;
}

int main(void)
{
    std::cout << "Actor Test v0.1" << std::endl;
    std::vector<std::thread*> t;
    for (int i = 0; i < 2; i++) t.push_back(new std::thread(test));
    for (auto it = t.begin(); it != t.end(); it++) (*it)->join();
    std::cout << "Test done..." << std::endl; 
    return EXIT_SUCCESS;
}   
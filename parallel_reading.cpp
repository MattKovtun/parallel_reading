//
//
//
// Created by matt on 4/3/17.
//
#include <atomic>
#include <condition_variable>

#include <boost/algorithm/string/replace.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <thread>
#include <mutex>
#include <sstream>


using namespace std;
//g++ read.cpp -pthread -std=c++11

map<string, int> m;
mutex myMutex;
condition_variable cv;
deque<string> dq;
atomic<bool> done {false};
//bool done = false;

void printMap(const map<string, int> &m) {
    for (auto elem : m) {
        cout << elem.first << " " << elem.second << "\n";
    }
}


void producer(string path) {
    ifstream myfile;
    string word;
    myfile.open(path);
    if (!myfile.is_open()) {
        cerr << "Error" << endl;
        return;
    }

    while (myfile >> word) {
        lock_guard<mutex> lg(mutex);
        dq.push_back(word);
        cv.notify_one();

    }
    done = true;
    myfile.close();
    return;
}

void write_to_file(const map<string, int> &m, string path) {
    ofstream myfile;
    myfile.open(path);
    for (auto elem : m) {
        myfile << elem.first << "    " << elem.second << "\n";
    }
    myfile.close();
}

void consumer() {
    unique_lock<mutex> ul(myMutex);
    while (!done) {
        if (!dq.empty()) {
            string w = dq.front();
            dq.pop_front();
            ul.unlock();
            ++m[w];
            ul.lock();
        } else {
            cv.wait(ul);
        }
    }

}


int main() {


//    producer("/home/matt/CLionProjects/ConCurrency/example.txt");


    thread thread1 = thread(producer, "/home/matt/CLionProjects/ConCurrency/shakespear.txt");
    thread thread2 = thread(consumer);
    thread2.join();
    thread1.join();
    write_to_file(m, "/home/matt/CLionProjects/ConCurrency/parallel_res.txt");
    return 0;
}









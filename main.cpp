#include "simple_time_stamp.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>


using namespace std;
//g++ read.cpp -pthread -std=c++11

map<string, int> m;
mutex myMutex;

void printMap(const map<string, int> &m) {
    for (auto elem : m) {
        cout << elem.first << " " << elem.second << "\n";
    }
}


vector<string> open_read(string path) {
    ifstream myfile;
    vector<string> words;
    string word;
    myfile.open(path);
    if (!myfile.is_open()) {
        cerr << "Error" << endl;
        return words;
    }
    string formated_word;
    while (myfile >> word) {
        formated_word = word;  //format_word(word);
        words.push_back(formated_word);
        //        ++checkM[word];       // check map with only main thread
    }
    myfile.close();
    return words;
}

void write_to_file(const map<string, int> &m, string path) {
    ofstream myfile;
    myfile.open(path);
    for (auto elem : m) {
        myfile << elem.first << "    " << elem.second << "\n";
    }
    myfile.close();
}

void worker(int l, int r, const vector<string> &words) {
    map<string, int> mp;
    for (; l <= r; ++l) {
        ++mp[words[l]];
    }
    {
        lock_guard<mutex> lg(myMutex);
        for (auto w: mp) {
            m[w.first] += w.second;
        }
    }

}


int main(int argc, char *argv[]) {  // input_file, threads, output_file
    vector<string> words;
    auto stage1_start_time = get_current_time_fenced();
    if (!argv[1]) words = open_read("/home/matt/CLionProjects/ConCurrency/example.txt");
    else words = open_read(argv[1]);

    auto stage2_start_time = get_current_time_fenced();

    vector<thread> rthreads;
    istringstream ss(argv[2]);
    int x;
    ss >> x;
    cout << "Spawning " << x << " workers" << endl;
    for (int i = 0; i < x; ++i) {
        int a = (words.size()) / x * i;
        int b = (words.size()) / x * (i + 1);
//        cout << (i + 1) << " interval from " << a << " to " << b - 1 << " word" << endl;
        rthreads.push_back(thread(worker, a, b - 1, cref(words)));


    }
    for (int i = 0; i < x; ++i) {
        rthreads[i].join();
    }

//    printMap(checkM);
//    cout << "==================================" << endl;
    auto finish_time = get_current_time_fenced();

    auto total_time = finish_time - stage1_start_time;
    auto stage1_time = stage2_start_time - stage1_start_time;
    auto stage2_time = finish_time - stage2_start_time;
//    printMap(m);
    cout << "==================================" << endl;
    cout << "Total time: " << to_us(total_time) << endl;
    cout << "Stage 1 time: " << to_us(stage1_time) << endl;
    cout << "Stage 2 time: " << to_us(stage2_time) << endl;
    cout << "==================================" << endl;
    write_to_file(m, argv[3]);
    return 0;
}


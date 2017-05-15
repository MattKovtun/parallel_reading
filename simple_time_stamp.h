#include <chrono>
#include <atomic>


using namespace std;


//
// Created by matt on 3/27/17.
//

#ifndef CONCURRENCY_SIMPLE_TIME_STAMP_H
#define CONCURRENCY_SIMPLE_TIME_STAMP_H

#endif //CONCURRENCY_SIMPLE_TIME_STAMP_H


inline chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    atomic_thread_fence(memory_order_seq_cst);
    auto res_time = chrono::high_resolution_clock::now();
    atomic_thread_fence(memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
    return chrono::duration_cast<chrono::microseconds>(d).count();
}
//
// Created by angel on 2022-05-04.
//

#ifndef COMPETITION_RADIX_SORT_H
#define COMPETITION_RADIX_SORT_H

#include <algorithm>
#include <future>
#include <map>
#include <thread>
#define MAX_THREAD 4

using plate_t = std::string;
using price_t = float;
using galaxy_t = double;
std::mutex mtx;

template <typename T>
std::vector<T> counting_sort(std::vector<T> vec, const size_t& k, int count[], const int& n, const int& min){
    std::vector<T> output(vec.size());

    for(int i = 0; i < vec.size(); i++){
        count[vec[i][k] - min + 1]++;
    }
    for(int i = 1; i < n; i++){
        count[i] += count[i - 1];
    }

    for (int i = 0; i < vec.size(); i++) {
        output[count[vec[i][k] - min]] = vec[i];
        count[vec[i][k] - min]++;
    }

    return output;
}

std::vector<price_t> countingSort(std::vector<price_t> prices, int place) {
    const int max = 10;
    std::vector<price_t> output(prices.size());
    int count[max];

    for (int i = 0; i < max; ++i) {
        count[i] = 0;
    }

    // Calculate count of elements
    for (int i = 0; i < prices.size(); i++) {
        count[(int) (prices[i] / place) % 10]++;
    }

    // Calculate cumulative count
    for (int i = 1; i < max; i++) {
        count[i] += count[i - 1];
    }

    // Place the elements in sorted order
    for (int i = prices.size() - 1; i >= 0; i--) {
        output[count[(int)(prices[i] / place) % 10] - 1] = prices[i];
        count[(int)(prices[i] / place) % 10]--;
    }

    return output;
}

template <typename T>
std::vector<T> radix_sort(std::vector<T> vec){
    return vec;
}

template <>
std::vector<price_t> radix_sort<price_t>(std::vector<price_t> vec){
    int max = (int)*std::max_element(vec.begin(), vec.end());
    for (int place = 1; max / place > 0; place *= 10) {
        vec = countingSort(vec, place);
    }
    return vec;
}

struct result{
    result(std::vector<int> c, int m, int p):count(std::move(c)), min(m), pos(p){}
    std::vector<int> count;
    int min;
    int pos;
};


result radix_counter(std::vector<plate_t> vec, int k){
    int limit, min;
    if(vec[0][k] > '0' - 1 && vec[0][k] < '9' + 1){
        limit = 11;
        min = (int)'0';
    }else{
        limit = 27;
        min = (int)'A';
    }

    std::vector<int> count(limit);
    for(int i = 0; i < vec.size(); i++){
        count[(int)vec[i][k] + 1 - min]++;
    }
    for(int i = 1; i < limit; i++){
        count[i] += count[i - 1];
    }
    return result{count, min, k};
}

template <>
std::vector<plate_t> radix_sort<plate_t>(std::vector<plate_t> vec){
    auto max = *std::max_element(vec.begin(), vec.end());
    std::vector<plate_t> output(vec.size());

    for(int i = (int)max.size() - 1; i >= 0; i--){
        auto r = radix_counter(vec, i);

        for(int p = 0; p < vec.size(); p++) {
            output[r.count[(int) vec[p][r.pos] - r.min]++] = vec[p];
        }
        std::copy(__pstl::execution::par, output.begin(), output.end(), vec.begin());
        //vec = read_element(std::move(vec), i);
    }
    return vec;
}

template <typename T>
std::vector<T> async_radix_sort(std::vector<T> vec){
    return vec;
}

template <>
std::vector<plate_t> async_radix_sort<plate_t>(std::vector<plate_t> vec){
    auto max = *std::max_element(vec.begin(), vec.end());

    std::vector<std::future<result>> futures;
    for(int i = (int)max.size() - 1; i >= 0; i--){
        futures.push_back(std::async(std::launch::async, radix_counter, vec, i));
    }
    std::vector<result> counting_result;
    for(auto& fur : futures){
        counting_result.emplace_back(fur.get());
    }
/*
    std::vector<plate_t> output(vec.size());
    for(auto k : counting_result){
        for (int i = 0; i < vec.size(); i++) {
            output[k.count[(int) vec[i][k.pos] - k.min]] = vec[i];
            k.count[(int)vec[i][k.pos] - k.min]++;
        }
        vec = output;
    }*/
    return vec;
}

template <>
std::vector<price_t> async_radix_sort<price_t>(std::vector<price_t> vec){
    return vec;
}

template <typename T>
void parallel_radix_sort(std::vector<T>& vec){
}

auto c(std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator end, int k, int size){
    int min;
    if((*first)[k] > '0' - 1 && (*first)[k] < '9' + 1){
        min = (int)'0';
    }else{
        min = (int)'A';
    }

    int histogram[27] = {0};
    for (int i = 0; i < size; i++) {
        histogram[(*(first + i))[k] - min + 1]++;
    }

    for (int i = 1; i < 27; i++) {
        histogram[i] += histogram[i - 1];
    }
}


std::vector<plate_t>::iterator parallell_count(std::vector<int>& count, std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator last, int i, int min){
    for (; first != last; first++){
        count[(*first)[i] - min + 1]++;
    }
    return last;
}

std::vector<int> run_threads(std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator last, int i, int min){
    auto n = std::distance(first, last) / MAX_THREAD;
    std::vector<int> count(27);
    std::vector<std::future<std::vector<plate_t>::iterator>> futures;

    futures.push_back(std::async(parallell_count, std::ref(count), first, first + n, i, min));
    futures.push_back(std::async( parallell_count, std::ref(count), first + n, first + 2*n, i, min));
    futures.push_back(std::async(parallell_count, std::ref(count), first + 2*n, first + 3*n, i, min));
    futures.push_back(std::async( parallell_count, std::ref(count), first + 3*n, last, i, min));

    for(auto& f : futures){
        f.get();
    }
    return count;
    /*
    threads.push_back(std::thread(parallell_count, std::ref(count), first, first + n, i, min));
    threads.push_back(std::thread(parallell_count, std::ref(count), first + n, first + 2*n, i, min));
    threads.push_back(std::thread(parallell_count, std::ref(count), first + 2*n, first + 3*n, i, min));
    threads.push_back(std::thread(parallell_count, std::ref(count), first + 3*n, last, i, min));

    for(auto& t : threads){
        t.join();
    }*/

    /*
    std::vector<std::thread> threads;
    threads.push_back(std::thread(parallell_count, std::ref(count), first, first + n, i, min));
    threads.push_back(std::thread(parallell_count, std::ref(count), first + n, first + 2*n, i, min));
    threads.push_back(std::thread(parallell_count, std::ref(count), first + 2*n, first + 3*n, i, min));
    threads.push_back(std::thread(parallell_count, std::ref(count), first + 3*n, last, i, min));

    for(auto& t : threads){
        t.join();
    }*/
}

template<>
void parallel_radix_sort<plate_t>(std::vector<plate_t>& vec){
    auto max = *std::max_element(vec.begin(), vec.end());


    for(int n = max.size() - 1; n >= 0; n--) {

        int min = (vec[0][n] >= '0' && vec[0][n] <= '9' ? (int) '0' : (int) 'A');

        std::vector<int> count = run_threads( vec.begin(), vec.end(), n, min);

        for (int i = 1; i < 27; i++) {
            count[i] += count[i - 1];
        }

        std::vector<plate_t> output(vec.size());
/*
        for (int i = 0; i < vec.size(); i++) {
            output[count[(int)vec[i][n] - min]] = vec[i];
            count[(int)vec[i][n] - min]++;
        }*/
        std::copy(output.begin(), output.end(), vec.begin());
        //vec = output;
    }
}


#endif //COMPETITION_RADIX_SORT_H
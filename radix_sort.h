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
std::vector<T> radix_sort(std::vector<T>& vec){
    return vec;
}

template <>
std::vector<price_t> radix_sort<price_t>(std::vector<price_t>& vec){
    int max = (int)*std::max_element(vec.begin(), vec.end());
    for (int place = 1; max / place > 0; place *= 10) {
        vec = countingSort(vec, place);
    }
    return vec;
}

//------------------------------------RADIX SORT PLATES
void lsd_sort(std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator last, int n){
    if(last <= first){ return; }

    int min = ((*first)[n] >= '0' && (*first)[n] <= '9' ? (int) '0' : (int) 'A');

    int count[27] = { 0 };

    for (auto it = first; it != last; it++){
        count[(*it)[n] - (int)min + 1]++;
    }

    for (int i = 1; i < 27; i++) {
        count[i] += count[i - 1];
    }

    std::vector<plate_t> output(std::distance(first, last));

    for (auto it = first; it != last; it++) {
        output[count[(int)(*it)[n] - min]] = *(it);
        count[(int)(*it)[n] - min]++;
    }

    for (int i = 0; first != last; first++) {
        *first = output[i++];
    }
}

template <>
std::vector<plate_t> radix_sort<plate_t>(std::vector<plate_t>& vec){
    auto max = *std::max_element(vec.begin(), vec.end());

    for(int pos = max.size() - 1; pos >= 0; pos--) {
        lsd_sort(vec.begin(), vec.end(), pos);
    }

    return vec;
}


//------------------------------------RADIX SORT ASYNC
template <typename T>
std::vector<T> async_radix_sort(std::vector<T>& vec){
    return vec;
}

struct result_s{
    result_s(std::vector<int> c, int m, int p):count(std::move(c)), min(m), pos(p){}
    std::vector<int> count;
    int min;
    int pos;
};

result_s radix_counter(std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator last, int8_t p){
    int8_t limit, min;

    if((*first)[p] >= '0' && (*first)[p] <= '9'){
        limit = 11;
        min = (int8_t)'0';
    }else{
        limit = 27;
        min = (int8_t)'A';
    }

    std::vector<int> count(limit);
    for (auto it = first; it != last; it++){
        count[(*it)[p] - (int8_t)min + 1]++;
    }

    for(int8_t i = 1; i < limit; i++){
        count[i] += count[i - 1];
    }

    return result_s{count, min, p};
}

template <>
std::vector<plate_t> async_radix_sort<plate_t>(std::vector<plate_t>& vec){
    auto max = *std::max_element(vec.begin(), vec.end());

    int8_t pos = max.size() - 1;

    std::future<result_s> futures[max.size()];
    for(int8_t i = 0; i < max.size(); i++){
        futures[i] = std::async(std::launch::async, radix_counter, vec.begin(), vec.end(), pos--);
    }

    std::vector<result_s> counting_result;
    for(auto& fur : futures){
        counting_result.emplace_back(fur.get());
    }

    std::vector<plate_t> output(vec.size());
    for(auto result : counting_result){
        for (int i = 0; i < vec.size(); i++) {
            output[result.count[(int8_t)vec[i][result.pos] - result.min]] = vec[i];
            result.count[(int8_t)vec[i][result.pos] - result.min]++;
        }
        vec = output;
    }

    return vec;
}

template <>
std::vector<price_t> async_radix_sort<price_t>(std::vector<price_t>& vec){
    return vec;
}


//------------------------------------THREADED RADIX SORT
void parallell_count(std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator last, int n, int min, int size){
    int count[27] = { 0 };

    for (auto it = first; it != last; it++){
        count[(*it)[n] - (int)min + 1]++;
    }

    for (int i = 1; i < 27; i++) {
        count[i] += count[i - 1];
    }

    std::vector<plate_t> output(size);

    for (auto it = first; it != last; it++) {
        output[count[(int)(*it)[n] - min]] = *(it);
        count[(int)(*it)[n] - min]++;
    }

    for (int i = 0; first != last; first++) {
        *first = output[i++];
    }
}

void merge(std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator middle, std::vector<plate_t>::iterator last, int pos){
    std::inplace_merge(first, middle, last,
                       [pos](const std::string &s1, const std::string &s2) { return s1[pos] < s2[pos]; });
}

void run_threads(std::vector<plate_t>::iterator first, std::vector<plate_t>::iterator last, int pos){
    int min = ((*first)[pos] >= '0' && (*first)[pos] <= '9' ? (int) '0' : (int) 'A');
    auto n = std::distance(first, last) / MAX_THREAD;

    std::thread threads[MAX_THREAD];
    for(int i = 0; i < MAX_THREAD; i++){
        threads[i] = std::thread(parallell_count, first + (i * n), first + ((i + 1) * n), pos, min, n);
    }

    for(auto& t : threads){
        t.join();
    }

    std::thread t1 = std::thread(merge, first, first + n, first + (2 * n), pos);
    std::thread t2 = std::thread(merge, first + (2 * n), first + (3 * n), last, pos);
    t1.join();
    t2.join();
    std::thread t3 = std::thread(merge, first, first + (2 * n), last, pos);
    t3.join();
}

template <typename T>
void parallel_radix_sort(std::vector<T>& vec){
}

template<>
void parallel_radix_sort<plate_t>(std::vector<plate_t>& vec){
    auto max = *std::max_element(vec.begin(), vec.end());

    for(int n = max.size() - 1; n >= 0; n--){
        run_threads( vec.begin(), vec.end(), n);
    }
}


#endif //COMPETITION_RADIX_SORT_H
//
// Created by angel on 2022-05-04.
//

#ifndef COMPETITION_RADIX_SORT_H
#define COMPETITION_RADIX_SORT_H

#define MAX_THREAD 4
#include <algorithm>
#include <future>
#include <valarray>

using plate_t = std::string;
using price_t = float;
using galaxy_t = double;


//------------------------------------RADIX SORT PRICES
void lsd_sort(std::vector<price_t>::iterator first, std::vector<price_t>::iterator last, int n){
    int factor = std::pow(10, 2);
    size_t size = std::distance(first, last);

    int count[10] = { 0 };
    for (auto it = first; it != last; it++) {
        count[int(((*it) * factor) / n) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    std::vector<price_t> output(size);

    for (auto it = last - 1; it != first - 1; it--) {
        output[count[(int(((*it) * factor) / n) % 10)] - 1] = *it;
        count[(int(((*it) * factor) / n) % 10)]--;
    }

    for (int i = 0; first != last; first++) {
        *first = output[i++];
    }
}

template <typename T>
std::vector<T> radix_sort(std::vector<T>& vec){
    return vec;
}

template <>
std::vector<price_t> radix_sort<price_t>(std::vector<price_t>& vec){
    price_t max = (price_t)*std::max_element(vec.begin(), vec.end());

    for (int p = 1; (int)(max * 100/ p) > 0; p *= 10) {
        lsd_sort(vec.begin(), vec.end(), p);
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
        output[count[(int)(*it)[n] - min]] = *it;
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


//-----------------------------ASYNCHRONOUS RADIX SORT
std::pair<std::vector<int>, int> async_counter(std::vector<plate_t>& vec, int k){
    std::vector<int> count(255);
    for(int i = 0; i < vec.size(); i++){
        count[(int)vec[i][k]]++;
    }
    for(int i = 0; i < 254; i++){
        count[i + 1] += count[i];
    }
    return std::make_pair(count, k);
}

template <typename T>
std::vector<T> async_radix_sort(std::vector<T>& vec){
    return vec;
}

template <>
std::vector<plate_t> async_radix_sort<plate_t>(std::vector<plate_t>& vec){
    auto max = *std::max_element(vec.begin(), vec.end());

    std::vector<std::future<std::pair<std::vector<int>, int>>> futures;
    for(int8_t i = (int)max.size() - 1; i >= 0; i--){
        futures.push_back(std::async(std::launch::async, async_counter, std::ref(vec), i));
    }

    std::vector<std::pair<std::vector<int>, int>> counting_result;
    for(auto& fur : futures){
        counting_result.push_back(fur.get());
    }

    std::vector<plate_t> output(vec.size());
    for(auto k : counting_result){
        for(int i = vec.size() - 1; i >= 0; i--) {
            output[k.first[(int) vec[i][k.second]] - 1] = vec[i];
            k.first[(int)vec[i][k.second]]--;
        }
        std::copy(output.begin(), output.end(), vec.begin());
    }
    return vec;
}

//------------------------------------ASYNC RADIX SORT PRICES
std::pair<std::vector<int>, int> async_price_counter(std::vector<price_t>::iterator first,
                                                     std::vector<price_t>::iterator last, int k){
    int factor = std::pow(10, 2);

    std::vector<int> count(10);
    for (auto it = first; it != last; it++) {
        count[int(((*it) * factor) / k) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    return std::make_pair(count, k);
}

template <>
std::vector<price_t> async_radix_sort<price_t>(std::vector<price_t>& vec){
    auto max = *std::max_element(vec.begin(), vec.end());

    std::vector<std::future<std::pair<std::vector<int>, int>>> futures;
    for(int p = 1; (int)(max * 100/ p) > 0; p *= 10){
        futures.push_back(std::async(std::launch::async, async_price_counter, vec.begin(), vec.end(), p));
    }

    std::vector<std::pair<std::vector<int>, int>> counting_result;
    for(auto& fur : futures){
        counting_result.push_back(fur.get());
    }

    auto factor = std::pow(10,2);
    std::vector<price_t> output(vec.size());

    for(auto k : counting_result){
        for (auto it = vec.end() - 1; it != vec.begin() - 1; it--) {
            output[k.first[(int(((*it) * factor) / k.second) % 10)] - 1] = *it;
            k.first[(int(((*it) * factor) / k.second) % 10)]--;
        }
     //   std::copy(output.begin(), output.end(), vec.begin());
    }
    return vec;
}

//-----------------------------THREADED RADIX SORT
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

template<typename T>
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
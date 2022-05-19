//
// Created by angel on 2022-05-04.
//

#ifndef COMPETITION_RADIX_SORT_H
#define COMPETITION_RADIX_SORT_H

#include <algorithm>
#include <future>

using plate_t = std::string;
using price_t = float;
using galaxy_t = double;

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

//PLATE COUNT SORT
template <>
std::vector<plate_t> counting_sort<plate_t>(std::vector<plate_t> vec, const size_t& k, int count[], const int& n, const int& min){
    std::vector<plate_t> output(vec.size());

    for(int i = 0; i < vec.size(); i++){
        count[(int)vec[i][k] - min + 1]++;
    }
    for(int i = 1; i < n; i++){
        count[i] += count[i - 1];
    }

    for (int i = 0; i < vec.size(); i++) {
        output[count[(int) vec[i][k] - min]] = vec[i];
        count[(int)vec[i][k] - min]++;
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

std::vector<plate_t> read_element(std::vector<plate_t> plates, size_t k){
    if(plates[0][k] > '0' - 1 && plates[0][k] < '9' + 1){
        int count[11] = { 0 };
        return counting_sort<plate_t>(std::move(plates), k, count, 11, (int)'0');
    }else{
        int count[27] = { 0 };
        return counting_sort<plate_t>(std::move(plates), k, count, 27, (int)'A');
    }
}

std::pair<std::vector<int>, int> count(std::vector<plate_t> vec, int k){
    std::vector<int> count(256);
    for(int i = 0; i < vec.size(); i++){
        count[(int)vec[i][k] + 1]++;
    }
    for(int i = 1; i < 255; i++){
        count[i] += count[i - 1];
    }

    return std::make_pair(count, k);
}

template <>
std::vector<plate_t> radix_sort<plate_t>(std::vector<plate_t> vec){
    auto max = *std::max_element(vec.begin(), vec.end());
    /*for(int i = (int)max.size() - 1; i >= 0; i--){
        vec = read_element(std::move(vec), i);
    }*/


    std::vector<std::future<std::pair<std::vector<int>, int>>> futures;
    for(int i = (int)max.size() - 1; i >= 0; i--){
        futures.push_back(std::async(std::launch::async, count, vec, i));
    }
    std::vector<std::pair<std::vector<int>, int>> counting_result;
    for(auto& fur : futures){
            counting_result.emplace_back(fur.get());
    }
    //Måste fixa sortering här......
    std::vector<plate_t> output(vec.size());
    for(auto k : counting_result){
        for (int i = vec.size() - 1; i >= 0; i--) {
            if(vec[i][k.second] > 0) {
                output[k.first[(int) vec[i][k.second]] - 1] = vec[i];
                k.first[(int) vec[i][k.second]]--;
            }
        }
    }
    return output;
}



#endif //COMPETITION_RADIX_SORT_H
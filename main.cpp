#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Timer.h"
#include "radix_sort.h"
#include <thread>
#include "reader.h"
//#include <execution>
#include <string>
#include <unistd.h>
#include <stdio.h>

template<typename T>
void get_result(std::vector<T> result, const std::string& filename = "result.txt"){
    std::ofstream out(filename);
    out.precision(7); //Gör att vi skriver ut två decimaltal
    for(int i = 0; i < result.size(); i++){
        if(i % 10 == 0 && i){
            out << "\n";
        }
        out << result[i] << " ";
    }
}

template<>
void get_result<price_t>(std::vector<price_t> result, const std::string& filename){
    std::ofstream out(R"(C:\Users\angel\OneDrive\Dokument\DT046G Datastrukturer och Algoritmer\Competition\test\)" + filename);
    out.precision(7); //Gör att vi skriver ut två decimaltal
    for(int i = 0; i < result.size(); i++){
        if(i % 10 == 0 && i){
            out << "\n";
        }
        out << result[i] << " kr  ";
    }
}

void print_result(const std::string& filename = "res.txt"){
    std::ifstream in(R"(C:\Users\angel\OneDrive\Dokument\DT046G Datastrukturer och Algoritmer\Competition\test\)" + filename);
    std::string ye;
    usleep(250000);
    while(std::getline(in, ye)){
        std::cout << ye << std::endl;
    }
}

template<typename FILE>
void generate(data series, FILE filename){
    std::vector<plate_t> plates;
    std::vector<price_t> prices;
    std::vector<galaxy_t> galaxies;

    Timer time;

    switch(series){
        case PLATES:
            plates = read_file<plate_t>(filename);
            std::cout << "Generating bogo sort..." << std::endl;
            sleep(7);

            time.start();
            //radix_sort(plates);
            //parallel_radix_sort(plates);
            async_radix_sort<plate_t>(plates);

            time.stop();
            std::cout << time << std::endl;
            if(std::is_sorted(plates.begin(), plates.end())){
                get_result(plates);
            }
            break;
        case PRICES:
            prices = read_file<price_t>(filename);
            std::cout << "Fetching bubble sort..." << std::endl;
            sleep(7);
            time.start();
            radix_sort(prices);
            //async_radix_sort(prices);
            time.stop();
            std::cout << time << std::endl;

            if(std::is_sorted(prices.begin(), prices.end())){
                get_result(prices);
            }
            break;
        case GALAXIES:
            time.start();
            //std::sort(std::execution::par_unseq, galaxies.begin(), galaxies.end());
            time.stop();
            std::cout << time << std::endl;
            if(std::is_sorted(galaxies.begin(), galaxies.end())){
                get_result(galaxies);
            }
            break;
    }
}


int main() {
    if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0){
        data series = get_type(stdin);
        generate(series, stdin);
        return 0;
    }

    std::string filename = "test2.txt";
    data series = get_type(filename);
    generate(series, filename);

    return 0;
}

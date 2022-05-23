#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Timer.h"
#include "radix_sort.h"
#include <thread>
#include "reader.h"
#include <execution>
#include <string>
#include <unistd.h>
#include <stdio.h>

template<typename T>
void get_result(std::vector<T> result, const std::string& filename = "result.txt"){
    std::ofstream out(R"(C:\Users\angel\OneDrive\Dokument\Skola\Data\sortcomp\text\)" + filename);
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
    std::ofstream out(R"(C:\Users\angel\OneDrive\Dokument\Skola\Data\sortcomp\text\)" + filename);
    out.precision(7); //Gör att vi skriver ut två decimaltal
    for(int i = 0; i < result.size(); i++){
        if(i % 10 == 0 && i){
            out << "\n";
        }
        out << result[i] << " kr  ";
    }
}

void print_result(const std::string& filename = "res.txt"){
    std::ifstream in(R"(C:\Users\angel\OneDrive\Dokument\Skola\Data\sortcomp\text\)" + filename);
    std::string ye;
    usleep(250000);
    while(std::getline(in, ye)){
        std::cout << ye << std::endl;
    }
}

int main() {
    std::string filename = "ICA.txt";
    if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0){
        data series = get_type(stdin);
    }

    data series = get_type(filename);
    std::vector<plate_t> plates;
    std::vector<price_t> prices;
    std::vector<galaxy_t> galaxies;

    Timer time;

    //Tar lång tid att lasta in
    //std::vector<galaxy_t> galaxies = read_file<galaxy_t>("JST.txt");

    switch(series){
        case PLATES:
            plates = read_file<plate_t>(filename);
            std::cout << "Generating bogo sort..." << std::endl;
            sleep(7);
            time.start();
            //radix_sort<plate_t>(plates);
            //parallel_radix_sort(plates);
            async_radix_sort(plates);
            break;
        case PRICES:
            std::cout << "Fetching bubble sort..." << std::endl;
            sleep(7);
            prices = read_file<price_t>("ICA.txt");
            time.start();
            break;
        case GALAXIES:
            time.start();
            std::sort(std::execution::par_unseq, plates.begin(), plates.end());
            break;
    }
    time.stop();
    std::cout << time << std::endl;
    if(!std::is_sorted(plates.begin(), plates.end())){
        auto it = std::is_sorted_until(plates.begin(),plates.end());
        std::cout << *it << std::endl;
    }
    get_result(plates);

    print_result();
    return 0;
}

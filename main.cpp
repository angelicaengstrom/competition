#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "Timer.h"
#include "radix_sort.h"
#include <thread>
#include <future>
#include "reader.h"
#include "merge_sort.h"
#include <execution>
#include "trie.h"

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

int main() {
    std::string filename = "new_plates.txt";
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

    //------------------------------------- LÅT STÅ
    //-------------------------------------- TEST AV PLATES
    //STD::SORT ÄR 1.13732 s
    //RADIX SORT MED ASYNC PÅ COUNTING ÄR 0.7 S
    //RADIX SORT UTAN ASYNC ÄR 0.6S
   // mergeSort(plates, 0, plates.size() - 1);
    switch(series){
        case PLATES:
            plates = read_file<plate_t>(filename);

            time.start();
            //radix_sort(plates);
            parallel_radix_sort(plates);
            //std::sort(std::execution::par_unseq, plates.begin(), plates.end());
            //parallel_radix_sort(plates.begin(), plates.end());
            break;
        case PRICES:
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
    //get_result(plates, "result.txt");

    //sorted_plates.get();
    //-----------------------------------------------


    //--------------------------------------------MERGE SORT TEST
/*
    // t1 and t2 for calculating time for
    std::cout << prices.size() << std::endl;
    time.start();

    std::cout << "std::sort: ";
    time.start();
    //std::sort(prices.begin(), prices.end(), std::less<>()); //1.46211 s
    time.stop();
    std::cout << time << std::endl;

    std::cout << "mergesort_mt3: ";
    time.start();
    //mergesort_mt3(prices.begin(), prices.end()); //0.359 s
    time.stop();
    std::cout << time << std::endl;

    std::cout << prices.size() << std::endl;
    get_result(prices, "result2.txt");*/



    //--------------------------------- TEST OF ICA.txt
    //std::vector<price_t> prices = read_file<price_t>("ICA.txt");
    //Multiplikation och delningsoperatorer tar superlång tid - vi måste konvertera flyttalen till unsigned int istället
    //Med transform till radix tog detta 0.53 sekunder, med std::sort tog detta 0.95 sekunder
    //Med async till delningsoperatorer tog detta 0.0029 sekunder

    //std::future<std::vector<price_t>> result = std::async(std::launch::async, test, prices);
    //std::sort(prices.begin, prices.end);
    //std::vector<price_t> result = radix_sort<price_t>(prices.begin, prices.end);
    //get_result(result.get(), "result2.txt");


    //------------------------------------- PROMISE
    //Vi skickar ett värde som vi inte har än och skickar den i framtiden, det är vårt löfte

    //std::promise<int> p;
    //std::future<int> f = p.get_future();
    //std::future<int> fu = std::async(std::launch::async, count, plates, std::ref(f));

    //std::future<int> fu = std::async(std::launch::async, count, plates);

    //p.set_value(4) ger parametern 4. i framtiden
    //std::cout << fu.get() << std::endl; //Kan bara hämtas en gång
    return 0;
}

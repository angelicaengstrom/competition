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

template<typename T>
void get_result(std::vector<T> result, const std::string& filename = "result.txt"){
    std::ofstream out(R"(C:\Users\angel\OneDrive\Dokument\DT046G Datastrukturer och Algoritmer\Competition\test\)" + filename);
    out.precision(7); //Gör att vi skriver ut två decimaltal
    for(int i = 0; i < result.size(); i++){
        if(i % 10 == 0 && i){
            out << "\n";
        }
        out << result[i] << " ";
    }
}

std::vector<price_t> test(std::vector<price_t> prices){
    std::transform(prices.begin(), prices.end(), prices.begin(), [](const price_t& price){ return price * 100; });
    std::vector<price_t> result = radix_sort<price_t>(prices);
    std::transform(result.begin(), result.end(), result.begin(), [](const price_t& price){ return price / 100; });
    return result;
}

enum data{ PLATES, PRICES, GALAXIES };
int main() {
    data series = PLATES;
    //Martin använder vektorer vid inläsning av data
    std::vector<plate_t> plates = read_file<plate_t>("new_plates.txt");
    //Tar lång tid att lasta in
    //std::vector<galaxy_t> galaxies = read_file<galaxy_t>("JST.txt");

    Timer time;
    time.start();


    //------------------------------------- LÅT STÅ
    //-------------------------------------- TEST AV PLATES
    //Testa om det tar snabbare med en array....

    //std::future<std::vector<plate_t>> sorted_plates;
    switch(series){
        case PLATES:
            radix_sort<plate_t>(std::move(plates));
            //sorted_plates = std::async(std::launch::async, radix_sort<plate_t>, std::move(plates));
            break;
        case PRICES:

            break;
        case GALAXIES:
            break;
    }
    //auto data = sorted_plates.get();
    time.stop();
    std::cout << time << std::endl;
    //get_result(data, "result.txt");

    //sorted_plates.get();
    //Använd for_each istället, snyggare...
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

//
// Created by angel on 2022-05-11.
//

#ifndef COMPETITION_READER_H
#define COMPETITION_READER_H

using plate_t = std::string;
using galaxy_t = double;
using price_t = float;

template<typename T>
std::vector<T> read_file(const std::string& filename){
    std::ifstream file(R"(C:\Users\angel\OneDrive\Dokument\Skola\Data\sortcomp\text\)" + filename);
    std::vector<T> buffer;
    T element;
    while(file >> element){
        buffer.push_back(element);
    }
    file.close();
    return buffer;
}

template <>
std::vector<price_t> read_file<price_t>(const std::string& filename){
    std::ifstream file(R"(C:\Users\angel\OneDrive\Dokument\Skola\Data\sortcomp\text\)" + filename);
    std::vector<price_t> buffer;
    std::string element;
    while(file >> element){
        if(element != "kr") {
            buffer.push_back(std::stof(element));
        }
    }
    file.close();
    return buffer;
}
/*
template <> std::vector<galaxy_t> read_file<galaxy_t>(const std::string& filename){
    std::ifstream file(R"(C:\Users\angel\OneDrive\Dokument\DT046G Datastrukturer och Algoritmer\Competition\test\)" + filename);
    std::vector<galaxy_t> buffer;
    galaxy_t element;
    while(file >> element){
        buffer.push_back(element);
    }
    file.close();
    return buffer;
}*/

#endif //COMPETITION_READER_H

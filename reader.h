//
// Created by angel on 2022-05-11.
//

#ifndef COMPETITION_READER_H
#define COMPETITION_READER_H

template<typename T>
std::vector<T> read_file(const std::string& filename){
    std::ifstream file(R"(C:\Users\angel\OneDrive\Dokument\DT046G Datastrukturer och Algoritmer\Competition\test\)" + filename);
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
    std::ifstream file(R"(C:\Users\angel\OneDrive\Dokument\DT046G Datastrukturer och Algoritmer\Competition\test\)" + filename);
    std::vector<price_t> buffer;
    std::string element;
    while(file >> element){
        if(element != "kr") {
            buffer.push_back(std::stod(element));
        }
    }
    file.close();
    return buffer;
}

template<typename T>
std::vector<T> read_file(FILE *__stream){
    rewind(__stream);
    std::vector<T> buffer;
    T element;
    while(std::cin >> element){
        buffer.push_back(element);
    }
    fclose(__stream);
    return buffer;
}

template <>
std::vector<price_t> read_file<price_t>(FILE *__stream){
    rewind(__stream);
    std::vector<price_t> buffer;
    std::string element;
    while(std::cin >> element){
        if(element != "kr") {
            buffer.push_back(std::stod(element));
        }
    }
    fclose(__stream);
    return buffer;
}

enum data{PLATES, PRICES, GALAXIES, NONE};
enum::data get_type(const std::string& filename){
    std::ifstream file(R"(C:\Users\angel\OneDrive\Dokument\DT046G Datastrukturer och Algoritmer\Competition\test\)" + filename);
    file >> std::ws;
    int c = file.peek();

    if ( c == EOF ) return NONE;
    if (std::isdigit(c)){
        std::string n;
        file >> n;
        file.close();
        if(n.find_first_of('.')){
            return PRICES;
        }else{
            return GALAXIES;
        }

    }
    else{
        file.close();
        return PLATES;
    }
}

//funkar denna????
enum::data get_type(FILE *__stream){
    rewind(__stream);
    std::cin >> std::ws;
    int c = std::cin.peek();

    if ( c == EOF ) return NONE;
    if (std::isdigit(c)){
        std::string n;
        std::cin >> n;
        fclose(__stream);
        if(n.find_first_of('.')){
            return PRICES;
        }else{
            return GALAXIES;
        }
    }
    else{
        fclose(__stream);
        return PLATES;
    }
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

//
// Created by angel on 2022-05-04.
//

#ifndef COMPETITION_TIMER_H
#define COMPETITION_TIMER_H
#include <chrono>
#include <iostream>

class Timer{
public:
    void start(){  t_start = std::chrono::system_clock::now(); };
    void stop(){ elapsed_seconds = std::chrono::system_clock::now() - t_start; };

    friend std::ostream& operator<<(std::ostream& out, const Timer& time){
        out << time.elapsed_seconds.count() << " s\n";
        return out;
    };
private:
    std::chrono::time_point<std::chrono::system_clock> t_start;
    std::chrono::duration<double> elapsed_seconds;
};
#endif //COMPETITION_TIMER_H

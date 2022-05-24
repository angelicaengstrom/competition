//
// Created by angel on 2022-05-12.
//

#ifndef COMPETITION_MERGE_SORT_H
#define COMPETITION_MERGE_SORT_H

template<typename Iter>
void mergesort_mt3(Iter begin, Iter end,
                   unsigned int N = std::thread::hardware_concurrency()/2)
{
    auto len = std::distance(begin, end);

    if (len <= 1024 || N < 2)
    {
        std::sort(begin,end);
        return;
    }

    Iter mid = std::next(begin, len/2);
    auto fn = std::async(mergesort_mt3<Iter>, begin, mid, N-2);
    mergesort_mt3(mid, end, N-2);
    fn.wait();
    std::inplace_merge(begin, mid, end);
}

#endif //COMPETITION_MERGE_SORT_H

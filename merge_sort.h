//
// Created by angel on 2022-05-12.
//

#ifndef COMPETITION_MERGE_SORT_H
#define COMPETITION_MERGE_SORT_H

#define sz 20
#define thread_size 4
int arr[sz];
int temp_val = 0;

// Merges two subarrays of array[].
// First subarray is arr[begin..mid]
// Second subarray is arr[mid+1..end]
template<typename T>
void merge(std::vector<T>& array, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    std::vector<T> leftArray(subArrayOne);
    std::vector<T> rightArray(subArrayTwo);

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
    indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
template <typename T>
void mergeSort(std::vector<T>& array, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void combine_array(int first, int mid_val, int end){
    int* start = new int[mid_val - first + 1];
    int* last = new int[end - mid_val];
    int temp_1 = mid_val - first + 1;
    int temp_2 = end - mid_val;
    int i, j;
    int k = first;
    for(i = 0; i < temp_1; i++){
        start[i] = arr[i + first];
    }
    for (i = 0; i < temp_2; i++){
        last[i] = arr[i + mid_val + 1];
    }
    i = j = 0;
    while(i < temp_1 && j < temp_2){
        if(start[i] <= last[j]){
            arr[k++] = start[i++];
        }
        else{
            arr[k++] = last[j++];
        }
    }
    while (i < temp_1){
        arr[k++] = start[i++];
    }
    while (j < temp_2){
        arr[k++] = last[j++];
    }
}
void Sorting_Threading(int first, int end){
    int mid_val = first + (end - first) / 2;
    if(first < end){
        Sorting_Threading(first, mid_val);
        Sorting_Threading(mid_val + 1, end);
        combine_array(first, mid_val, end);
    }
}
void* Sorting_Threading(void* arg){
    int set_val = temp_val++;
    int first = set_val * (sz / 4);
    int end = (set_val + 1) * (sz / 4) - 1;
    int mid_val = first + (end - first) / 2;
    if (first < end){
        Sorting_Threading(first, mid_val);
        Sorting_Threading(mid_val + 1, end);
        combine_array(first, mid_val, end);
    }
}

#endif //COMPETITION_MERGE_SORT_H

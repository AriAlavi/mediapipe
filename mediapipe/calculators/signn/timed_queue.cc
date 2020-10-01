#include <vector>
#include <queue>
#include <time.h>


#include "mediapipe/calculators/signn/timed_queue.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/calculator_framework.h"

using namespace mediapipe;

template <class T>
TimedQueue<T>::TimedQueue(){
    memory_length = 1;
}
template <class T>
TimedQueue<T>::TimedQueue(double memory_in_seconds){
    if (memory_length <= 0){
        throw __EXCEPTIONS;
    }
    memory_length = memory_in_seconds;
}
template <class T>
void TimedQueue<T>::add(T adding){
    dispenser.push_back(adding);
    times.push(clock());
}
template <class T>
std::vector<T> TimedQueue<T>::get(){
    auto front = times.front();
    auto now = clock();
    while((delta_t(now, front) > memory_length) && dispenser.size() > 0){
        dispenser.erase(dispenser.begin());
        times.pop();
        front = times.front();
    }
    return dispenser;
}
template <class T>
double TimedQueue<T>::delta_t(clock_t start, clock_t end){
    return abs((double)(end - start) / CLOCKS_PER_SEC);
}

template class TimedQueue<bool>;
template class TimedQueue<int>;
template class TimedQueue<double>;
template class TimedQueue<float>;
template class TimedQueue<mediapipe::NormalizedLandmarkList>;
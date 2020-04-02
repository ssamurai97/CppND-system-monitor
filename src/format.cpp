
#include "format.h"
#include <string>


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
std::string Format::ElapsedTime(long seconds) {

    int ss{}, mm {} , hh {}; 
    std::string time{};

    //hours 
    hh = seconds / 3600;
    //minutes
    seconds = seconds % 3600;
    mm = seconds / 60;

    // seconds
    ss = seconds % 60;

    //format time string

    if(hh < 10 ){time ="0";}
    time += std::to_string(hh) + ":"; 

    if(mm < 10 ){time +="0";}
    time += std::to_string(mm) + ":";
    if(ss < 10 ){ time += "0";}
     time += std::to_string(ss);
   return time; 
     }
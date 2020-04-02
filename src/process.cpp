
#include "process.h"
#include "linux_parser.h"
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;


Process::Process(int p) :pid_{p}{}
// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 


    auto totalTime = LinuxParser::ActiveJiffies(pid_);
    auto startTime = LinuxParser::UpTime(pid_);
    auto upTime = LinuxParser::UpTime();

    auto seconds = upTime - (startTime / (_SC_CLK_TCK));

    return (totalTime / sysconf(_SC_CLK_TCK)) / seconds;
 }

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_) ;}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    
    return std::stol(LinuxParser::Ram(pid_)) > std::stol(LinuxParser::Ram(a.pid_));
    }
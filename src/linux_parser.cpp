#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include<iomanip>
#include<sstream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
[[nodiscard]]vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
[[nodiscard]]float LinuxParser::MemoryUtilization() { 
  string key, value, line {};

  float memTotal {0.0}, memFree {0.0};

  std::ifstream ifs {kProcDirectory + kMeminfoFilename };

  if(ifs.is_open()){
    while(std::getline(ifs, line)){
    std::istringstream linestream(line);
    linestream>>key >> value;
    if(key == "MemToTal:"){
      memTotal = std::stof(value);
    }
    if(key == "MemFree:"){
      memFree = std::stof(value);
    }

    }
    return (memTotal - memFree ) / memTotal;
  }
  

  return 0.0; 
  }

// TODO: Read and return the system uptime
[[nodiscard]]long LinuxParser::UpTime() {
   string uptime{};
   string line{};
   std::ifstream ifs{kProcDirectory + kUptimeFilename};
   if(ifs.is_open()){
     std::getline(ifs, line);
     std::istringstream linestream{line};

     linestream>>uptime;

     return stol(uptime);
   }
  
  return 0; 
  }

// TODO: Read and return the number of jiffies for the system
[[nodiscard]]long LinuxParser::Jiffies() { 
  
  long total{0};
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for(int i = kUser_; i <= kSteal_; i++){
    total += std::stol(cpuUtilization[i]);
  }
  return total; 
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
[[nodiscard]]long LinuxParser::ActiveJiffies(int pid) {
  
    long total{0};
    string line{}, value{};
    std::ifstream ifs{kProcDirectory + std::to_string(pid) + kStatFilename};

    if(ifs.is_open()){
      std::getline(ifs, line);
      std::istringstream linestream(line);
      int i{0};
      while(linestream >> value){
        if(i >= 13 && i <= 16){
          total += stol(value);
        }
        i++;
      }
    }

   return total; 
   }

// TODO: Read and return the number of active jiffies for the system
[[nodiscard]]long LinuxParser::ActiveJiffies() { 

  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long total {0};

  auto cpuUtilization = LinuxParser::CpuUtilization();
  for(int i =kIdle_; i <= kIOwait_; i++){
    total += stol(cpuUtilization[i]);
  }

  return 0;
   }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string value{}; 
  string line{};

  vector<string>cpuValues{};
  std::ifstream ifs{kProcDirectory + kStatFilename};

  if(ifs.is_open()){
    std::getline(ifs,line);
    std::istringstream linestream(line);

    while(linestream >> value ){
      if(value != "cpu"){
        cpuValues.push_back(value);
      }
    }
  }
   return cpuValues; 
   
   }


//get value form proc/stat

[[nodiscard]] string LinuxParser::GetValueFromProcStat(string keyToLook){
  string key {}, value {}, line {};

  std::ifstream ifs{kProcDirectory + kStatFilename};

  if(ifs.is_open()){
    while(std::getline(ifs, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == keyToLook){
        return value;
      }
    }
  }
  return value;
}


string LinuxParser::StatsPid(int pid){
  std::ifstream ifs{kProcDirectory + to_string(pid) + kStatFilename};
  string line {};
  std::getline(ifs, line);

  return line;
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  
  auto value = LinuxParser::GetValueFromProcStat("processes");

  if(value !=""){
    return std::stoi(value);
  }
  
   return 0; 
   }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  auto value = LinuxParser::GetValueFromProcStat("procs_running");

  if(value != ""){
    return std::stoi(value);
  }
  return 0; 
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd{};
  std::ifstream ifs{kProcDirectory + to_string(pid) + kCmdlineFilename};

  if(ifs.is_open()){
    std::getline(ifs, cmd);
    return cmd;
  }
  return string(); 
  
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string value {}, key {}, line {};

  std::stringstream ram{};

  std::ifstream ifs{kProcDirectory + to_string(pid) + kStatusFilename};
  if(ifs.is_open()){
    while(std::getline(ifs, line)){
    std::istringstream linestream{line};
    linestream >> key >> value;

    if( key== "VmSize:"){
      ram << std::fixed <<std::setprecision(3) << stof(value) / 1000 ;
      return ram.str();
    }

    }
  }
  
  return "0";

   }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line{}, key {}, value{};

  std::ifstream ifs{kProcDirectory + to_string(pid) + kStatusFilename };

  if(ifs.is_open()){
    while (std::getline(ifs, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid:"){
        return value;
      }
    
    }
    
  }
  return string(); 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
[[nodiscard]]string LinuxParser::User(int pid) { 
  
  auto uid = LinuxParser::Uid(pid);
  string line {}, key {}, x {}, value {};
  std::ifstream ifs{kPasswordPath};

  if(ifs.is_open()){
    while (std::getline(ifs, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      linestream >> key >> x >> value;
      if(value == uid){
        return key;
      }
    }
    
  }
  return string(); 
  
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line {}, value {};

  std::ifstream ifs{kProcDirectory + to_string(pid) + kStatFilename};

  if(ifs.is_open()){
    std::getline(ifs, line);
    std::istringstream linestream(line);

    auto i {0};
    while(linestream >> value){
      if(i == 21){
        return stol(value) / sysconf(_SC_CLK_TCK);
      }
      i++;
    }
  }
  
  return 0; 
  }
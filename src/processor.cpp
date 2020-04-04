#include "processor.h"
#include "linux_parser.h"
#include<unistd.h>

// TODO: Return the aggregate CPU utilization
[[nodiscard]] float Processor::Utilization() {
    totalJiffiesStart = LinuxParser::Jiffies();
    activeJiffiesStart= LinuxParser::ActiveJiffies();

    usleep(100000);

     totalJiffiesEnd = LinuxParser::Jiffies();

     activeJiffiesEnd = LinuxParser::ActiveJiffies();


    auto totalDelta = totalJiffiesEnd - totalJiffiesStart;
    auto activeDelta = activeJiffiesEnd - activeJiffiesStart;
    if(totalDelta == 0 ){
        return 0.0;
    }
    return float(activeDelta) / float(totalDelta);
 }
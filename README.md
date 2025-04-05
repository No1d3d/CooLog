# CooLog
CooLog - a cpp leveled logger

CooLog is a minimalist logging tool for C++ projects that allows easy and convenient logging to both the console and files. It supports multiple logging levels, automatic log file name generation, and color-coded output for terminal messages.

Features
Console logging with color support (Blue, Green, Yellow, Red).
File logging with automatic file name generation if a file name is not provided.
Logging levels:
- LOGOFF: Disables all logging.
- DEBUG: Logs only debug messages.
- INFO: Logs info messages and above.
- WARNING: Logs warning messages and above.
- ERROR: Logs only error messages.

Installation
To use CooLog, simply include the header file in your project:

```cpp
#include "coolog.hpp"
// You don't need to link any external libraries for basic functionality.
```
Usage
You can create an instance of CooLog and start logging messages with various levels:

```cpp
CooLog logger(CooLog::Level::DEBUG, true, "output.txt");
// Logger set to debug level (debug msgs output in console), logging to file enabled, log file "output.txt"
logger.debug("This is a debug message.");
logger.info("This is an info message.");
logger.warning("This is a warning message.");
logger.error("This is an error message.");
```
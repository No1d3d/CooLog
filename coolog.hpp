#ifndef COOLOG_HPP
#define COOLOG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <regex>

class CooLog {
public:
    enum class Level {
        LOGOFF = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    explicit CooLog(Level level = Level::INFO, bool logToFile = false, const std::string& filename = "")
        : currentLevel(level)
    {
        if (logToFile && !fileLoggingInitialized) {
            std::string finalFilename = validateOrGenerateFilename(filename);
            logFileStream.open(finalFilename, std::ios::out | std::ios::app);
            if (!logFileStream.is_open()) {
                std::cerr << "Failed to open log file: " << finalFilename << "\n";
                toFile = false;
            } else {
                toFile = true;
                fileLoggingInitialized = true;
            }
        }
    }

    ~CooLog() {}

    void log(Level level, const std::string& message) {
        if (!shouldLog(level)) return;

        std::string timestamp = getTimestamp();
        std::string levelStr = levelToString(level);
        std::string entry = "[" + timestamp + "] [" + levelStr + "] " + message;

        if (toFile && logFileStream.is_open()) {
            logFileStream << entry << std::endl;
        }

        std::string colored = getColorForLevel(level) + entry + ansiReset;
        std::cout << colored << std::endl;
    }

    void debug(const std::string& message) { log(Level::DEBUG, message); }
    void info(const std::string& message) { log(Level::INFO, message); }
    void warning(const std::string& message) { log(Level::WARNING, message); }
    void error(const std::string& message) { log(Level::ERROR, message); }

    void setLevel(Level level) { currentLevel = level; }

private:
    Level currentLevel;
    bool toFile = false;

    static int fallbackLogCounter;
    static bool fileLoggingInitialized;
    static std::ofstream logFileStream;

    const std::string ansiReset = "\033[0m";

    std::string getColorForLevel(Level level) const {
        switch (level) {
            case Level::DEBUG: return "\033[36m";   // blue
            case Level::INFO: return "\033[32m";    // green
            case Level::WARNING: return "\033[33m"; // yellow
            case Level::ERROR: return "\033[31m";   // red
            default: return "";
        }
    }

    bool shouldLog(Level level) const {
        if (currentLevel == Level::LOGOFF) return false;
        if (currentLevel == Level::DEBUG) return level == Level::DEBUG;
        return static_cast<int>(level) >= static_cast<int>(currentLevel);
    }

    std::string levelToString(Level level) const {
        switch (level) {
            case Level::DEBUG: return "DEBUG";
            case Level::INFO: return "INFO";
            case Level::WARNING: return "WARNING";
            case Level::ERROR: return "ERROR";
            case Level::LOGOFF: return "LOGOFF";
            default: return "UNKNOWN";
        }
    }

    std::string getTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&t), "%F %T")
            << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }

    bool fileExists(const std::string& filename) {
        std::ifstream infile(filename);
        return infile.good();
    }

    std::string validateOrGenerateFilename(const std::string& name) {
        std::regex extPattern(R"(.*\.(log|txt))", std::regex_constants::icase);
        if (!name.empty() && std::regex_match(name, extPattern) && !fileExists(name)) {
            return name;
        }

        std::string generatedName;
        do {
            std::ostringstream oss;
            oss << "mycoolog" << fallbackLogCounter++ << ".log";
            generatedName = oss.str();
        } while (fileExists(generatedName));

        return generatedName;
    }
};


int CooLog::fallbackLogCounter = 1;
bool CooLog::fileLoggingInitialized = false;
std::ofstream CooLog::logFileStream;

#endif // COOLOG_HPP

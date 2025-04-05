#ifndef COOLOG_HPP
#define COOLOG_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class CooLog {
public:
    enum class Level {
        LOGOFF = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    explicit CooLog(Level level = Level::INFO) : currentLevel(level) {}

    void log(Level level, const std::string& message) {
        if (shouldLog(level)) {
            std::cout << "[" << getTimestamp() << "] "
                      << "[" << levelToString(level) << "] "
                      << message << std::endl;
        }
    }

    void debug(const std::string& message) { log(Level::DEBUG, message); }
    void info(const std::string& message) { log(Level::INFO, message); }
    void warning(const std::string& message) { log(Level::WARNING, message); }
    void error(const std::string& message) { log(Level::ERROR, message); }

    void setLevel(Level level) { currentLevel = level; }

private:
    Level currentLevel;

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
};

#endif // COOLOG_HPP
#include "logger.h"

#include <Arduino.h>

namespace Logger {
    Message::Message() : timestamp(millis()) {}

    Message& Message::setSource(String source) {
        this->source = source;
        return *this;
    }

    Message& Message::setLevel(LogLevel level) {
        this->level = level;
        return *this;
    }

    Message& Message::setContent(String content) {
        this->content = content;
        return *this;
    }

    Message& Message::setTimestamp(timestamp_t timestamp) {
        this->timestamp = timestamp;
        return *this;
    }

    void Message::log() const {
        // Logger::getInstance().log(*this);
        if (level < LogLevel::Info){
            return;
        }

        char str[100];
        const char* level_str = Logger::logLevelToString(level);
        snprintf(str, 100, "[%09lu][%s][%s] : %s", timestamp, level_str, source.c_str(), content.c_str());
        Serial.println(str);
    }

    String Message::getSource() const {
        return source;
    }

    String Message::getContent() const {
        return content;
    }

    LogLevel Message::getLevel() const {
        return level;
    }

    timestamp_t Message::getTimestamp() const {
        return timestamp;
    }

    Logger::Logger() : level(LogLevel::Error) {}

    Logger& Logger::getInstance() {
        static Logger instance = Logger();
        return instance;
    }

    void Logger::setLevel(LogLevel level) {
        this->level = level;
    }

    void Logger::log(LogLevel level, const char* msg) {
        // Do not log if the level isn't high enough
        if (level < this->level) {
            return;
        }

        const char* level_str = Logger::logLevelToString(level);
        Serial.println("[" + String(level_str) + "]:" + String(msg));
        // Serial.flush();
    }

    void Logger::log(const Message& msg) {
        // Do not log if the level isn't high enough
        if (msg.getLevel() < this->level) {
            return;
        }

        char str[100];
        const char* level_str = Logger::logLevelToString(level);
        snprintf(str, 100, "[%09lu][%s][%s] : %s", msg.getTimestamp(), level_str, msg.getSource().c_str(), msg.getContent().c_str());
        Serial.println(str);
        // Serial.flush();
    }

    const char* Logger::logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info:  return "INFO";
            case LogLevel::Error: return "ERROR";
            default:              return "UNKNOWN_LOG_LEVEL";
        }
    }
}
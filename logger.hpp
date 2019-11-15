#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <map>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <mutex>
#include <type_traits>
#include "singleton.hpp"
#include "enum_util.hpp"
#include "macro_util.hpp"
#include "str_util.hpp"
#include "time_util.hpp"

DECLARE_TYPED_ENUM(LogSeverity, std::uint8_t, INFO, SUCCESS, TRACE, WARNING, CRITICAL );

class Logger
{
protected:
    using Clock = std::chrono::system_clock;
    std::mutex log_mutex_;
    std::string name_;
    bool print_time_stamp_;

public:
    Logger() {}
    virtual ~Logger() {};
    const std::string name() { return name_; }
    virtual void log(LogSeverity severity, const std::string& msg) = 0;
    void enable_time_stamp(bool value) {
        print_time_stamp_ = value;
    }
};

class LoggerFactory: public Singleton<LoggerFactory>
{
private:
    friend class Singleton<LoggerFactory>;

    class FileLogger : public Logger
    {
    private:
        const std::string file_name_;
        std::ofstream log_file_;
    public:
        FileLogger(const std::string& logger_name, const std::string& file_name ) : file_name_(file_name) 
        { 
            name_ = logger_name; 
            log_file_.open(file_name);
            if(log_file_.is_open()) {
                std::lock_guard<std::mutex> lock(log_mutex_);
                log_file_ << "[" << time_util::get_time_stamp() << "] " << "Logger " << logger_name << " started." << std::endl;
            }
        }

        ~FileLogger() 
        {
            if(log_file_.is_open())
                log_file_.close();
        }

        void log(LogSeverity severity, const std::string& msg)
        {
            if(log_file_.is_open()) {
                std::lock_guard<std::mutex> lock(log_mutex_);
                if(print_time_stamp_)
                    log_file_ << "[" << time_util::get_time_stamp() << "] ";
                log_file_ << "[" << LogSeverityToStr[static_cast<std::underlying_type_t<LogSeverity>>(severity)] << "] "; 
                log_file_ << msg << std::endl;
            }
        }

    };

private:
    std::map<std::string, std::shared_ptr<Logger>> loggers_;

private:
    LoggerFactory() { }
    ~LoggerFactory() { }

public:
    std::shared_ptr<Logger> create_file_logger(const std::string& name, const std::string& file_name);
    std::shared_ptr<Logger> get_logger(const std::string& name);
};

inline std::shared_ptr<Logger> LoggerFactory::create_file_logger(const std::string& name, const std::string& file_name)
{
    auto itr = loggers_.find(name);
    if(itr == loggers_.end())
    {
        loggers_[name] = std::make_shared<FileLogger>(name, file_name);
        return loggers_[name];
    }
    else {
        loggers_[name]->log(LogSeverity::WARNING, std::string("Attempt to create a logger with name").append(name).append(", which is already created."));
        return loggers_[name];
    }
}

inline std::shared_ptr<Logger> LoggerFactory::get_logger(const std::string& name)
{
    auto itr = loggers_.find(name);
    if(itr == loggers_.end())
    {
        //throw std::runtime_error(std::string("LoggerFactory::get_logger(").append(name).append(") : no such a looger exist."));
        return nullptr;
    }
    else
        return loggers_[name];
}

/* #define Log_Info(logger, msg) \
    logger->log(Logger::Severity::INFO, str_util::format("[{}:{} -> {}]: {}", __SHORT_FILE_NAME__, __LINE__, __func__, msg)); \

#define Log_Trace(logger, msg) \
    logger->log(Logger::Severity::TRACE, str_util::format("[{}:{} -> {}]: {}", __SHORT_FILE_NAME__, __LINE__, __func__, msg)); \

#define Log_Success(logger, msg) \
    logger->log(Logger::Severity::SUCCESS, str_util::format("[{}:{} -> {}]: {}", __SHORT_FILE_NAME__, __LINE__, __func__, msg)); \

#define Log_Warning(logger, msg) \
    logger->log(Logger::Severity::WARNING, str_util::format("[{}:{} -> {}]: {}", __SHORT_FILE_NAME__, __LINE__, __func__, msg)); \

#define Log_Critical(logger, msg) \
    logger->log(Logger::Severity::ERROR, str_util::format("[{}:{} -> {}]: {}", __SHORT_FILE_NAME__, __LINE__, __func__, msg)); \ */

#endif // !LOGGER_HPP
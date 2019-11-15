#pragma once
#ifndef TRACING_H
#define TRACING_H

#include <string>
#include <iostream>
#include <iomanip>
#include <memory>
#include "logger.hpp"

//TODO comment it out in case you don't want to trace activity.
#define TRACING_ENABLED

#if defined(NDEBUG) && !defined(TRACING_DISABLED)
#define TRACING_ENABLED
#endif

#ifdef TRACING_ENABLED
#define TRACE() \
    tracing::tracer _tracer_object__##__COUNTER__ { __func__, __SHORT_FILE_NAME__, __LINE__ }
#else
#define TRACE() // Nothing
#endif

static int nr_of_indentation = 0;

#ifdef TRACING_ENABLED
namespace tracing
{
class tracer
{
public:
    tracer() = delete;
    tracer(tracer const &) = delete;
    tracer(tracer &&) = delete;
    tracer &operator=(tracer const &) = delete;
    tracer &operator=(tracer &&) = delete;

    tracer(std::string const &fun, std::string const &file, int const line)
        : function_name{fun}, file_name{file}, line_number{line}, start_time_{time_util::Clock::now()}
    {
        logger = LoggerFactory::instance().get_logger("tracing");
        if(!logger)
            logger = LoggerFactory::instance().create_file_logger("tracing", "activity_tracking.log");


        auto indent = std::vector<char>(nr_of_indentation++ * 4, ' ');
        logger->log(LogSeverity::TRACE, str_util::format("{}{} ({}:{})[{}]", 
            std::string(indent.begin(), indent.end()), function_name, file_name, line_number, time_util::get_time()));
    }

    ~tracer()
    {
        auto indent = std::vector<char>(--nr_of_indentation * 4, ' ');
        logger->log(LogSeverity::TRACE, str_util::format("{}{} [{} ms]",std::string(indent.begin(), indent.end()), function_name, 
                        (double)std::chrono::duration_cast<std::chrono::microseconds>(time_util::Clock::now() - start_time_).count()/1000.0));
    }

private:
    std::string function_name;
    std::string file_name;
    int line_number;
    std::shared_ptr<Logger> logger;
    time_util::Clock::time_point start_time_;
};
} // namespace tracing
#endif // TRACING_ENABLED

#endif // TRACING_H
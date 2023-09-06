#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <time.h>
#include <chrono>
#include <spdlog/spdlog.h>
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h" // or "../stdout_sinks.h" if no color needed
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace nanami::util
{
    static inline int NowDateToInt()
    {
        time_t now;
        time(&now);

        // choose thread save version in each platform
        tm p;
        #ifdef PLATFROM_WINDOWS
        localtime_s(&p, &now);
        #else
        localtime_r(&now, &p);
        #endif
        int now_date = (1900 + p.tm_year) * 10000 + (p.tm_mon + 1) * 100 + p.tm_mday;
        return now_date;
    }

    static inline int NowTimeToInt()
    {
        time_t now;
        time(&now);
        // choose thread save version in each platform
        tm p;
        #ifdef PLATFROM_WINDOWS
        localtime_s(&p, &now);
        #else
        localtime_r(&now, &p);
        #endif

        int now_int = p.tm_hour * 10000 + p.tm_min * 100 + p.tm_sec;
        return now_int;
    }

    struct Logger
    {
        static Logger& instance()
        {
            static Logger logger();
            return logger;
        }
        std::shared_ptr<spdlog::logger> logger()
	    {
	    	return logger;
	    }

    private:
        Logger()
        {
            // hardcode log path
            const std::string log_dir = "./log"; // should create the folder if not exist
            const std::string logger_name_prefix = "test_";

            // decide print to console or log file
            bool console = false;

            // decide the log level
            std::string level = "debug";

            try {
                // logger name with timestamp
                int date = NowDateToInt();
                int time = NowTimeToInt();
                const std::string logger_name = logger_name_prefix + std::to_string(date) + "_" + std::to_string(time);

                if (console)
                    logger = spdlog::stdout_color_st(logger_name); // single thread console output faster
                else
                    logger = spdlog::create_async<spdlog::sinks::rotating_file_sink_mt>(logger_name, log_dir + "/" + logger_name + ".log", 500 * 1024 * 1024, 1000); // multi part log files, with every part 500M, max 1000 files

                // custom format
                logger->set_pattern("%Y-%m-%d %H:%M:%S.%f <thread %t> [%l] [%@] %v"); // with timestamp, thread_id, filename and line number

                if (level == "trace") {
                    logger->set_level(spdlog::level::trace);
                    logger->flush_on(spdlog::level::trace);
                } else if (level == "debug") {
                    logger->set_level(spdlog::level::debug);
                    logger->flush_on(spdlog::level::debug);
                } else if (level == "info") {
                    logger->set_level(spdlog::level::info);
                    logger->flush_on(spdlog::level::info);
                } else if (level == "warn") {
                    logger->set_level(spdlog::level::warn);
                    logger->flush_on(spdlog::level::warn);
                } else if (level == "error") {
                    logger->set_level(spdlog::level::err);
                    logger->flush_on(spdlog::level::err);
                }
            }
            catch (const spdlog::spdlog_ex& ex) {
                std::cout << "Log initialization failed: " << ex.what() << std::endl;
            }
        }
        XLogger(const XLogger&) = delete;
        XLogger& operator=(const XLogger&) = delete;
        ~XLogger()
        {
            spdlog::drop_all(); // must do this
        }

        std::shared_ptr<spdlog::logger> logger;
    };
}
#define LOG_TRACE(...) SPDLOG_LOGGER_CALL(nanami::util::Logger::instance()->logger().get(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_CALL(nanami::util::Logger::instance()->logger().get(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)  SPDLOG_LOGGER_CALL(nanami::util::Logger::instance()->logger().get(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)  SPDLOG_LOGGER_CALL(nanami::util::Logger::instance()->logger().get(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_CALL(nanami::util::Logger::instance()->logger().get(), spdlog::level::err, __VA_ARGS__)

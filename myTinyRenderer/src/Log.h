#pragma once
#include<spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include<chrono>
#include<filesystem>


class Log
{
private:

	std::string currentTimestamp()
	{
		auto now = std::chrono::system_clock::now();
		auto time_t_now = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
		localtime_s(&tm, &time_t_now);
		std::ostringstream timestamp;
		timestamp << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
		return timestamp.str();
	}

	Log()
	{
		std::ostringstream filename;
		filename << "logs/log_" << currentTimestamp() << ".log";

		// 确保日志目录存在
		std::filesystem::create_directories("logs");
		// 检查日志记录器是否已经存在
		if (!spdlog::get("basic_logger"))
		{
			mLogger = spdlog::basic_logger_mt("basic_logger", filename.str());
			mLogger->set_level(spdlog::level::debug); // 设置最低日志级别
			mLogger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v"); // 自定义日志格式
		}
		else
		{
			mLogger = spdlog::get("basic_logger");
		}
	}

	static Log& GetInstance()
	{
		static Log instance;
		return instance;
	}

public:
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;

	template<typename... Args>
	static void info(const std::string& fmt, const Args&... args)
	{
		GetInstance().mLogger->info(fmt::runtime(fmt), args...);
	}

	template<typename... Args>
	static void warn(const std::string& fmt, const Args&... args)
	{
		GetInstance().mLogger->warn(fmt::runtime(fmt), args...);
	}

	template<typename... Args>
	static void error(const std::string& fmt, const Args&... args)
	{
		GetInstance().mLogger->error(fmt::runtime(fmt), args...);
	}

	template<typename... Args>
	static void debug(const std::string& fmt, const Args&... args)
	{
		GetInstance().mLogger->debug(fmt::runtime(fmt), args...);
	}
private:
	static std::shared_ptr<spdlog::logger> mLogger;
};

std::shared_ptr<spdlog::logger> Log::mLogger = nullptr;
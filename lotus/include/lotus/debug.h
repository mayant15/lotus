/**
 * Provides debug tools, like logging.
 */

#pragma once

#include <spdlog/spdlog.h>

#define LOG_INFO(...)   spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)   spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)  spdlog::error(__VA_ARGS__)

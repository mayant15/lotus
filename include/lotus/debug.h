/**
 * Provides debug tools, like logging.
 */

#pragma once

// TODO: spdlog as a usage dependency
#include <spdlog/spdlog.h>

#define LOG_INFO(...)   spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)   spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)  spdlog::error(__VA_ARGS__)

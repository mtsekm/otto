/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2024 Sky UK
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef OTTO_LOGGER_H
#define OTTO_LOGGER_H

#include <iostream>
#include <string>

/**
 * Defines the different levels of logging supported.
 */
enum class LogLevel {
    TRACE, ///< Detailed tracing information for debugging.
    DEBUG, ///< General debugging information.
    INFO,  ///< Informative messages about the application's operation.
    WARN,  ///< Warning messages indicating potential issues.
    ERROR  ///< Error messages indicating significant problems.
};

/**
 * Contains configuration settings for the logger.
 */
namespace LoggerConfig {
extern LogLevel currentLevel; ///< Current logging level.

/**
 * Sets the global logging level.
 * 
 * @param level The desired logging level.
 */
void setLogLevel(LogLevel level);
} // namespace LoggerConfig

/**
 * Converts a string representation of a log level to a LogLevel enum.
 * 
 * @param levelStr The string representation of the log level.
 * @return The corresponding LogLevel.
 */
LogLevel stringToLogLevel(const std::string &levelStr);

/**
 * Logs a message at a specified logging level.
 * 
 * @param level The logging level.
 * @param message The message to log.
 */
void logMessage(LogLevel level, const std::string &message);

/**
 * Convenience functions for logging at specific levels.
 */
inline void logTrace(const std::string &message) { logMessage(LogLevel::TRACE, message); }
inline void logDebug(const std::string &message) { logMessage(LogLevel::DEBUG, message); }
inline void logInfo(const std::string &message) { logMessage(LogLevel::INFO, message); }
inline void logWarn(const std::string &message) { logMessage(LogLevel::WARN, message); }
inline void logError(const std::string &message) { logMessage(LogLevel::ERROR, message); }

#endif // OTTO_LOGGER_H

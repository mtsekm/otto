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

#include "WaitExecutor.h"
#include "Logger.h"

#include <chrono>
#include <regex>
#include <thread>

void WaitExecutor::execute(const std::vector<std::string> &args) {
    if (args.size() != 2) {
        logError("Invalid wait command format. Usage: wait <duration>");
        return;
    }

    const std::string &durationStr = args[1];
    int durationMs = parseDuration(durationStr);

    if (durationMs < 0) {
        logError("Invalid duration format: " + durationStr + ". Usage examples: 5s, 2m.");
        return;
    }

    logDebug("Waiting for " + std::to_string(durationMs) + " milliseconds.");
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
}

int WaitExecutor::parseDuration(const std::string &durationStr) const {
    std::regex durationRegex(R"(^(\d+)(ms|s|m)$)");
    std::smatch match;

    if (std::regex_match(durationStr, match, durationRegex)) {
        int value = std::stoi(match[1].str());
        const std::string &unit = match[2].str();

        if (unit == "ms") {
            return value;
        } else if (unit == "s") {
            return value * 1000;
        } else if (unit == "m") {
            return value * 60 * 1000;
        }
    }

    return -1;
}

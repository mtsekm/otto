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

#include "AppExecutor.h"
#include "Logger.h"

#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <thread>

void AppExecutor::execute(const std::vector<std::string> &args) {
    if (args.size() != 2) {
        logError("Invalid command format. Usage: launch_app <app_id> or close_app <app_id>");
        return;
    }

    const std::string &command = args[0];
    const std::string &appId = args[1];
    std::string url;

    if (command == "launch_app") {
        url = "http://127.0.0.1:9005/as/apps/action/launch?appId=" + appId;
        logDebug("Launching app: " + appId);
    } else if (command == "close_app") {
        url = "http://127.0.0.1:9005/as/apps/action/close?appId=" + appId;
        logDebug("Closing app: " + appId);
    } else {
        logError("Unknown command: " + command);
        return;
    }

    try {
        sendHttpRequest(url);
        logDebug("Command executed: " + command + " for app: " + appId);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    } catch (const std::exception &e) {
        logError("Failed to execute command: " + command + " for app: " + appId + ". Error: " + e.what());
    }
}

void AppExecutor::sendHttpRequest(const std::string &url) {
    std::string command = "curl \"" + url + "\" -X POST --data ' '";
    int result = std::system(command.c_str());
    if (result != 0) {
        throw std::runtime_error("HTTP request failed with error code: " + std::to_string(result));
    }
}

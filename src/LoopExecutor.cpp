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

#include "LoopExecutor.h"
#include "Logger.h"

#include <stdexcept>

LoopExecutor::LoopExecutor(std::shared_ptr<CommandExecutor> executor) : executor(std::move(executor)) {}

void LoopExecutor::execute(const std::vector<std::string> &args) {
    if (args.empty()) {
        logError("Invalid loop command. Usage: loop_start <count> or loop_end.");
        return;
    }

    const std::string &command = args[0];

    if (command == "loop_start") {
        if (args.size() != 2) {
            logError("Invalid loop_start command. Usage: loop_start <count>");
            return;
        }

        try {
            int count = std::stoi(args[1]);
            if (count <= 0) {
                logError("Loop count must be greater than 0.");
                return;
            }

            loopStack.push({executor->getCurrentCommandIndex(), count});
            logDebug("Loop started. Current index: " + std::to_string(executor->getCurrentCommandIndex()) +
                     ", Count: " + std::to_string(count) + ", Loop stack size: " + std::to_string(loopStack.size()));
        } catch (const std::exception &e) {
            logError("Invalid loop count. Usage: loop_start <count>");
        }
    } else if (command == "loop_end") {
        if (loopStack.empty()) {
            logError("loop_end encountered without matching loop_start.");
            return;
        }

        auto &[startIndex, remainingCount] = loopStack.top();
        if (--remainingCount > 0) {
            logDebug("Loop iteration remaining: " + std::to_string(remainingCount) + ", Returning to index: " +
                     std::to_string(startIndex) + ", Loop stack size: " + std::to_string(loopStack.size()));
            executor->setCommandIndex(startIndex);
        } else {
            loopStack.pop();
            logDebug("Loop completed. Loop stack size: " + std::to_string(loopStack.size()));
        }
    } else {
        logError("Unknown loop command: " + command);
    }
}

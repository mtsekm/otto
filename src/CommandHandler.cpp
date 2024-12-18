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

#include "CommandHandler.h"
#include "Logger.h"

#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>

CommandHandler::CommandHandler(std::shared_ptr<CommandExecutor> executor) : executor(std::move(executor)) {}

void CommandHandler::parseFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        logError("Failed to open commands file: " + filePath);
        throw std::runtime_error("Could not open commands file.");
    }

    logDebug("Parsing commands file: " + filePath);

    std::vector<std::vector<std::string>> parsedCommands;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens((std::istream_iterator<std::string>(iss)),
                                        std::istream_iterator<std::string>());

        if (!tokens.empty()) {
            parsedCommands.push_back(tokens);
        }
    }

    logDebug("Parsed " + std::to_string(parsedCommands.size()) + " commands.");
    executor->setParsedCommands(parsedCommands);
}

void CommandHandler::executeCommands() {
    logDebug("Executing commands...");
    executor->executeAll();
    logDebug("All commands executed successfully.");
}

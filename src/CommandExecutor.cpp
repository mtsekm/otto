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

#include "CommandExecutor.h"
#include "Logger.h"

CommandExecutor::CommandExecutor(std::unordered_map<std::string, std::string>& variables)
    : variables(variables) {}

void CommandExecutor::registerCommand(const std::string &command, std::shared_ptr<BaseExecutor> executor) {
    if (executors.find(command) != executors.end()) {
        logWarn("Command already registered: " + command + ". Overwriting.");
    }
    executors[command] = std::move(executor);
    logDebug("Registered command: " + command);
}

std::shared_ptr<BaseExecutor> CommandExecutor::getExecutor(const std::string &command) {
    auto it = executors.find(command);
    if (it != executors.end()) {
        return it->second;
    }
    logError("Executor not found for command: " + command);
    return nullptr;
}

void CommandExecutor::execute(const std::vector<std::string> &args) {
    if (args.empty()) {
        logError("No command provided for execution.");
        return;
    }

    const std::string &command = args[0];
    auto executor = getExecutor(command);
    if (!executor) {
        logError("Unsupported command: " + command);
        return;
    }

    logDebug("Executing command: " + command);

    std::vector<std::string> resolvedArgs = resolveVariables(args);
    executor->execute(resolvedArgs);
}

void CommandExecutor::setParsedCommands(const std::vector<std::vector<std::string>> &commands) {
    parsedCommands = commands;
    currentCommandIndex = 0;
    logDebug("Parsed commands set successfully. Total commands: " + std::to_string(commands.size()));
}

void CommandExecutor::executeAll() {
    while (currentCommandIndex < parsedCommands.size()) {
        execute(parsedCommands[currentCommandIndex]);
        ++currentCommandIndex;
    }
}

size_t CommandExecutor::getCurrentCommandIndex() const { return currentCommandIndex; }

void CommandExecutor::setCommandIndex(size_t index) {
    if (index < parsedCommands.size()) {
        currentCommandIndex = index;
    } else {
        logError("Invalid command index: " + std::to_string(index));
        throw std::out_of_range("Command index out of range");
    }
}

std::vector<std::string> CommandExecutor::resolveVariables(const std::vector<std::string> &args) const {
    std::vector<std::string> resolvedArgs = args;

    for (auto &arg : resolvedArgs) {
        if (!arg.empty() && arg[0] == '$') {
            std::string varName = arg.substr(1);
            auto it = variables.find(varName);
            if (it != variables.end()) {
                arg = it->second;
            } else {
                logError("Undefined variable: " + varName);
            }
        }
    }

    return resolvedArgs;
}
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

#ifndef OTTO_COMMANDEXECUTOR_H
#define OTTO_COMMANDEXECUTOR_H

#include "BaseExecutor.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * CommandExecutor maps commands to their corresponding executors.
 */
class CommandExecutor {
public:
    /**
     * Constructor for CommandExecutor.
     * 
     * @param variables A reference to the shared variables map.
     */
    explicit CommandExecutor(std::unordered_map<std::string, std::string>& variables);

    /**
     * Registers a command with its corresponding executor.
     *
     * @param command The name of the command (e.g., "var").
     * @param executor A shared pointer to the executor for the command.
     */
    void registerCommand(const std::string &command, std::shared_ptr<BaseExecutor> executor);

    /**
     * Retrieves the executor for a specific command.
     *
     * @param command The name of the command.
     * @return A shared pointer to the executor if found, otherwise nullptr.
     */
    std::shared_ptr<BaseExecutor> getExecutor(const std::string &command);

    /**
     * Executes a command by delegating it to the appropriate executor.
     *
     * @param args The arguments for the command, where args[0] is the command name.
     */
    void execute(const std::vector<std::string> &args);

    /**
     * Sets the parsed commands for execution.
     *
     * @param commands The list of commands.
     */
    void setParsedCommands(const std::vector<std::vector<std::string>> &commands);

    /**
     * Executes all parsed commands from the current index.
     */
    void executeAll();

    /**
     * Retrieves the current command index.
     *
     * @return The index of the current command.
     */
    size_t getCurrentCommandIndex() const;

    /**
     * Sets the index of the command to be executed next.
     *
     * @param index The index of the command to execute next.
     */
    void setCommandIndex(size_t index);

    /**
     * Resolves variables in the given command arguments.
     */
    std::vector<std::string> resolveVariables(const std::vector<std::string>& args) const;

private:
    size_t currentCommandIndex = 0;                                           // Tracks the currently executing command
    std::vector<std::vector<std::string>> parsedCommands;                     // List of parsed commands
    std::unordered_map<std::string, std::shared_ptr<BaseExecutor>> executors; // Use shared_ptr
    std::unordered_map<std::string, std::string>& variables;                  // Reference to the shared variables map
};

#endif // OTTO_COMMANDEXECUTOR_H

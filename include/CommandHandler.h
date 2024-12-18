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

#ifndef OTTO_COMMANDHANDLER_H
#define OTTO_COMMANDHANDLER_H

#include "CommandExecutor.h"

#include <memory>
#include <string>
#include <vector>

/**
 * CommandHandler parses a commands file and delegates command execution
 * to the CommandExecutor.
 */
class CommandHandler {
public:
    /**
     * Constructs a CommandHandler with a shared CommandExecutor.
     * 
     * @param executor The CommandExecutor instance for delegating commands.
     */
    CommandHandler(std::shared_ptr<CommandExecutor> executor);

    /**
     * Parses a file containing commands.
     * 
     * @param filePath The path to the commands file.
     */
    void parseFile(const std::string &filePath);

    /**
     * Executes all parsed commands.
     */
    void executeCommands();

private:
    std::shared_ptr<CommandExecutor> executor;
};

#endif // OTTO_COMMANDHANDLER_H

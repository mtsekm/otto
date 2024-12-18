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

#ifndef OTTO_LOOP_EXECUTOR_H
#define OTTO_LOOP_EXECUTOR_H

#include "BaseExecutor.h"
#include "CommandExecutor.h"

#include <memory>
#include <stack>
#include <utility>

/**
 * The `LoopExecutor` manages the execution of loops in the command sequence.
 */
class LoopExecutor : public BaseExecutor {
public:
    explicit LoopExecutor(std::shared_ptr<CommandExecutor> executor);

    /**
     * Executes loop-related commands.
     */
    void execute(const std::vector<std::string> &args) override;

private:
    std::shared_ptr<CommandExecutor> executor; ///< Shared pointer to the CommandExecutor for delegating commands.
    std::stack<std::pair<int, int>> loopStack; ///< Stack to manage nested loops.
};

#endif // OTTO_LOOP_EXECUTOR_H

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

#ifndef OTTO_VARIABLEEXECUTOR_H
#define OTTO_VARIABLEEXECUTOR_H

#include "BaseExecutor.h"
#include <string>
#include <unordered_map>

/**
 * Executor for handling variable commands (e.g., "var x 5").
 */
class VariableExecutor : public BaseExecutor {
public:
    /**
     * Constructs a VariableExecutor with access to a shared variable map.
     * 
     * @param variables Reference to a map where variables are stored.
     */
    VariableExecutor(std::unordered_map<std::string, std::string> &variables);

    /**
     * Executes the "var" command to define or update a variable.
     * 
     * @param args The arguments for the command (e.g., ["var", "x", "5"]).
     */
    void execute(const std::vector<std::string> &args) override;

private:
    std::unordered_map<std::string, std::string> &variables;
};

#endif // OTTO_VARIABLEEXECUTOR_H

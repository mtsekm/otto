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

#include "VariableExecutor.h"
#include "Logger.h"

VariableExecutor::VariableExecutor(std::unordered_map<std::string, std::string> &vars) : variables(vars) {}

void VariableExecutor::execute(const std::vector<std::string> &args) {
    if (args.size() != 3) {
        logError("Invalid variable command format. Usage: var <name> <value>");
        return;
    }

    const std::string &name = args[1];
    const std::string &value = args[2];

    if (name.empty() || name.find('$') != std::string::npos) {
        logError("Invalid variable name: " + name);
        return;
    }

    variables[name] = value;
    logDebug("Variable set: " + name + " = " + value);
}

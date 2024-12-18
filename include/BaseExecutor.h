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

#ifndef OTTO_BASEEXECUTOR_H
#define OTTO_BASEEXECUTOR_H

#include <string>
#include <vector>

/**
 * Abstract base class for command executors.
 */
class BaseExecutor {
public:
    virtual ~BaseExecutor() = default;

    /**
     * Executes the command with the provided arguments.
     * 
     * @param args The arguments for the command, where args[0] is the command name.
     */
    virtual void execute(const std::vector<std::string> &args) = 0;
};

#endif // OTTO_BASEEXECUTOR_H

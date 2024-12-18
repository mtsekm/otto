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

#ifndef OTTO_WAITEXECUTOR_H
#define OTTO_WAITEXECUTOR_H

#include "BaseExecutor.h"
#include <string>
#include <vector>

/**
 * WaitExecutor handles "wait" commands to introduce delays.
 */
class WaitExecutor : public BaseExecutor {
public:
    /**
     * Executes the "wait" command.
     * 
     * @param args The arguments for the command (e.g., ["wait", "5s"]).
     */
    void execute(const std::vector<std::string> &args) override;

private:
    /**
     * Parses a duration string and converts it to milliseconds.
     * 
     * @param durationStr The duration string (e.g., "5s", "2m").
     * @return The equivalent duration in milliseconds, or -1 if invalid.
     */
    int parseDuration(const std::string &durationStr) const;
};

#endif // OTTO_WAITEXECUTOR_H

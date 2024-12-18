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

#ifndef OTTO_APPEXECUTOR_H
#define OTTO_APPEXECUTOR_H

#include "BaseExecutor.h"

#include <string>
#include <vector>

/**
 * Executes commands to launch and close applications using HTTP requests.
 */
class AppExecutor : public BaseExecutor {
public:
    AppExecutor() = default;

    /**
     * Executes the launch or close app commands.
     *
     * @param args Command arguments where args[0] is the command name,
     *             and args[1] is the app ID.
     */
    void execute(const std::vector<std::string> &args) override;

private:
    /**
     * Sends an HTTP request using `curl` to the specified URL.
     *
     * @param url The URL for the HTTP request.
     */
    void sendHttpRequest(const std::string &url);
};

#endif // OTTO_APPEXECUTOR_H

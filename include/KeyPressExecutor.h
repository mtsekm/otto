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

#ifndef OTTO_KEYPRESSEXECUTOR_H
#define OTTO_KEYPRESSEXECUTOR_H

#include "BaseExecutor.h"
#include "KeyManager.h"

#include <string>
#include <vector>

/**
 * KeyPressExecutor handles "key_press" commands to send key events.
 */
class KeyPressExecutor : public BaseExecutor {
public:
    /**
     * Constructs a KeyPressExecutor with a reference to the KeyManager.
     *
     * @param keyManager The KeyManager instance for sending key events.
     */
    KeyPressExecutor(KeyManager &keyManager);

    /**
     * Executes the "key_press" command.
     *
     * @param args The arguments for the command (e.g., ["key_press", "power", "3"]).
     */
    void execute(const std::vector<std::string> &args) override;

private:
    KeyManager &keyManager;
};

#endif // OTTO_KEYPRESSEXECUTOR_H

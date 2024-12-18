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

#ifndef OTTO_IARMUTILS_H
#define OTTO_IARMUTILS_H

#include <functional>

/**
 * Utility functions for IARM initialization and event handling.
 */
namespace IARMUtils {
/**
 * Initialize the IARM bus with a given name.
 *
 * @param name The application name to register with IARM.
 * @return True if initialization was successful, otherwise false.
 */
bool initialize(const char *name);

/**
 * Disconnect and terminate the IARM bus.
 */
void terminate();

/**
 * Send a key event through the IRmgr's UInput dispatcher.
 *
 * @param keyType The type of the key event (e.g., press, release, repeat).
 * @param keyCode The key code of the event.
 */
void sendKeyEvent(int keyType, int keyCode);

/**
 * Register an event handler to process incoming IR key events.
 *
 * @param handler A callback function to handle IR key events.
 *                It receives keyType and keyCode as parameters.
 */
void registerIRKeyHandler(const std::function<void(int keyType, int keyCode)> &handler);

} // namespace IARMUtils

#endif // OTTO_IARMUTILS_H

/* Copyright 2020, Edwin Chiu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef VERSION_H
#define VERSION_H

// TODO(jlebar): Move this constant into a cpp file, otherwise we're duplicating
// it.
/* Len must be 8 bytes (excluding NULL) */
static const char SW_VERSION[] = "01.01.01";

char *version_getVersion();

#endif // VERSION_H

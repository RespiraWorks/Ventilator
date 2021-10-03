/* Copyright 2020-2021, RespiraWorks

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

#pragma once

#include <QCoreApplication>
#include <QtTest>

#include "gui_state_container.h"

class GuiStateContainerTest : public QObject {
  Q_OBJECT
 public:
  GuiStateContainerTest() = default;
  ~GuiStateContainerTest() = default;

 private slots:
  void initTestCase() {}
  void cleanupTestCase() {}

  void testInit() { GuiStateContainer ch(DurationMs(0), DurationMs(0)); }
};

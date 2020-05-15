#pragma once

#include <limits>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <VentilatorUI/surfacecontroller.hpp>

using namespace testing;

class SurfaceControllerInterfaceTest : public Test
{
    // Test interface
protected:
    void SetUp();
    void TearDown();

    VentilatorUI::SurfaceController *iface;
};

void SurfaceControllerInterfaceTest::SetUp()
{
    iface = new VentilatorUI::SurfaceController;
}

void SurfaceControllerInterfaceTest::TearDown()
{
    delete iface;
}

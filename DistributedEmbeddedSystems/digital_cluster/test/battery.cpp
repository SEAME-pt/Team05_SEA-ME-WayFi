#include <gtest/gtest.h>
#include <QApplication>
#include <QDebug>
#include "../include/battery.h"

class BatteryTest : public testing::Test {
protected:
    Battery* dial;
    void SetUp() override {  // new before each test
        dial = new Battery();
    }
    void TearDown() override { //delete after each test
        delete dial;
    }
};
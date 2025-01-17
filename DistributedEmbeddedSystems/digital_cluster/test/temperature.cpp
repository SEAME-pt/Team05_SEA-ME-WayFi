#include <gtest/gtest.h>
#include <QApplication>
#include <QDebug>
#include "../include/temperature.h"

class TemperatureTest : public testing::Test {
protected:
    Temperature* temp;
    void SetUp() override {  // new before each test
        temp = new Temperature();
    }
    void TearDown() override { //delete after each test
        delete temp;
    }
};


#include <gtest/gtest.h>
#include <QApplication>
#include <QDebug>
#include "../include/temperature.h"

class TemperatureT : public testing::Test {
protected:
    Temperature* temp;
    void SetUp() override {  // new before each test
        temp = new Temperature();
    }
    void TearDown() override { //delete after each test
        delete temp;
    }
};

//testing sections
// TEST_F(TemperatureT, ) {
//     temp->set_temperature(50);
    
// }
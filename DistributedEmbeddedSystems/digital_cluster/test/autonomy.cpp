#include <gtest/gtest.h>
#include <QApplication>
#include <QDebug>
#include "../include/autonomy.h"

class AutonomyTest : public testing::Test {
protected:
    Autonomy* bar;
    void SetUp() override {  // new before each test
        bar = new Autonomy();
    }
    void TearDown() override { //delete after each test
        delete bar;
    }
};


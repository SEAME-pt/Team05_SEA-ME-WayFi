#include <gtest/gtest.h>
#include <QApplication>
#include <QDebug>
#include "../include/battery.h"
#include "../include/mainwindow.h"
#include <memory>

class BatteryT : public testing::Test {
protected:
    Battery* battery;
    MockQPainter painter;
    MainWindow *mw;
    void SetUp() override {
        battery = mw->get_battery();
    }
    void TearDown() override {
        delete battery;
    }
};

TEST_F(BatteryT, TestBatteryState) {
    battery->set_current(50);
    std::cout << "Testing BatteryState, current = " << battery->get_current() << std::endl;
    EXPECT_EQ(battery->get_current(), 50); 

    EXPECT_CALL(painter, drawText(testing::_, testing::_, testing::_)).Times(testing::AtLeast(1));
    EXPECT_CALL(painter, drawPixmap(testing::_, testing::_)).Times(testing::AtLeast(1));
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

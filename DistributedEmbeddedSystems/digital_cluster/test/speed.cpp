#include <gtest/gtest.h>
#include <QApplication>
#include <QDebug>
#include "../include/speed.h"
#include <QTimer>
#include <QEventLoop>

class SpeedTest : public testing::Test {
protected:
    Speed* speed;
    void SetUp() override {  // new before each test
        speed = new Speed();
    }
    void TearDown() override { //delete after each test
        delete speed;
    }
};

TEST_F(SpeedTest, TestSetCurrent) {
    speed->set_current(8);
    EXPECT_FLOAT_EQ(speed->get_current(), static_cast<int>(8 * 3.6));
}

TEST_F(SpeedTest, TestSetCurrentTargetAngle) {
    speed->set_current(8);
    float expected_target_angle = (8 * 3.6 * 270.0f) / speed->get_max();
    expected_target_angle = std::min(expected_target_angle, 270.0f);
    EXPECT_FLOAT_EQ(speed->get_target_angle(), expected_target_angle);
}

TEST_F(SpeedTest, TestAnimationStart) {
    speed->set_current(8);
    EXPECT_TRUE(speed->get_is_animating());
}

TEST_F(SpeedTest, TestPaintText) {
    QPixmap pixmap(speed->size());
    QPainter painter(&pixmap); 
    speed->paint_text(painter);  // Test if paint_text doesn't crash.
}

TEST_F(SpeedTest, TestAnimationUpdate) {
    speed->set_current(3);  
    QTimer timer;
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(200);
    loop.exec();
    qDebug() << "current angle in tests" << speed->get_current_angle();
    EXPECT_TRUE(speed->get_current_angle() > 0);  
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include "tempbar.h"

TempBar::TempBar(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 200); 
    QVBoxLayout* main_layout = new QVBoxLayout(this); 
    main_layout->setSpacing(18); 

    layout = new QHBoxLayout();
    layout->setSpacing(1); 
    nb_sections = 10;
    for (int i = 0; i < nb_sections; ++i) {
        QWidget *section = new QWidget(this);
        section->setFixedSize(20, 30); //rectangular section
        layout->addWidget(section);
        sections.append(section);
    }
    main_layout->addLayout(layout, 1);
    set_temperature(50);
    QLabel *label = new QLabel(this);
    QFont font("Noto Sans");
    label->setFont(font);
    label->setText("🌡️");
    label->setStyleSheet("font-size: 28px; color: rgb(0, 120, 140);");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setContentsMargins(10, 0, 0, 0);
    main_layout->addWidget(label);
    setLayout(main_layout);
}

TempBar::~TempBar() {
    delete layout;
}

void TempBar::set_temperature(int temp) {
    current = temp;
    int sections_color = static_cast<int>((current / 100.0) * nb_sections);
    for (int i = 0; i < nb_sections; ++i) {
        if (i < sections_color) {
            QColor sectionColor;
            if (current < 70) {
                int blue_value = 170 + (i * (150 / nb_sections));  //dim blueincrease to bright blue
                int green_value = 80 + (i * (100 / nb_sections)); //from dim cyan to regular cyan
                sectionColor.setRgb(0, green_value, blue_value);
            } else {
                int red_value = (i * (255 / nb_sections));         //increase red component
                int green_value = 80 + (i * (8 / nb_sections));  //decrease green
                int blue_value = 130 - (i * (90 / nb_sections));
                sectionColor.setRgb(red_value, green_value, blue_value);
            }
            sections[i]->setStyleSheet(QString("background-color: %1").arg(sectionColor.name()));
        } else {
            QColor inactive(22, 32, 90); //dim cyan color
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive.name()));
        }
    }
    // QPixmap pixmap(":/resources/thermometer.png");
    // QLabel *iconLabel = new QLabel(this);
    // iconLabel->setPixmap(pixmap);
}

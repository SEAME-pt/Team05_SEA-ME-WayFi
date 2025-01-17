#include "../include/temperature.h"

Temperature::Temperature(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 200); 
    main_layout = new QVBoxLayout(); 
    main_layout->setSpacing(10); 

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
    label = new QLabel(this);
    set_temperature(50);
    setLayout(main_layout); //setting layout for qwidget (no need to delete them)
}

Temperature::~Temperature() {
}

void Temperature::set_text(int temp) {
    label->setTextFormat(Qt::RichText);
    label->setText("<span style='font-family: Noto Sans; font-size: 25px; color: rgb(0, 120, 140);'>🌡️&nbsp;&nbsp;</span>"
        "<span style='font-family: Digital-7; font-size: 25px; color: rgb(0, 120, 140);'>" + 
        QString::number(temp) + "</span>"
        "<span style='font-family: Calculator; font-size: 23px; color: rgb(0, 120, 140);'> °C</span>");
    label->setContentsMargins(10, 0, 0, 0);
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    main_layout->addWidget(label);
}

void Temperature::set_temperature(int temp) {
    int sections_color = static_cast<int>((temp / 80.0) * nb_sections);
    for (int i = 0; i < nb_sections; ++i) {
        if (i < sections_color) {
            QColor sectionColor;
            if (temp < 60) {
                int blue = 170 + (i * (150 / nb_sections));  //dim blueincrease to bright blue
                int green = 80 + (i * (160 / nb_sections)); //from dim cyan to regular cyan
                sectionColor.setRgb(0, green, blue);
            } else {
                int red = (i * (240 / nb_sections));         //increase red component
                int green = 80 + (i * (10 / nb_sections));  //decrease green
                int blue = 130 - (i * (90 / nb_sections));
                sectionColor.setRgb(red, green, blue);
            }
            sections[i]->setStyleSheet(QString("background-color: %1").arg(sectionColor.name()));
        } else {
            QColor inactive(22, 32, 90); //dim cyan color
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive.name()));
        }
    }
    set_text(temp);
}

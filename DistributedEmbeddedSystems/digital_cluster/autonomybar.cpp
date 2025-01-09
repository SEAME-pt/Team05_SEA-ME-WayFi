#include "autonomybar.h"

AutonomyBar::AutonomyBar(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 100); 
    QVBoxLayout * main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(10);
    layout = new QHBoxLayout();
    layout->setSpacing(1); 
    layout->setContentsMargins(0, 0, 0, 0); 
    nb_sections = 10;
    for (int i = 0; i < nb_sections; ++i) {
        QWidget *section = new QWidget(this);
        section->setFixedSize(20, 30); //rectangular section
        layout->addWidget(section);
        sections.append(section);
    }
    main_layout->addLayout(layout);
    set_autonomy(100);
}

AutonomyBar::~AutonomyBar() {
    delete layout;
}

void AutonomyBar::set_autonomy(int aut) {
    current = aut;
    int sections_color = static_cast<int>((current / 100.0) * nb_sections);
    for (int i = 0; i < nb_sections; ++i) {
        if (i < sections_color) {
            QColor sectionColor;
            if (current > 100) {
                int blue_value = 170 + (i * (150 / nb_sections));  
                int green_value = 80 + (i * (100 / nb_sections)); 
                sectionColor.setRgb(0, green_value, blue_value);
            } else {
                int red_value = (i * (255 / nb_sections));         
                int green_value = 80 + (i * (8 / nb_sections)); 
                int blue_value = 130 - (i * (90 / nb_sections));
                sectionColor.setRgb(red_value, green_value, blue_value);
            }
            sections[i]->setStyleSheet(QString("background-color: %1").arg(sectionColor.name()));
        } else {
            QColor inactiveColor(22, 32, 90); 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactiveColor.name()));
        }
    }
    // QPixmap pixmap(":/resources/.png");
    // QLabel *iconLabel = new QLabel(this);
    // iconLabel->setPixmap(pixmap);
    QLabel *label = new QLabel(this);
    label->setText("km");
    label->setStyleSheet("font-size: 30px; color: rgb(0, 120, 140);");
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setContentsMargins(0, 0, 10, 0);
    main_layout->addWidget(label);
}

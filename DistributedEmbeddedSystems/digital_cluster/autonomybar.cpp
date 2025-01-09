#include "autonomybar.h"

AutonomyBar::AutonomyBar(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 200); 
    main_layout = new QVBoxLayout(this);
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
    main_layout->addLayout(layout);
    set_autonomy(60);
}

AutonomyBar::~AutonomyBar() {
    delete layout;
}

void AutonomyBar::set_autonomy(int aut) {
    current = aut;
    int sections_color = static_cast<int>((current / 120.0) * nb_sections);
    for (int i = 0; i < nb_sections; ++i) {
        if (i < sections_color) {
            QColor sectionColor;
            if (current > 60) {
                int red_value = 180 - (i * (200 / nb_sections)); 
                int blue_value = 50 + (i * (160 / nb_sections));  
                int green_value = 90 + (i * (150 / nb_sections)); 
                sectionColor.setRgb(red_value, green_value, blue_value);
            } else {
                int red_value = 200 - (i * (255 / nb_sections));         
                int green_value = 100 + (i * (180 / nb_sections)); 
                int blue_value = 60 + (i * (255 / nb_sections));
                sectionColor.setRgb(red_value, green_value, blue_value);
            } 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(sectionColor.name()));
        } else {
            QColor inactiveColor(22, 32, 90); 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactiveColor.name()));
        }
    }
    QLabel *label = new QLabel(this);
    label->setFont(QFont("Calculator", 30));
    label->setText(QString("km"));
    label->setStyleSheet("color: rgb(0, 120, 140);");
    label->setAlignment(Qt::AlignTop | Qt::AlignRight);
    label->setContentsMargins(0, 0, 10, 0);
    main_layout->addWidget(label);
}

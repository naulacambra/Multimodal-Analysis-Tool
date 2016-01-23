#include "Headers/styles.h"

void setButtonStyle_ok(QPushButton *b){

    b->setStyleSheet("QPushButton{background-color:#0089c7;border-radius:10px;padding:5px;color:#FFFFFF}"
                              "QPushButton:hover{background-color:#20a9e7;}"
                              "QPushButton:pressed{border: 1px solid #FFF;color:#EEE}");
}

void setButtonStyle_cancel(QPushButton *b){

    b->setStyleSheet("QPushButton{background-color:#AAAAAA;border-radius:10px;padding:5px;}"
                              "QPushButton:hover{background-color:#BBBBBB;}"
                              "QPushButton:pressed{border: 1px solid #FFF;color:#EEE}");
}


void setButtonStyle_save(QPushButton *b){

    b->setStyleSheet("QPushButton{background-color:#76ea23;border-radius:10px;padding:5px;}"
                              "QPushButton:hover{background-color:#97fc6c;}"
                              "QPushButton:pressed{border: 1px solid #FFF;color:#EEE}");
}


void setButtonStyle_plus(QPushButton *b){

    b->setStyleSheet("QPushButton{background-color:#BBBBBB;border-radius:8px;padding:7px 10px 7px 10px;}"
                              "QPushButton:hover{background-color:#AAAAAA;}"
                              "QPushButton:pressed{border: 1px solid #FFF;color:#EEE}");
}

void setButtonStyle_return(QPushButton *b){

    b->setStyleSheet("QPushButton{background-color:#BBBBBB;border-radius:2px;padding:2px;}"
                              "QPushButton:hover{background-color:#AAAAAA;}"
                              "QPushButton:pressed{border: 1px solid #FFF;color:#EEE}");
}



// Widget

void setScrollStyle(QScrollArea *s){

    s->setStyleSheet("QScrollArea{border: 0px solid #FFF;}");
}







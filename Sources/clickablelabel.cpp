#include "Headers/clickablelabel.h"

ClickableLabel::ClickableLabel(visualizeGL *gl, QMediaPlayer* p,customslider* s, int t, int i, QWidget * parent ) :
QLabel(parent)
{

    if(t==-1){
        type = 0;
        vgl = gl;

    } else {
//        this->_player = p;
//        this->slider = s;
//        this->timestamp = t;

        vgl = gl;
        type = 1;
        index = i;
    }

    connect( this, SIGNAL( clicked() ), this, SLOT( labelClicked() ) );
    //this->setText(text);

}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
  emit clicked();
}

void ClickableLabel::labelClicked(){

    //Mostrar label, text area, button para aplicar cambios

    if(type == 0){
        QColor color = QColorDialog::getColor(Qt::green, this,"Select color",QColorDialog::ShowAlphaChannel);

        if (color.isValid())
        {
            this->setAutoFillBackground(true);
            this->setPalette(color);

            vgl->colors.push_back(color);
            vgl->pick = true;
        }


    } else {       

        QColor color = QColorDialog::getColor(Qt::green, this,"Select color",QColorDialog::ShowAlphaChannel);

        if (color.isValid())
        {
            this->setAutoFillBackground(true);
            this->setPalette(color);

            //Cambiar vector de colores de GLWidget
            vgl->gl[0]->colors2D[index] = color;

        }


//        slider->setMaximum(slider->size);
//        slider->setValue(timestamp);
//        _player->setPosition(timestamp);
    }
}

#include "defines.h"
#include "xf.h"
#include "ui.h"


UI::UI(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("User Interface");
    setFixedWidth(200);
    setFixedHeight(100);

    b = new QPushButton(this);
    b->setGeometry(30,20,20,20);
    this->connect(b,SIGNAL(pressed()),this,SLOT(onButtonDown()));
    this->connect(b,SIGNAL(released()),this,SLOT(onButtonUp()));

    for (int i=0; i<4; i++)
    {
         ledStatus[i] = false;
    }
}

void UI::onButtonDown()
{
    XF_ISR(IFBDOWN);
}

void UI::onButtonUp()
{
    XF_ISR(IFBUP);
}

void UI::setLed(unsigned char thisLed, bool status)
{
    ledStatus[thisLed] = status;
    repaint();
}


void UI::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    int x = 30;
    int y = 60;
    int w= 20, h = 20;


    for (int i=0; i<4; i++)
    {
        switch (i)
        {
            case 0:
                x = 30;
                break;
            case 1:
                x = 70;
                break;
            case 2:
                x = 110;
                break;
            case 3:
                x = 150;
                break;
        }
        if (ledStatus[i] == false)
        {
            p.setBrush(Qt::gray);
        }
        else
        {
            p.setBrush(Qt::red);
        }
        p.drawEllipse(x,y,w,h);
    }
}

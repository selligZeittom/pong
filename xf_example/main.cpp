#include <QApplication>
#include "defines.h"
#include "xf.h"
#include "application.h"
#include "ui.h"

App* myApp;
UI* ui;

extern "C" void setLed(unsigned char, bool);

void setLed(unsigned char thisLed, bool status)
{
    ui->setLed(thisLed, status);
}

class Ticker :public QObject
{
private:
    int tickEventId;
    int mainEventId;
    public:
    Ticker()
    {
        tickEventId = this->startTimer(TICKINTERVAL);
        //start a kind of background main process
        mainEventId = this->startTimer( 10 );
    }

    protected:
    void timerEvent(QTimerEvent *event)
    {
        Event ev;
        if ( event->timerId() == tickEventId )
            XF_ISR(IFTMR1);
        else if ( event->timerId() == mainEventId )
        {
            //this is the main loop
            ev = XF_popEvent(false);
            App_SM1(myApp, ev);
            App_SM2(myApp, ev);
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //quit();
    //a.quitOnLastWindowClosed();
    Ticker t;
    ui = new UI();
    ui->show();
    myApp = new App();
    App_init(myApp);
    return a.exec();
}

#ifndef UI_H
#define UI_H




#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>


class UI : public QWidget
{
    private:
    Q_OBJECT
    QPushButton* b;
    bool ledStatus[4];
    public:
    explicit UI(QWidget *parent = 0);
    void setLed(unsigned char thisLed, bool status);
    void paintEvent(QPaintEvent* event);

    signals:

    public slots:
    void onButtonDown();
    void onButtonUp();
};

#endif // UI_H

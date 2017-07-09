#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    ~Widget();
    void preparePixmap();
protected:
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
private:
    QTimeLine m_animator;
    QPixmap m_pixmap;
    QPixmap m_lastPixmap;
    QPixmap pixlist[2];
    int pixindex;
};

#endif // WIDGET_H

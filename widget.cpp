#include "widget.h"
#include <QtGui>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    connect(&m_animator, SIGNAL(frameChanged(int)), SLOT(update()));
    m_animator.setFrameRange(0, 200);
    m_animator.setDuration(1000);
    m_animator.setCurveShape(QTimeLine::EaseInOutCurve);
    pixlist[0]=QPixmap(":/images/2.jpg");
    pixlist[1]=QPixmap(":/images/3.png");
    pixindex=0;
    m_pixmap=pixlist[pixindex++];
    pixindex%=2;
}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
#if !defined(Q_OS_SYMBIAN) && !defined(Q_OS_WINCE_WM)
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.setRenderHint(QPainter::Antialiasing, true);
#endif
    p.fillRect(rect(), Qt::black);

    int hw = width() / 2;
    int hh = height() / 2;

    // behind is the new pixmap
    int pad = width() / 10;
    QRect fr = rect().adjusted(pad, pad, -pad, -pad);
    p.drawPixmap(0,0,m_pixmap);

    int index = m_animator.currentFrame();

    // qDebug()<<"Animator CurrentFrame:"<<index;
    if (index <= 100) {
        //        // the top part of the old pixmap is flipping
        int angle = -180 * index / 200;
        QTransform transform;
        transform.translate(hw, hh);
        transform.rotate(angle, Qt::YAxis);
        p.setTransform(transform);
        p.drawPixmap(-(hw), -hh, m_lastPixmap);

        // the bottom part is still the old pixmap
        p.resetTransform();
        p.setClipRect((hw), 0, hw, width());
        p.drawPixmap(0, 0, m_lastPixmap);
    } else {
        p.setClipRect(hw, 0, hw, width());

        // the bottom part is still the old pixmap
        p.drawPixmap(0, 0, m_lastPixmap);

        // the bottom part of the new pixmap is flipping
        int angle = 180 - 180 * m_animator.currentFrame() / 200;
        QTransform transform;
        transform.translate(hw, hh);
        transform.rotate(angle, Qt::YAxis);
        p.setTransform(transform);
        p.drawPixmap(-(hw), -hh, m_pixmap);
    }
}

void  Widget::preparePixmap() {
    m_pixmap=pixlist[pixindex++];
    pixindex%=2;
}


void Widget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_lastPixmap = m_pixmap;
        preparePixmap();
        m_animator.stop();
        m_animator.start();
        event->accept();
    }
}

void Widget::resizeEvent(QResizeEvent*) {
    preparePixmap();
    update();
}



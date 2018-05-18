/*
 * @author: wenht
 * @date: 2018-5-18 11:22:59
 * @version: 1.0
 * @brief: To achieve the borderless form to minimize, maximize, restore, close, drag and other functions
 *
 **/

#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QDesktopWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget

{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QPushButton *m_closeBtn;
    QPushButton *m_normalBtn;
    QPushButton *m_minBtn;
    QPoint m_mousePos;
    QPoint m_windowPos;
    QRect clientRect;
    QDesktopWidget* desktop;
    QRect *rect;
    int lengthToMouse;
    double ratio; // 比例
    bool isMaxWindow;
    bool m_mousePressed;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void updateNormalBtn();

public slots:
    void maxRestoreSlot();
};

#endif // WIDGET_H

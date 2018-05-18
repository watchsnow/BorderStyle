/*

 * @author: wenht
 * @date: 2018-5-18 11:22:59
 * @version: 1.0
 * @brief: To achieve the borderless form to minimize, maximize, restore, close, drag and other functions
 *
 **/
#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setMinimumSize(800,600);
    this->setWindowFlags(Qt::FramelessWindowHint); // 隐藏标题栏
    isMaxWindow = false;
    clientRect = QApplication::desktop()->availableGeometry(); //用户可用窗口大小
    rect = new QRect(0, 0, clientRect.width(), 30); // 鼠标点击的有效区域
    m_minBtn = new QPushButton(tr("最小化"),this);
    m_normalBtn = new QPushButton(tr("最大化"),this);
    m_closeBtn = new QPushButton(tr("关闭"),this);
    // 布局（把按钮布局到右上角）
    QHBoxLayout *hbLayout = new QHBoxLayout();
    hbLayout->addStretch();
    hbLayout->addWidget(m_minBtn);
    hbLayout->addWidget(m_normalBtn);
    hbLayout->addWidget(m_closeBtn);
    QVBoxLayout *vbLayout = new QVBoxLayout(this);
    vbLayout->addLayout(hbLayout);
    vbLayout->addStretch();
    connect(m_minBtn,&QPushButton::clicked,this,&Widget::showMinimized);
    connect(m_normalBtn,&QPushButton::clicked,this,&Widget::maxRestoreSlot);
    connect(m_closeBtn,&QPushButton::clicked,this,&Widget::close);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    // this->isMaximized() ? this->showNormal() : this->showMaximized();
    if(rect->contains(this->mapFromGlobal(QCursor::pos())) == true) // 指定区域内
        maxRestoreSlot();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(rect->contains(this->mapFromGlobal(QCursor::pos())) == true){ // 指定区域内
        if(event->button() == Qt::LeftButton){
            m_windowPos = this->pos();
            m_mousePos = event->globalPos();
            this->m_mousePressed = true;
        }
    }

    ratio = 0.0;
    double x = m_mousePos.x();
    double y = clientRect.width();
    ratio = x / y;
    lengthToMouse = 800 * ratio; // 利用鼠标在屏幕的比例来设置鼠标在可用窗体的位置
}

void Widget::mouseMoveEvent(QMouseEvent *event)

{
    //bool bMaximize = this->isMaximized(); // isMaximized() 不一定返回我们期望的值
    int x = m_mousePos.x() - lengthToMouse;
    int y = m_mousePos.y() - 2/* - m_mousePos.y()*/;
    QPoint point(x,y);
    if (m_mousePressed) {
        if (isMaxWindow) {
            //this->showNormal(); // 拖动最大化的窗口是否需要变成正常？
            resize(800, 600);
            move(x, y);
            isMaxWindow = false;
            m_windowPos=point;
        } else {
            this->move(m_windowPos + (event->globalPos() - m_mousePos));
        }

        if(event->globalY() < 4){ // 鼠标拖动窗体到屏幕顶端最大化窗体
            move(0, 0);
            resize(clientRect.width(), clientRect.height());
            isMaxWindow = true;
        }

        if(event->globalX() < 4){// 鼠标拖动窗体到屏幕最左端窗体占左半边屏幕大小
            resize(clientRect.width() / 2, clientRect.height());
            move(0, 0);
            isMaxWindow = false;
        }

        if(event->globalX() > clientRect.width() - 4){// 鼠标拖动窗体到屏幕最右端窗体占右半边屏幕大小
            resize(clientRect.width() / 2, clientRect.height());
            move(clientRect.width() / 2, 0);
            isMaxWindow = false;
        }
    }

    updateNormalBtn();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_mousePressed = false;
}

void Widget::maxRestoreSlot()
{
    //this->isMaximized() ? showNormal() : showMaximized();
    if(isMaxWindow){
        resize(800, 600);
        move((clientRect.width() - this->width())/2, (clientRect.height() - this->height())/2); // 居中
        isMaxWindow = false;
    }else{
        //showMaximized();
        resize(clientRect.width(), clientRect.height());
        move(0, 0);
        isMaxWindow = true;
    }
    updateNormalBtn();
}

void Widget::updateNormalBtn()
{
    isMaxWindow ? m_normalBtn->setText(tr("还原")) : m_normalBtn->setText(tr("最大化"));
}

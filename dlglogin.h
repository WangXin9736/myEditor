#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>

namespace Ui {
class dlgLogin;
}

class dlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit dlgLogin(QWidget *parent = nullptr);
    ~dlgLogin();

protected:
    //鼠标事件处理函数，拖动窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_btnLogin_clicked();

    void on_btnExit_clicked();

private:
    Ui::dlgLogin *ui;
    //自定义变量与函数
    bool m_moving = false;      //表示窗口在鼠标操作下移动
    QPoint m_lastPos;           //上一次光标移动的位置
    QString m_usr = "asuka";    //初始化用户名
    QString m_psd = "12345";    //初始化密码
    int m_tryCount = 0;         //试错次数
    void readSettings();        //读取设置
    void writeSettings();       //写入设置
    QString encrypt(const QString& str);    //字符串加密
};

#endif // DLGLOGIN_H

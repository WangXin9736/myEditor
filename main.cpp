#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dlgLogin *dlg = new dlgLogin;           //创建Splash登陆对话框
    QFile qssFile(":/style/style/style.qss");
    if(qssFile.open(QFile::ReadOnly))
    {
        dlg->setStyleSheet(qssFile.readAll());
    }
    else
    {
        QMessageBox::critical(0, "错误", "样式表加载失败", QMessageBox::Ok);
    }
    qssFile.close();

    if(dlg->exec() == QDialog::Accepted)
    {
        MainWindow w;
        w.show();               //显示主窗口
        return a.exec();        //应用程序正常运行
    }
    else
        return 0;               //退出应用程序
}

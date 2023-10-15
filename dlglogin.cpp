#include "dlglogin.h"
#include "ui_dlglogin.h"
#include <QMouseEvent>
#include <QApplication>
#include <QByteArray>
#include <QCryptographicHash>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

dlgLogin::dlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgLogin)
{
    ui->setupUi(this);
    ui->lineEdit_Psd->setEchoMode(QLineEdit::Password);     //设置为密码输入模式
    this->setAttribute(Qt::WA_DeleteOnClose);               //对话框关闭时自动删除，close()是隐藏窗口，widget接受关闭事件后才会释放窗口
    this->setWindowFlags(Qt::SplashScreen);                 //窗口无边框，不在任务栏显示
    this->setWindowFlags(Qt::FramelessWindowHint);          //无边框化
    setAttribute(Qt::WA_TranslucentBackground);             //背景透明
    QApplication::setOrganizationName("myEditor");          //组织名
    QApplication::setApplicationName("Login");              //应用程序名
    readSettings();     //读取用户名和密码
}

dlgLogin::~dlgLogin()
{
    delete ui;
}


//-------------------------------------------------------
//鼠标按下
//-------------------------------------------------------
void dlgLogin::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_moving = true;
        m_lastPos = event->globalPosition().toPoint() - this->pos();    //获取鼠标在窗口中的相对位置
    }
    return QDialog::mousePressEvent(event);
}


//-------------------------------------------------------
//鼠标移动
//-------------------------------------------------------
void dlgLogin::mouseMoveEvent(QMouseEvent *event)
{
    QPoint eventPos = event->globalPosition().toPoint();
    if(m_moving && (event->buttons() & Qt::LeftButton)
        && (eventPos - m_lastPos).manhattanLength() > QApplication::startDragDistance())
    {
        move(eventPos - m_lastPos);
        m_lastPos = eventPos - this->pos();
    }
}


//-------------------------------------------------------
//鼠标释放
//-------------------------------------------------------
void dlgLogin::mouseReleaseEvent(QMouseEvent *event)
{
    m_moving = false;
    event->accept();    //停止移动
}


//-------------------------------------------------------
//字符串加密
//QCryptographicHash创建时需要指定一种加密算法
//QCryptographicHash只提供了加密功能，没有提供解密功能。
//-------------------------------------------------------
QString dlgLogin::encrypt(const QString& str)
{
    QByteArray btArray = str.toLocal8Bit();                 //字符串转换为字节数组数据
    QCryptographicHash hash(QCryptographicHash::Md5);       //MD5加密算法
    hash.addData(btArray);                                  //添加数据
    QByteArray resultArray = hash.result();                 //返回最终散列值
    QString md5 = resultArray.toHex();                      //转换为16进制字符串
    return md5;
}


//-------------------------------------------------------
//读取应用程序设置
//-------------------------------------------------------
void dlgLogin::readSettings()
{
    QSettings settings;
    bool saved = settings.value("saved", false).toBool();   //读取saved值
    m_usr = settings.value("Usrname", "asuka").toString();  //读取Usrname值
    QString defalutPsd = encrypt(m_psd);                    //密码加密
    m_psd = settings.value("PSWD", defalutPsd).toString();     //读取PSWD值
    if(saved)
    {
        ui->lineEdit_Usr->setText(m_usr);
    }
    ui->checkBox_rmPsd->setChecked(saved);
}


//-------------------------------------------------------
//写入设置
//-------------------------------------------------------
void dlgLogin::writeSettings()
{
    QSettings settings;
    settings.setValue("Usrname", m_usr);        //用户名
    settings.setValue("PSWD", m_psd);           //密码
    settings.setValue("saved", ui->checkBox_rmPsd->isChecked());
}



void dlgLogin::on_btnLogin_clicked()
{
    QString usr = ui->lineEdit_Usr->text().trimmed();       //输入用户名
    QString psd = ui->lineEdit_Psd->text().trimmed();       //输入密码
    QString encryptPsd = encrypt(psd);      //密码加密
    if((usr == m_usr) && (encryptPsd == m_psd))
    {
        writeSettings();    //保存设置
        qDebug()<<"密码正确";
        this->accept();     //调用accept(),关闭对话框
    }
    else
    {
        m_tryCount++;
        if(m_tryCount > 3){
            QMessageBox::critical(this, "错误", "输入错误次数太多，强制退出");
            this->reject();
        }
        else
            QMessageBox::warning(this, "警告", "用户名或密码错误");
    }
}


void dlgLogin::on_btnExit_clicked()
{
    this->reject();
}


#pragma execution_character_set("utf-8")
#include"QtGradeManage.h"
#include <QMessageBox>

QtGradeManage::QtGradeManage(QWidget *parent)
    : QMainWindow(parent),change_flag(0),id_complete(0),PW_complete(0)
{
    ui.setupUi(this);
}

//myslot
void QtGradeManage::idEdited() {
    id_complete = true;
}
void QtGradeManage::pswEdited() {
    PW_complete = true;
}
void QtGradeManage::exchanged() {
    switch (change_flag)
    {
    case 0:this->ui.Exchange->setText("管理员"); change_flag = 1; break;
    default:
        this->ui.Exchange->setText("学生"); change_flag = 0; break;
    }
}
void QtGradeManage::logined() {
    if (!(PW_complete && id_complete))
    {
        ui.userId->setFocus();
        QMessageBox::critical(NULL, "登录失败", "账号或密码为空");
        return;
    }
    if (change_flag) {
        if (ui.userId->text() == root.getuserid() && root.checkPW(ui.passward->text())) {
            admin_ui* Ad_ui = new admin_ui(this);
            Ad_ui->root = &root;
            Ad_ui->gradelist = &gradelist;
            Ad_ui->intcontent();
            this->close();
            Ad_ui->show();
        }
        else {
            QMessageBox::critical(NULL, "登录失败", "账号或密码错误");
        }
    }
    else
    {
        int index = gradelist.idseach(ui.userId->text());
        if (index == -1) {
            QMessageBox::critical(this, "登录失败", "未找到相关账户");
            return;
        }
        if (ui.passward->text() == gradelist.grade.at(index).stu.content.at((int)student::Password)){
            student_ui* std_ui = new student_ui(this,&gradelist.grade[index],&gradelist.course,this);
            std_ui->setAttribute(Qt::WA_DeleteOnClose);
            std_ui->intcontent();
            this->close();
            std_ui->show();
        }
        else {
            QMessageBox::critical(NULL, "登录失败", "密码错误");
        }
    }
    
}

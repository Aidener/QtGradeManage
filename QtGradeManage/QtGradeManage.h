#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include "ui_QtGradeManage.h"
#include"otherUI.h"

class QtGradeManage : public QMainWindow
{
    Q_OBJECT
public:
    QtGradeManage(QWidget *parent = Q_NULLPTR);
private:
    Ui::QtGradeManageClass ui;
    bool change_flag;
    bool id_complete;
    bool PW_complete;
    admin root;
    GradeList gradelist;
 //myslot
private slots:
    void idEdited();
    void pswEdited();
    void exchanged();
    void logined();
};

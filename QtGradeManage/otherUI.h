#pragma execution_character_set("utf-8")
#ifndef OTHER_UI
#define OTHER_UI
#include"ui_student.h"
#include"ui_admin.h"
#include"ui_tipDialog.h"
#include <QtWidgets/QMainWindow>
#include<QtWidgets/qdialog.h>
#include"data.h"
#include"ui_oneInputDialog.h"
#include"ui_QcourseManage.h"
#include"ui_searchDialog.h"

class QtGradeManage;

class student_ui : public QMainWindow
{
    Q_OBJECT
public:
    student_ui(QtGradeManage* w, Grade* g, Course* c,QWidget* parent = Q_NULLPTR);
    void intcontent();
private:
    Ui::MainWindow ui;
    QtGradeManage* w;
    Grade* g;
    Course *c;
private slots:
    void stexit();
    void seetips();
    void pwsModifyed();
    void dateChanged();
};



class admin_ui :public QMainWindow {
    Q_OBJECT
public:
    admin_ui(QtGradeManage* w, QWidget* parent = Q_NULLPTR);
    void intcontent();
    admin* root;
    GradeList* gradelist;
private slots:
    void BserachChange(int);
    void Bquit();
    void Bsave();
    void Bupdate();
    void BAddGrade();
    void BsetTip();
    void BResetStudentPW();
    void BmodifyPW();
    void cellchanged(int, int);
    void expelGrade();
    void Bsearch();
    void BcourseManage();
private:
    Ui::admin ui;
    QtGradeManage* w;
    int updateRow;
};


class tipDialog_ui:public QDialog
{
     Q_OBJECT
public:
    tipDialog_ui(QWidget* parent = Q_NULLPTR);
    Ui::tipDialog ui;
private:
    void writeTip();
private slots:
    void Accept();
};


class oneInputDialog:public QDialog
{
    Q_OBJECT
public:
    oneInputDialog(QWidget* parent);
    Ui_oneInputDialog ui;
private slots:
};


class  QcourseManage:public QDialog
{
    Q_OBJECT
public:
    QcourseManage(QWidget* parent, GradeList* gradelist);
    void intcontent();
private:
    GradeList *gradelist;
    Ui::QCourseManage ui;
private slots:
    void cellchange(int, int);
    void remove();
    void add();
};

class DialogForSearch :public QDialog{
    Q_OBJECT
public:
    DialogForSearch(QWidget* parent, QTableWidget* tw,GradeList *g);
private:
    Ui::UIsearchDialog ui;
    QTableWidget* tw;
    GradeList* g;
    int way;
    bool finish;
private slots:
    void endedit();
    void classChange(int);
    void majorChange(int);
    void waychange(int);
    void Accept();
};
#endif
#pragma execution_character_set("utf-8")
#include"otherUI.h"
#include "QtGradeManage.h"
#include<qtextstream.h>
#include<qstringlist.h>
#include<qfile.h>
#include<iostream>
#include<Qdebug>
#include<qscrollbar.h>
#include<QMessageBox>
#include<algorithm>
student_ui::student_ui(QtGradeManage* w, Grade* g, Course* c,QWidget* parent)
    : QMainWindow(parent),w(w),g(g),c(c)
{
    ui.setupUi(this);
}

void student_ui::stexit() {                  
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->close();
    w->show();
}

void student_ui::intcontent()                  //初始化窗口数据
{
    ui.Qname->setText(g->stu.content.at((int)student::Name));
    ui.major->setText(g->stu.content.at((int)student::Major));
    ui.classes->setText(g->stu.content.at((int)student::GradeClass));
    ui.studentId->setText(g->stu.content.at((int)student::ID));

    int rowcount = c->content.length();
    ui.gradeTable->setRowCount(rowcount);
    ui.gradeTable->setColumnCount(2);
    for (int i = 0; i < rowcount; i++) {
        QTableWidgetItem* item = new QTableWidgetItem(c->content.at(i));
        ui.gradeTable->setItem(i, 0, item);
    }
    for (int i = 0; i < rowcount; i++) {
        QTableWidgetItem* iteam = new QTableWidgetItem(g->content.at(i));
        ui.gradeTable->setItem(i,1,iteam);
    }
}

void student_ui::seetips()
{
    QFile tip("tip.txt");
    tip.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&tip);
    QString str=in.readAll();
    tipDialog_ui* tip_ui = new tipDialog_ui(this);
    tip_ui->setAttribute(Qt::WA_DeleteOnClose, true);
    tip_ui->ui.okButton->hide();
    tip_ui->ui.textEdit->setReadOnly(true);
    tip_ui->ui.textEdit->setText(str);
    tip_ui->show();
}

void student_ui::pwsModifyed()
{
    oneInputDialog* md = new oneInputDialog(this);
    md->ui.Label->setText("请输入密码");
    if (md->exec() == QDialog::Accepted) {
        g->stu.content.replace(5,md->ui.LineEdit->text());
    }
    delete md;
}

void student_ui::dateChanged()
{

}


//admin_ui

admin_ui::admin_ui(QtGradeManage* w, QWidget* parent):QMainWindow(parent),w(w),root(NULL),gradelist(NULL){
    ui.setupUi(this);
    ui.gradeListTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //设为整行选择
}

void admin_ui::intcontent()
{
    ui.userName->setText(root->getname());
    ui.userId->setText(root->getuserid());
    ui.gradeListTable->blockSignals(true);
    QStringList heard;
    int gradecount = gradelist->course.content.length();
    heard.append("学号");
    heard.append("姓名");
    heard.append("性别");
    heard.append("专业");
    heard.append("班级");
    if (!gradecount) {
        gradelist->course.content.append("计算机组成原理");
        gradecount++;
    }
    heard += gradelist->course.content;
    ui.gradeListTable->setColumnCount(5 + gradelist->course.content.length());
    ui.gradeListTable->setHorizontalHeaderLabels(heard);

    int Rowcount = gradelist->grade.length();    //设置行内容
    if (Rowcount == 0) {                        //初始化样例
        ui.gradeListTable->setRowCount(1);
        Grade tem;
        tem.stu.content.append("2000150001");
        tem.stu.content.append("张三");
        tem.stu.content.append("男");
        tem.stu.content.append("计算机");
        tem.stu.content.append("2001");
        tem.stu.content.append("2000150001");
        tem.content.append("90");
        gradelist->grade.append(tem);
        Rowcount++;
    }
    else
        ui.gradeListTable->setRowCount(Rowcount);

    QTableWidgetItem* p = NULL;
    for (size_t i = 0; i < Rowcount; i++)         //设置表格内容
    {
        size_t j = 0;
        for (;j < 5; j++)
        {
            p = new QTableWidgetItem(gradelist->grade.at(i).stu.content.at(j));
            ui.gradeListTable->setItem(i, j,p);
        }
        for (j=0; j < gradecount; j++)
        {
            p = new QTableWidgetItem(gradelist->grade.at(i).content.at(j));
            ui.gradeListTable->setItem(i, j+5, p);
        }
    }
    updateRow = gradelist->grade.length();
    ui.gradeListTable->blockSignals(false);
}

void admin_ui::BserachChange(int p) {

}

void admin_ui::Bquit()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);        //设置窗口为可销毁
    this->close();                                         //关闭并销毁窗口
    w->show();                                             //打开登录界面
}

void admin_ui::Bsave()
{
   
}

void admin_ui::Bupdate()
{
    if (gradelist->course.content.size()!=ui.gradeListTable->columnCount()-5)   //重置列
    {
        ui.gradeListTable->blockSignals(true);
        ui.gradeListTable->clear();
        ui.gradeListTable->blockSignals(false);
        intcontent();
        return;
    }
    int len = gradelist->grade.length();
    int Rowcount;
    if (updateRow > len)            //多列时，如果有部分列不规范，会记录并插入规范数据，后续无需再处理
        Rowcount = updateRow;
    else {
        Rowcount = ui.gradeListTable->rowCount();
    }
    int columncout = ui.gradeListTable->columnCount();
    
    QString str,str2;
    if (Rowcount > len) {                             //添加数据入表
        for (; Rowcount - len; Rowcount--) {
            Grade tem;
            if (ui.gradeListTable->item(Rowcount - 1, 0) == 0) {      //检测id有效性
                str2 = "第" + QString::number(Rowcount) + "行学号为空";
                QMessageBox::critical(this, "添加失败", str2);
                return;
            }
            else {
                str = ui.gradeListTable->item(Rowcount - 1, 0)->text();
                if (gradelist->idseach(str) != -1)
                {
                    str2 = "第" + QString::number(Rowcount) + "行学号重复第"+ QString::number(gradelist->idseach(str) + 1)+"行";
                    QMessageBox::critical(this, "添加失败", str2);
                    return;
                }
                else
                {
                    str = ui.gradeListTable->item(Rowcount - 1, 0)->text();
                    tem.stu.content.append(str);
                }
            }

            //**************************************************
            for (int i = 1; i < 5; i++) {                //添加信息
                if (ui.gradeListTable->item(Rowcount - 1, i) == 0)
                {
                    str2 = "第" + QString::number(Rowcount) + "行第"+ QString::number(i + 1)+"列存在非法数据";
                    QMessageBox::critical(this, "添加失败", str2);
                    return;
                }
                else {
                    str = ui.gradeListTable->item(Rowcount - 1, i)->text();
                    tem.stu.content.append(str);
                }
                    
            }
            for (int i = 5; i < columncout; i++) {               //添加成绩
                if (ui.gradeListTable->item(Rowcount-1, i)==0)
                {
                    str2 = "第" + QString::number(Rowcount) + "行第" + QString::number(i + 1) + "列存在非法数据";
                    QMessageBox::critical(this, "添加失败", str);
                    return;
                }
                else {
                    str = ui.gradeListTable->item(Rowcount - 1, i)->text();
                    tem.content.append(str);
                }
                    
            }
            updateRow = Rowcount;
            gradelist->grade.append(tem);
        }
    }
    gradelist->idsort();              //表排序
    intcontent();                     //重置展示
}

void admin_ui::BAddGrade()
{
    ui.gradeListTable->setRowCount(ui.gradeListTable->rowCount() + 1);
}

void admin_ui::BsetTip()
{
    tipDialog_ui* tip_ui = new tipDialog_ui(this);
    tip_ui->setAttribute(Qt::WA_DeleteOnClose);
    QFile tip("tip.txt");
    tip.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&tip);
    QString str = in.readAll();
    tip_ui->ui.textEdit->setText(str);
    tip_ui->show();
}

void admin_ui::BResetStudentPW()
{
    oneInputDialog* Br = new oneInputDialog(this);
    Br->ui.Label->setText("请输入学号");
    if (Br->exec()==QDialog::Accepted) {
        int index = gradelist->idseach(Br->ui.LineEdit->text());
        if (index == -1) {
            QMessageBox::critical(this, "查找失败", "未找到相关信息");
        }
        else
        {
            if (gradelist->grade[index].stu.content.length() > 5)
                gradelist->grade[index].stu.content.removeLast();
            QMessageBox::information(this, "成功", "重置成功");
        }
    }
    delete Br;
}

void admin_ui::BmodifyPW()
{
    oneInputDialog* md = new oneInputDialog(this);
    md->ui.Label->setText("请输入密码");
    if (md->exec() == QDialog::Accepted) {
        root->modifyPW(md->ui.LineEdit->text());
    }
    delete md;
}

void admin_ui::cellchanged(int i, int j)
{
    if (i>=gradelist->grade.size())
    {
        return;
    }
    if (j < gradelist->grade.at(i).stu.content.length())
    {
        gradelist->grade[i].stu.content.replace(j, ui.gradeListTable->item(i, j)->text());
    }
    else
    {
        if (!j - 5 < gradelist->course.content.length())
        {
            for (int k = gradelist->course.content.length(); k <= j - 5; j++)
            gradelist->grade[i].content.append("none");

        }
        gradelist->grade[i].content.replace(j - 5, ui.gradeListTable->item(i, j)->text());
    }
}

void admin_ui::expelGrade()
{
    QList<QTableWidgetSelectionRange> range = ui.gradeListTable->selectedRanges();
    QVector<int> row;
    for (int i = 0; i < range.count(); i++) {
        for (int j = range.at(i).topRow(); j <=range.at(i).bottomRow(); j++)
            row.push_back(j);
    }
    std::sort(row.begin(), row.end(), [](int a, int b) {return a > b; });     //排序行，倒叙，避免删除大行越界
    for (int i = 0; i < row.size(); i++) {
        gradelist->grade.remove(row.at(i));
        ui.gradeListTable->removeRow(row.at(i));
    }
}

void admin_ui::Bsearch()
{
    DialogForSearch* d = new DialogForSearch(this, ui.gradeListTable, gradelist);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
}

void admin_ui::BcourseManage()
{
    QcourseManage* c = new QcourseManage(this, gradelist);
    setAttribute(Qt::WA_DeleteOnClose);
    c->exec();
}


//tipDialog_ui

tipDialog_ui::tipDialog_ui(QWidget* parent):QDialog(parent){
    ui.setupUi(this);
}

void tipDialog_ui::writeTip()
{
    QFile tip("tip.txt");
    QString str;
    tip.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&tip);
    str = ui.textEdit->toPlainText();
    QByteArray tem = str.toUtf8();
    out << tem;
    tip.close();
}

void tipDialog_ui::Accept() {
    writeTip();
    this->close();
}


oneInputDialog::oneInputDialog(QWidget* parent) :QDialog(parent){
    ui.setupUi(this);
}



QcourseManage::QcourseManage(QWidget* parent, GradeList* gradelist):QDialog(parent), gradelist(gradelist) {
    ui.setupUi(this);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);   //只允许个选
    intcontent();
}

void QcourseManage::intcontent()
{
    int column = gradelist->course.content.length();
    ui.tableWidget->blockSignals(true);
    ui.tableWidget->setRowCount(1);
    if (column)
        ui.tableWidget->setColumnCount(column);
    else {
        ui.tableWidget->setColumnCount(1);
    } 
    QTableWidgetItem* p;
    for (int i = 0; i < column; i++)
    {
        p = new QTableWidgetItem(gradelist->course.content.at(i));
        ui.tableWidget->setItem(0,i,p);
     }
    ui.tableWidget->blockSignals(false);
}

void QcourseManage::cellchange(int i, int j) {
    if (j < gradelist->course.content.length())
        gradelist->course.content.replace(j, ui.tableWidget->item(i, j)->text());
    else
    {
        QString str= ui.tableWidget->item(i, j)->text();
        string str1 = str.toStdString();
        gradelist->course.content.append(str);
        for (int i = 0; i < gradelist->grade.size(); i++)
            gradelist->grade[i].content.append("0");
    }
    ui.tableWidget->blockSignals(true);
    ui.tableWidget->clear();
    ui.tableWidget->blockSignals(false);
    intcontent();
}

void QcourseManage::add(){
    ui.tableWidget->setColumnCount(ui.tableWidget->columnCount() + 1);
}

void QcourseManage::remove() {
   QList<QTableWidgetItem*> list= ui.tableWidget->selectedItems();
   QVector<int> columns;
   for (int i = 0; i < list.size(); i++) {
       columns.push_back(list.at(i)->column());
   }
   std::sort(columns.begin(), columns.end(), [](int a, int b) {return a > b; });
   for ( int i = 0; i < columns.size(); i++)
   {
       ui.tableWidget->removeColumn(columns.at(i));
       gradelist->course.content.removeAt(columns.at(i));
       for (int i = 0; i < gradelist->grade.size(); i++)
           gradelist->grade[i].content.removeAt(columns.at(i));
   }
   
}



DialogForSearch::DialogForSearch(QWidget* parent, QTableWidget* tw,GradeList* g) :QDialog(parent),tw(tw),g(g),finish(0),way(0){
    ui.setupUi(this);
    waychange(0);
}

void DialogForSearch::endedit() {
    finish = true;
}

void DialogForSearch::classChange(int i)
{

}

void DialogForSearch::majorChange(int i)
{

}

void DialogForSearch::waychange(int i)
{
    way = i;
    switch (way)
    {
    case 0:
    case 1:
        ui.useClass->hide();
        ui.useMajor->hide();
        break;
    default:
        ui.useClass->show();
        ui.useMajor->show();
        break;
    }
}

void DialogForSearch::Accept()
{
    if (finish) {
        int index;
        switch (way)
        {
        case 0:index = g->idseach(ui.idName->text());
            if (index == -1) {
                QMessageBox::critical(NULL, "查找失败", "未找到相关信息");
                return;
            }
            tw->verticalScrollBar()->setSliderPosition(index);
            tw->selectRow(index); 
            break;
        case 1:index = g->nameserach(ui.idName->text());
            if (index == -1) {
                QMessageBox::critical(NULL, "查找失败", "未找到相关信息");
                return;
            }
            tw->verticalScrollBar()->setSliderPosition(index); 
            tw->selectRow(index);
            break;
        default:
            break;
        }
    }
    else
    {
        QMessageBox::critical(NULL, "查找失败", "请输入学号或名字");
        return;
    }
    this->close();     
}



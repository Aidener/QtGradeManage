#pragma execution_character_set("utf-8")
#include "QtGradeManage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtGradeManage w;
    w.show();
    return a.exec();
}

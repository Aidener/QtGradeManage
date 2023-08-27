#pragma execution_character_set("utf-8")
#ifndef MY_DATA
#define MY_DATA
#include <QStringList>
#include<Qfile>
#include<Qtextstream>
#include<Qvector>
using namespace std;
class admin {
	QString name;
	QString userId;
	QString password;
public:
	admin();
	~admin();
	QString getname();
	QString getuserid();
	bool checkPW(QString t);
	bool modifyPW(QString p);
};


class Course {                                       //科目表类 
public:
	Course();                           //从文件加载表头 
	QStringList content;
	~Course();                         //写回文件 
};


enum class student
{
	ID,Name,sex,Major,GradeClass,Password
};

class Student {                          //学生类 
public:
	QStringList content;
	Student(); 
};
QTextStream& operator<<(QTextStream& out,const Student &s);


class Grade {                                 //成绩类  
public:
	Student stu;                         //学生 
	QStringList content;                        //成绩单 
};
QTextStream& operator <<(QTextStream& out,const Grade &g);

class GradeList {                               //成绩表类 
private:
	bool sortMethod;                       //排序方式，用于查找
public:
	GradeList();
	~GradeList();
	QVector<Grade> grade;                  //成绩表
	Course course;
	void idsort();
	void keysort();                           //基排序 
	long long int stringTolong(string& str);
	int idseach(QString id);                           //查询
	int nameserach(QString name);                      //
	int keysearch(QString major, QString gradeclass, QString name);
};

#endif // !MY_DATA


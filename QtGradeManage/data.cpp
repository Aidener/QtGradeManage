#pragma execution_character_set("utf-8")
#include"data.h"
#include<Qfile>
#include<QTextStream>

admin::admin() {
	QFile file("user.bin");

	if (!file.open(QIODevice::ReadOnly)) {
		name = "管理员";
		userId = "400857";
		password = "400857";
		return;
	}
	else
	{
		QTextStream in(&file);
		in >> name >> userId >> password;
	}
}

admin::~admin() {
	QFile file("user.bin");
	file.open(QIODevice::WriteOnly | QIODevice::Truncate|QIODevice::Text);
	QTextStream out(&file);
	out << name + " " << userId + " " << password << endl;
}

QString admin::getname() {
	return name;
}

QString admin::getuserid() {
	return userId;
}

bool admin::checkPW(QString t) {
	return t == password;
}

bool admin::modifyPW(QString p)
{
	password = p;
	return true;
}



Student::Student()
{
	content.reserve(6);
}

QTextStream& operator<<(QTextStream& out,const Student& s)
{
	for (int i = 0; i < 5; i++) {
		out << s.content.at(i) << ' ';
	}
	if (s.content.length()==5)        //初始化密码为学号
		out << s.content.at(0);
	else
		out << s.content.at(s.content.length() - 1);
	out << '\t';
	return out;
}


QTextStream& operator<<(QTextStream& out,const Grade& g)
{
	out << g.stu;
	for (int i = 0; i < g.content.length(); i++) {
		out << g.content.at(i) << ' ';
	}
	out << '\n';
	return out;
}


Course::Course()
{
	QFile file("course.bin");
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		QString str = in.readAll();
		if (!str.isEmpty())
			content = str.split(" ");
	}
}

Course::~Course()
{
	QFile file("course.bin");
	file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
	QTextStream out(&file);
	for ( int i = 0;  i <content.length()-1; i++)
	{
		out << content.at(i) << ' ';
	}
	out << content.at(content.length() - 1);
}

GradeList::GradeList():sortMethod(0)
{
	QFile file("gradelist.bin");
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		QString str = in.readAll();
		QStringList str3;
		Grade tem;
		QStringList str2 = str.split(QRegExp("[\r\n]"),QString::SkipEmptyParts); //以换行分割
		for (int i = 0; i < str2.length(); i++) {
			str3 = str2.at(i).split('\t');                          //以\t分割
			switch (str3.length())
			{
			case 2:
				tem.stu.content = str3.at(0).split(' ');
			default:
				tem.content = str3.at(1).split(' ');
				break;
			}
			
			grade.append(tem);
		}
	}
}

GradeList::~GradeList()
{
	QFile file("gradelist.bin");
	file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
	QTextStream out(&file);
	for (int i = 0; i < grade.length(); i++) {
		out << grade.at(i);
	}
}


void GradeList::idsort()
{
	int left;
    int right;
    int i;
	left = 0;
	int r,l;                   
	right = grade.size()-1;
	string x1, x2;                   //关键字
	while (right> left)             //两端未到中间时
	{
		l = left + 1;
		r = right - 1;                  //避免r在数组有序时出现未赋值而直接赋值给right出错
		for (i = left; i < right; i++) // 正向冒泡,将最大值移动至最右端
		{
			x1 = grade.at(i).stu.content.at((int)student::ID).toStdString();
			x2 = grade.at(i+1).stu.content.at((int)student::ID).toStdString();
			if (stringTolong(x1) >stringTolong(x2)) 
			{
				Grade tem = grade.at(i);
				grade.replace(i, grade.at(i + 1));
				grade.replace(i + 1, tem);
				r = i;
			}
		}
		right = r;  //记录最后发生交换位置，显然，交换位置的前后的数据是有规律的，形如分块，前面均比此位置数据小，后面均比其大且有序

		for (i = right; i > left; i--) // 反向冒泡，从右端未被排序的元素开始，将最小值移至最左端
		{
			x1 = grade.at(i).stu.content.at((int)student::ID).toStdString();
			x2 = grade.at(i -1).stu.content.at((int)student::ID).toStdString();
			if (stringTolong(x1) < stringTolong(x2))   
			{
				Grade tem = grade.at(i);
				grade.replace(i, grade.at(i - 1));
				grade.replace(i-1, tem);
				l = i;
			}
		}
		left = l;  //记录最后发生交换位置，显然，交换位置的前后的数据是有规律的，形如分块，前面均比此位置数据小且有序，后面均比其大
		
	}
}


void GradeList::keysort()
{
    
}

long long int GradeList::stringTolong(string &str) {
	long long int sum = 0;
	for (int i = str.length()-1,j=0; i >=0; i--,j++) {
		sum += pow(10, j) * (str[i]-'0');
	}
	return sum;
}

int GradeList::idseach(QString id)           //折半查找
{
	int len = grade.length();
	int begin = (len-1)/ 2;
	int end = len-1;
	int index = -1;
	while (begin!=end)
	{
		if (id == grade.at(begin).stu.content.at((int)student::ID))
		{
			index = begin;
			return index;
		}
		string i = id.toStdString(); 
		string j = grade.at(begin).stu.content.at((int)student::ID).toStdString();
		if (stringTolong(i) < stringTolong(j))
		{
			end = begin;
			begin /= 2;
		}
		else {
			if (end - begin == 1)            //避免死循环
			{
				begin++;
				break;
			}
			begin = (begin+end)/2;
		}
	}
	if (id == grade.at(begin).stu.content.at((int)student::ID))
	{
		index = begin;
	}
	if (index != -1)
		return index;
	return -1;
}

int GradeList::nameserach(QString name)           //段式查找（希尔查找）
{
	int len = grade.length();
	int pice = len / 4;
	int rem = len % 4;
	int k=0;
	if (pice == 0)
		k = rem;
	if(rem)
		k += 1;
	int index, j;
	for (int i = 0; i < k; i++) {
		j = 0;
		index = 0;
		while (index < len)
		{
			if (name == grade.at(i).stu.content.at((int)student::Name)) {
				index = i;
				break;
			}
			j++;
			index = k * j + i;
		}
		if (index < len)
			return index;
	}
	return -1;
}

int GradeList::keysearch(QString major,QString gradeclass,QString name)
{

	return 0;
}


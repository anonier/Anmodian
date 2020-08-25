#pragma once
#include "CppSQLite3U.h"

//π ’œ◊÷∂Œ–≈œ¢
typedef struct _student
{
	CString name;
	CString no;
}student;

class DBOperation
{
public:
	student m_mystudent;
	CList<student,student&> m_studentlist;
	CppSQLite3DB db;
	DBOperation(void);
	~DBOperation(void);
	void addstudent(CString m_myname, CString m_myno);
	void editstudent(CString m_myname, CString m_myno);
	void dellstudent(CString m_myname);
	void dellALLstudent();
	void selstudent(CString m_myname);
	void selallstudent();
};

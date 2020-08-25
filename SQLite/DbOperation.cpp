#include "StdAfx.h"
#include "DBOperation.h"

DBOperation::DBOperation(void)
{
	CString strDbPath;
	strDbPath = _T("C:\\dboperation.db");
	//打开或新建一个数据库
	db.open(strDbPath);	
}

DBOperation::~DBOperation(void)
{
}

void DBOperation::addstudent(CString m_myname, CString m_myno)
{
	CString StrSql;
	StrSql.Format(_T("insert into student(name,no) values('%s','%s')"),m_myname,m_myno);
	db.execDML(StrSql);
}
void DBOperation::editstudent(CString m_myname, CString m_myno)
{
	CString StrSql;
		StrSql.Format(_T("update student set no='%s'where name='%s'"),m_myno,m_myname);
	db.execDML(StrSql);
}
void DBOperation::dellstudent(CString m_myname)
{
	CString StrSql;
	StrSql.Format(_T("delete from student where name='%s'"),m_myname);
	db.execDML(StrSql);

}

void DBOperation::dellALLstudent()
{
	CString StrSql;
	StrSql.Format(_T("delete from student"));
	db.execDML(StrSql);

}
void DBOperation::selstudent(CString m_myname)
{
	CString StrSql;
	m_studentlist.RemoveAll();
	m_studentlist.GetHeadPosition();
	StrSql.Format(_T("select * from student where name='%s'"),m_myname);
	bool bRet = FALSE;
	CppSQLite3Query q = db.execQuery(StrSql);
	CString strResult;
	while(!q.eof())
    {
		m_mystudent.name=q.getStringField(_T("name"));
		m_mystudent.no=q.getStringField(_T("no"));
		m_studentlist.AddTail(m_mystudent);
		bRet = TRUE;
		q.nextRow();
	}
	q.finalize();
}
void DBOperation::selallstudent()
{
	CString StrSql;
	m_studentlist.RemoveAll();
	m_studentlist.GetHeadPosition();
	StrSql.Format(_T("select * from student"));
	bool bRet = FALSE;
	CppSQLite3Query q = db.execQuery(StrSql);
	CString strResult;
	while(!q.eof())
    {
		m_mystudent.name=q.getStringField(_T("name"));
		m_mystudent.no=q.getStringField(_T("no"));
		m_studentlist.AddTail(m_mystudent);
		bRet = TRUE;
		q.nextRow();
	}
	q.finalize();


	
}
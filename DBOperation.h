#pragma once
//mysql
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <statement.h>  
#include "CppSQLite3U.h"
using namespace sql;  
using namespace std; 

typedef struct student
{
	CString name;
	CString userid;
	CString money;
	CString operation;
}student;

typedef struct jishi
{
	CString name;
	CString userid;
	CString money;
	CString password;
}jishi;

typedef struct _manager
{
	CString name;
	CString password;
}manager;

typedef struct _server
{
	CString userid;
	CString type;
	CString money;
	CString time;
	CString serverid;
}server;

class DBOperation
{
public:
	manager m_mymanager;
	CList<manager,manager&> m_managerlist;

	jishi m_jishi;
	CList<jishi, jishi&> m_jishilist;

	server m_myserver;
	CList<server,server&> m_serverlist;

	student m_mystudent;
	CList<student, student&> m_studentlist;
	CppSQLite3DB db;

	sql::mysql::MySQL_Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::Savepoint *savept;

	
	DBOperation(void);
	~DBOperation(void);


	bool loadmanager(CString m_myname, CString m_mypassword);
	bool editmanager(CString m_myname, CString m_mypassword);



	bool loadjishi(CString m_myname, CString m_mypassword);
	bool editjishi(CString m_myname, CString m_mypassword);
	void selalljishi();
	void addjishi(CString m_myno, CString m_myname, CString m_vmon, CString m_vop);
	void dellALLjsihi();
	bool delljishi(CString m_vno);
	void seljishi(CString m_myname);
	bool updatejishi(CString m_vname, CString m_vmon, CString m_vop, CString m_vno);

	void addstudent(CString m_myno, CString m_myname, CString m_vmon, CString m_vop);
	bool updatestudent(CString m_vname, CString m_vmon, CString m_vop, CString m_vno);
	bool dellstudent(CString m_vno);
	void selstudent(CString m_myname);
	void selallstudent();
	void dellALLstudent();

};

#include "StdAfx.h"
#include "DBOperation.h"

DBOperation::DBOperation(void)
{
	driver = sql::mysql::get_mysql_driver_instance();
	//防止网络堵塞
	try
	{
		con = driver->connect("tcp://rdsc70m84m457732q4q5.mysql.rds.aliyuncs.com:3306", "rfid", "Rfid123456");

	}  
	catch(SQLException &e)
	{  
		
	}
	stmt = con->createStatement();
	stmt->execute("USE student");
	stmt->execute("set names 'gbk'");
	CString strDbPath;
	strDbPath = _T("dboperation.db");
	//打开或新建一个数据库
	db.open(strDbPath);
}

DBOperation::~DBOperation(void)
{
	//delete res;
	delete stmt;
	delete con;
}

bool DBOperation::loadmanager(CString m_myname, CString m_mypassword)
{
	
	CString StrSql;
	
	StrSql.Format(_T("select * from manager where name='%s'"),m_myname);
	//防止网络堵塞
	try{res = stmt->executeQuery(StrSql.GetBuffer(0));}  
	catch(SQLException &e){ return false ;}
	if(res->next()) 
	{
		if(m_mypassword==res->getString("password").c_str())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool DBOperation::editmanager(CString m_vname, CString m_mypassword)
{

	CString StrSql;
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_vname, "改密", NULL,m_strTime, "manager");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	StrSql.Format(_T("update manager set password='%s'where name='%s'"),m_mypassword,m_vname);
	//防止网络堵塞
	try{stmt->execute(StrSql.GetBuffer(0));}  
	catch(SQLException &e){ return false; }
	return true;
}


bool DBOperation::loadjishi(CString m_myname, CString m_mypassword)
{

	CString StrSql;

	StrSql.Format(_T("select * from jishi where userid='%s'"), m_myname);
	//防止网络堵塞
	try { res = stmt->executeQuery(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	if (res->next())
	{
		if (m_mypassword == res->getString("password").c_str())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool DBOperation::editjishi(CString m_vname, CString m_mypassword)
{

	CString StrSql;
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_vname, "改密", NULL, m_strTime, "jishi");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	StrSql.Format(_T("update jishi set password='%s'where userid='%s'"), m_mypassword, m_vname);
	//防止网络堵塞
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	db.execDML(StrSql);
	return true;
}

void DBOperation::addstudent(CString m_myno, CString m_myname,  CString m_vmon, CString m_vop)
{
	CString StrSql;
	StrSql.Format(_T("insert into user(userid,name,money,operation) values('%s','%s','%s','%s')"), m_myno, m_myname, m_vmon, m_vop);
	db.execDML(StrSql);
}

void DBOperation::addjishi(CString m_myno, CString m_myname, CString m_vmon, CString m_vop)
{
	CString StrSql;
	StrSql.Format(_T("insert into jishi(userid,name,money,password) values('%s','%s','%s','%s')"), m_myno, m_myname, m_vmon, m_vop);
	db.execDML(StrSql);
}

bool DBOperation::dellstudent(CString m_vno)
{
	CString StrSql;
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_vno, "删除用户", NULL, m_strTime, "user");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	StrSql.Format(_T("delete from user where userid='%s'"), m_vno);
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	db.execDML(StrSql);
}

bool DBOperation::delljishi(CString m_vno)
{
	CString StrSql;
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_vno, "删除技师", NULL, m_strTime, "jishi");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	StrSql.Format(_T("delete from jishi where userid='%s'"), m_vno);
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	db.execDML(StrSql);

}

bool DBOperation::updatejishi(CString m_vname, CString m_vmon, CString m_vop, CString m_vno)
{
	CString StrSql;
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	StrSql.Format(_T("update jishi set name='%s' , money='%s' , password='%s' where userid='%s'"), m_vname,m_vmon,m_vop, m_vno);
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	db.execDML(StrSql);
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_vno, "修改技师信息", m_vmon,m_strTime, "jishi");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	return true;
}
bool DBOperation::updatestudent(CString m_vname, CString m_vmon, CString m_vop, CString m_vno)
{
	CString StrSql;
	int ComNum = _ttoi(m_vop);
	CString a =NULL;
	if (ComNum > 0)
		a = "加钱";
	else if (ComNum < 0)
		a = "减钱";
	else
		a = "修改用户信息";
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	StrSql.Format(_T("update user set name='%s' , money='%s' , operation='%s' where userid='%s'"), m_vname, m_vmon, m_vop, m_vno);
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	db.execDML(StrSql);
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_vno, a, m_vop, m_strTime,"user");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	StrSql.Format(_T("update user set money=money+'%s' ,operation ='%s' where userid = '%s'"),m_vop,NULL, m_vno);
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return false; }
	db.execDML(StrSql);
	return true;
}

void DBOperation::dellALLstudent()
{
	CString StrSql;
	StrSql.Format(_T("delete from user"));
	db.execDML(StrSql);
}

void DBOperation::dellALLjsihi()
{
	CString StrSql;
	StrSql.Format(_T("delete from jishi"));
	db.execDML(StrSql);
}

void DBOperation::selstudent(CString m_myname)
{
	CString StrSql;
	m_studentlist.RemoveAll();
	m_studentlist.GetHeadPosition();
	StrSql.Format(_T("select * from user where userid='%s'"), m_myname);
	bool bRet = FALSE;
	CppSQLite3Query q = db.execQuery(StrSql);
	CString strResult;
	while (!q.eof())
	{
		m_mystudent.userid = q.getStringField(_T("userid"));
		m_mystudent.name = q.getStringField(_T("name"));
		m_mystudent.money = q.getStringField(_T("money"));
		m_mystudent.operation = q.getStringField(_T("operation"));
		m_studentlist.AddTail(m_mystudent);
		bRet = TRUE;
		q.nextRow();
	}
	q.finalize();
}

void DBOperation::seljishi(CString m_myname)
{
	CString StrSql;
	m_jishilist.RemoveAll();
	m_jishilist.GetHeadPosition();
	StrSql.Format(_T("select * from jishi where userid='%s'"), m_myname);
	bool bRet = FALSE;
	CppSQLite3Query q = db.execQuery(StrSql);
	CString strResult;
	while (!q.eof())
	{
		m_jishi.userid = q.getStringField(_T("userid"));
		m_jishi.name = q.getStringField(_T("name"));
		m_jishi.money = q.getStringField(_T("money"));
		m_jishi.password = q.getStringField(_T("password"));
		m_jishilist.AddTail(m_jishi);
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
	StrSql.Format(_T("select * from user"));
	bool bRet = FALSE;
	CppSQLite3Query q = db.execQuery(StrSql);
	CString strResult;
	while (!q.eof())
	{
		m_mystudent.userid = q.getStringField(_T("userid"));
		m_mystudent.name = q.getStringField(_T("name"));
		m_mystudent.money = q.getStringField(_T("money"));
		m_mystudent.operation = q.getStringField(_T("operation"));
		m_studentlist.AddTail(m_mystudent);
		bRet = TRUE;
		q.nextRow();
	}
	q.finalize();
}

void DBOperation::selalljishi()
{
	CString StrSql;
	m_jishilist.RemoveAll();
	m_jishilist.GetHeadPosition();
	StrSql.Format(_T("select * from jishi"));
	bool bRet = FALSE;
	CppSQLite3Query q = db.execQuery(StrSql);
	CString strResult;
	while (!q.eof())
	{
		m_jishi.userid = q.getStringField(_T("userid"));
		m_jishi.name = q.getStringField(_T("name"));
		m_jishi.money = q.getStringField(_T("money"));
		m_jishi.password = q.getStringField(_T("password"));
		m_jishilist.AddTail(m_jishi);
		bRet = TRUE;
		q.nextRow();
	}
	q.finalize();
}
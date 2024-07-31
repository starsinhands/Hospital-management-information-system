#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <string.h>
using namespace std;


//数据库连接管理类
class MYSQLMSG
{
public:
	char* host     = nullptr;                 //主机地址localhost
	char* username = nullptr;                 //用户名root
	char* password = nullptr;                 //密码
	char* database = nullptr;                 //数据库
	int port = 0;                             //端口
	char* unix_socket = nullptr;              //本地套接字
	unsigned long clientflag = 0;             //连接标志

	MYSQLMSG() {}                             //无参构造函数


	//有参构造函数
	MYSQLMSG(const char* host, const char* username, const char* password, const char* database, int port, const char* unix_socket, unsigned long clientflag)
	{
		this->host = (char*)host;
		this->username = (char*)username;
		this->password = (char*)password;
		this->database = (char*)database;
		this->port = port;
		this->unix_socket = (char*)unix_socket;
		this->clientflag = clientflag;
	}


	//数据初始化
	void loadmsg()
	{
		printf("\n<------------------------------>\n");

		host = new char[20];
		printf("输入host(主机地址localhost): ");
		scanf_s("%s", host, 20);

		username = new char[20];
		printf("输入username(用户名root): ");
		scanf_s("%s", username, 20);

		password = new char[20];
		printf("输入password(密码): ");
		scanf_s("%s", password, 20);

		database = new char[20];
		printf("输入database(目标数据库): ");
		scanf_s("%s", database, 20);


		printf("输入port(端口): ");
		scanf_s("%d", &port);

		unix_socket = new char[20];
		printf("输入unix_socket(本地套接字): ");
		scanf_s("%s", unix_socket, 20);


		printf("输入clientflag(连接标志): ");
		scanf_s("%ld", &clientflag);

		printf("\n<------------------------------->\n");

	}

};




/*----------------------------函数预定义----------------------------*/
void LinkDatabase(MYSQL& mysql);        //连接数据库
void hospital_system(MYSQL& mysql);     //医院管理系统主要模块

//----------------------管理员功能------------------------------
void administrator(MYSQL& mysql);       //管理员功能模块
//管理员登录
bool adm_login(MYSQL& mysql);           
//管理员医院信息功能
void adm_hospitalmsg(MYSQL& mysql);
//管理员医生信息功能
void adm_doctormsg(MYSQL& mysql);
//管理员病人信息功能
void adm_patientmsg(MYSQL& mysql);
//管理员药品信息功能
void adm_medicinemsg(MYSQL& mysql);
//--------------------------------------------------------------


//-------------------------医生功能-----------------------------
void doctor(MYSQL& mysql);              //医生功能模块
//医生登录
string doc_login(MYSQL& mysql);
//医生排班情况
void doc_schedule(MYSQL& mysql, string doctorNo);
//医生接诊病人
void doc_patient(MYSQL& mysql, string doctorNo);
//医生开具处方
void doc_prescription(MYSQL& mysql, string doctorNo);
//病人缴费记录
void pat_bill(MYSQL& mysql, string patientNo);
//--------------------------------------------------------------


//-------------------------病人功能-----------------------------
void patient(MYSQL& mysql);             //病人功能模块
//病人登录
string pat_login(MYSQL& mysql);
//病人挂号
void pat_registration(MYSQL& mysql, string patientNo);
//--------------------------------------------------------------


/*------------------------------------------------------------------*/



//连接数据库，返回MYSQL类变量
void LinkDatabase(MYSQL& mysql)
{
	MYSQLMSG* mysqlmsg = new MYSQLMSG("localhost", "root", "", "hospital_system", 3308, NULL, 0);
	while (true)
	{
		char flag=0;
		printf("==是否开始连接<Y确定|N退出>==:");
		scanf_s("%c", &flag,1);
		if (flag == 'N' || flag == 'n')//拒绝连接
		{
			printf("3秒后退出连接\n");
			for (int i = 3; i > 0; i--)
			{
				Sleep(950);
				printf("%d\n", i);
			}
			exit(-1);
			break;
		}
		else if (flag == 'Y' || flag == 'y')//开始连接
		{
			mysqlmsg->loadmsg();
			printf("Loading......\n");
			if (mysql_real_connect(&mysql, mysqlmsg->host, mysqlmsg->username,mysqlmsg->password,mysqlmsg->database,mysqlmsg->port, mysqlmsg->unix_socket, mysqlmsg->clientflag) == NULL)
			{
				//连接失败，输出信息
				printf("错误原因：%s\n", mysql_error(&mysql));
				printf("连接失败\n");
				printf("5秒后退出......\n");
				Sleep(5000);
				exit(-1);
			}
			else//连接成功，输出信息
			{
				system("cls");
				printf("link succeed!\n\n");
				printf("---------------------------------------------\n");
				printf("<=连接信息=>\n");
				printf("host:        %s\n",  mysqlmsg->host);
				printf("user:        %s\n",  mysqlmsg->username);
				printf("database:    %s\n",  mysqlmsg->database);
				printf("port:        %d\n",  mysqlmsg->port);
				printf("unix_socket: %s\n",  mysqlmsg->unix_socket);
				printf("clientflag:  %ld\n", mysqlmsg->clientflag);
				printf("---------------------------------------------\n");
				system("pause");
				break;
			}
		}
		else//非法输入处理
		{
			system("cls");
			continue;
		}
	}

	
}

//医院管理系统，主要功能
void hospital_system(MYSQL& mysql)
{
	int select = 0;                     //选项

	while (true)//系统主界面
	{
		system("cls");
		printf("<=欢迎使用医院管理系统=>\n");
		printf("+------<选择用户>-------+\n");
		printf("|1.\t管理员\t\t|\n");
		printf("|2.\t医生  \t\t|\n");
		printf("|3.\t病人  \t\t|\n");
		printf("|0.\t退出  \t\t|\n");
		printf("+-----------------------+\n");
		printf("输入选项<0-3>: ");
		scanf_s("%d", &select);
		if (select == 1)//进入管理员功能
		{
			administrator(mysql);
		}
		else if (select == 2)//进入医生功能
		{
			doctor(mysql);
		}
		else if (select == 3)//进入病人功能
		{
			patient(mysql);
		}
		else if (select == 0)//退出程序
		{
			break;
		}
		else//错误操作
		{
			continue;
		}

	}
	
	

	system("pause");
}


//管理员登录
bool adm_login(MYSQL& mysql)
{
	char account[20]="";//账号
	char password[20]="";//密码
	system("cls");
	printf("<= 医院管理系统>>管理员登录 =>\n");
	printf("--------------------------------\n");
	printf("输入账号: ");
	scanf_s("%s", account, 20);
	printf("输入密码: ");
	scanf_s("%s", password, 20);
	printf("--------------------------------\n");
	if (strcmp(account, "admin")==0)
	{
		if (strcmp(password, "123456")==0)
		{
			printf("登录成功!\n");
			printf("loading");
			for (int i = 6; i > 0; i--)
			{
				printf(".");
				Sleep(500);
			}
			printf("\n");
			return false;
		}
		else
		{
			printf("账号密码错误\n");
			Sleep(1500);
			return true;
		}
	}
	else 
	{
		printf("账号密码错误!\n");
		Sleep(1500);
		return true;
	}	
}
//管理员医院信息功能
void adm_hospitalmsg(MYSQL& mysql)
{

}
//管理员医生信息功能
void adm_doctormsg(MYSQL& mysql)
{
	MYSQL_RES* res;      //返回结果
	MYSQL_ROW row;       //结果行
	MYSQL_FIELD* field;    //结果列
	int select = 0;      //功能选项
	int field_count = 0; //结果列数
	while (true)
	{
		//执行查询
		if (mysql_query(&mysql, "select * from doctor") != 0)
		{
			break;
		}
		res = mysql_store_result(&mysql);//获取结果集
		field_count = mysql_num_fields(res);//获取列数
		field = mysql_fetch_field(res);//获取列
		//输出病人数据
		system("cls");
		printf("<= 医院管理系统>>管理员功能>>医生信息 =>\n");
		printf("+---------------------------------------------------------------------------------+\n");
		printf("|doctorNo  roomNo\tname\t\tsex \ttitle\ttelephone\tsalary\t  |\n");
		printf("+---------------------------------------------------------------------------------+\n");

		while (row = mysql_fetch_row(res))//获取遍历行结果
		{
			printf("|%-8s  ", row[0]);
			printf("%-5s \t", row[1]);
			printf("%-8s\t", row[2]);
			printf("%-3s \t", row[3]);
			printf("%-4s \t", row[4]);
			printf("%-12s \t", row[5]);
			printf("%-8s  |\n", row[6]);
		}
		printf("+---------------------------------------------------------------------------------+\n");
		//功能部分
		printf("+------<选择功能>-------+\n");
		printf("|1.\t添加医生信息\t|\n");
		printf("|2.\t修改医生信息\t|\n");
		printf("|3.\t删除医生信息\t|\n");
		printf("|0.\t退出  \t\t|\n");
		printf("+-----------------------+\n");
		printf("输入选项<0-3>: ");
		scanf_s("%d", &select);
		if (select == 0)
		{
			return;
		}
		else if (select == 1)
		{
			string sql_insert = "insert into doctor(doctorNo,roomNo,name,sex,title,telephone,salary) values ( ";
			string doctorNo;
			string roomNo;
			string name;
			string sex;
			string title;
			string telephone;
			string salary;
			printf("\n<输入要添加的医生信息>\n");
			printf("编号: ");
			cin >> doctorNo;
			printf("房间号: ");
			cin >> roomNo;
			printf("姓名: ");
			cin >> name;
			printf("性别: ");
			cin >> sex;
			printf("职称: ");
			cin >> title;
			printf("电话号码: ");
			cin >> telephone;
			printf("工资: ");
			cin >> salary;
			string sql = sql_insert + "\'" + doctorNo + "\'," + "\'" + roomNo + "\',"+ "\'" + name + "\'," + "\'" + sex + "\'," + "\'" + title + "\'," + "\'" + telephone + "\'," + salary + ")";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0)
			{
				printf("\n医生 %s 添加成功!\n\n", doctorNo.c_str());
			}
			else
			{
				printf("\n医生 %s 添加失败!\n\n", doctorNo.c_str());
			}
			system("pause");
		}
		else if (select == 2)
		{
			string sql_update = "update doctor set ";
			string target;
			string doctorNo;
			string roomNo;
			string name;
			string sex;
			string title;
			string telephone;
			string salary;
			printf("输入要修改的医生编号: ");
			cin >> target;
			printf("\n<输入修改后的医生信息>\n");
			printf("编号: ");
			cin >> doctorNo;
			printf("房间号: ");
			cin >> roomNo;
			printf("姓名: ");
			cin >> name;
			printf("性别: ");
			cin >> sex;
			printf("职称: ");
			cin >> title;
			printf("电话号码: ");
			cin >> telephone;
			printf("工资: ");
			cin >> salary;
			string sql = sql_update + "doctorNo = \'" + doctorNo + "\'," + "roomNo = \'" + roomNo + "\'," + "name = \'" + name + "\'," + "sex = \'" + sex + "\'," + "title = \'" + title + "\'," + "telephone = \'" + telephone + "\'," + "salary = " + salary + " where doctorNo = " + "\'" + target + "\'";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0)
			{
				printf("\n医生 %s 修改成功!\n\n", doctorNo.c_str());
			}
			else
			{
				printf("\n医生 %s 修改失败!\n\n", target.c_str());
			}
			system("pause");
		}
		else if (select == 3)
		{
			string sql_del = "delete from doctor where doctorNo = ";
			string doctorNo;
			string sql;
			printf("输入要删除的医生编号: ");
			cin >> doctorNo;
			sql = sql_del + "\"" + doctorNo + "\"";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0)
			{
				printf("\n医生 %s 删除成功\n\n", doctorNo.c_str());
			}
			else
			{
				printf("\n医生 %s 删除失败\n\n", doctorNo.c_str());
			}
			system("pause");
		}
		else
		{
			continue;
		}
	}
}
//管理员病人信息功能
void adm_patientmsg(MYSQL& mysql)
{

	MYSQL_RES* res;      //返回结果
	MYSQL_ROW row;       //结果行
	MYSQL_FIELD* field;    //结果列
	int select = 0;      //功能选项
	int field_count = 0; //结果列数
	while (true)
	{	
		//执行查询
		if ( mysql_query(&mysql, "select * from patient") != 0 )
		{
			break;
		}
		res = mysql_store_result(&mysql);//获取结果集
		field_count = mysql_num_fields(res);//获取列数
		field = mysql_fetch_field(res);//获取列
		//输出病人数据
		system("cls");
		printf("<= 医院管理系统>>管理员功能>>病人信息 =>\n");
		printf("+--------------------------------------------------------------------------------------------------------------------+\n");
		printf("|patientNo\tname\t\tsex\t\taddress \t\ttelephone\t\tIDnumber\t     |\n");
		printf("+--------------------------------------------------------------------------------------------------------------------+\n");

		while (row = mysql_fetch_row(res))//获取遍历行结果
		{
			printf("|%-9s\t", row[0]);
			printf("%-8s\t", row[1]);
			printf("%-3s\t\t", row[2]);
			printf("%-20s\t", row[3]);
			printf("%-9s\t\t", row[4]);
			printf("%-10s   |\n", row[5]);
		}
		printf("+--------------------------------------------------------------------------------------------------------------------+\n");
		//功能部分
		printf("+------<选择功能>-------+\n");
		printf("|1.\t添加病人信息\t|\n");
		printf("|2.\t修改病人信息\t|\n");
		printf("|3.\t删除病人信息\t|\n");
		printf("|0.\t退出  \t\t|\n");
		printf("+-----------------------+\n");
		printf("输入选项<0-3>: ");
		scanf_s("%d", &select);
		if (select == 0)
		{
			return;
		}
		else if (select == 1)
		{
			string sql_insert = "insert into patient(patientNo,name,sex,address,telephone,IDnumber) values ( ";
			string patientNo;
			string name;
			string sex;
			string address;
			string telephone;
			string IDnumber;
			printf("<输入要添加的病人信息>\n");
			printf("编号: ");
			cin >> patientNo;
			printf("姓名: ");
			cin >> name;
			printf("性别: ");
			cin >> sex;
			printf("地址: ");
			cin >> address;
			printf("电话号码: ");
			cin >> telephone;
			printf("IDnumber: ");
			cin >> IDnumber;
			string sql = sql_insert + "\'" + patientNo + "\'," + "\'" + name +"\'," + "\'" + sex + "\'," + "\'" + address + "\'," + "\'" + telephone + "\'," + "\'" + IDnumber + "\')";
			cout << "< " << sql <<" >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0)
			{
				printf("\n病人 %s 添加成功!\n\n",patientNo.c_str());
			}
			else
			{
				printf("\n病人 %s 添加失败!\n\n", patientNo.c_str());
			}
			system("pause");
		}
		else if (select == 2)
		{
			string sql_update = "update patient set ";
			string target;
			string patientNo;
			string name;
			string sex;
			string address;
			string telephone;
			string IDnumber;
			printf("输入要修改的病人编号: ");
			cin >> target;
			printf("\n<输入修改后的病人信息>\n");
			printf("编号: ");
			cin >> patientNo;
			printf("姓名: ");
			cin >> name;
			printf("性别: ");
			cin >> sex;
			printf("地址: ");
			cin >> address;
			printf("电话号码: ");
			cin >> telephone;
			printf("IDnumber: ");
			cin >> IDnumber;
			string sql = sql_update + "patientNo = \'" + patientNo + "\'," + "name = \'" + name + "\'," + "sex = \'" + sex + "\'," + "address = \'" + address + "\'," + "telephone = \'" + telephone + "\'," + "IDnumber = \'" + IDnumber + "\' where patientNo = " + "\'" + target + "\'";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0 )
			{
				printf("\n病人 %s 修改成功!\n\n", patientNo.c_str());
			}
			else
			{
				printf("\n病人 %s 修改失败!\n\n", target.c_str());
			}
			system("pause");
		}
		else if (select == 3)
		{
			string sql_del = "delete from patient where patientNo = ";
			string patientNo;
			string sql;
			printf("输入要删除的病人编号: ");
			cin >> patientNo;
			sql = sql_del + "\"" + patientNo + "\"";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str() ) == 0)
			{
				printf("\n病人 %s 删除成功\n\n", patientNo.c_str() );
			}
			else
			{
				printf("\n病人 %s 删除失败\n\n", patientNo.c_str() );
			}
			system("pause");
		}
		else
		{
			continue;
		}
	}
}
//管理员药品信息功能
void adm_medicinemsg(MYSQL& mysql)
{
	MYSQL_RES* res;      //返回结果
	MYSQL_ROW row;       //结果行
	MYSQL_FIELD* field;    //结果列
	int select = 0;      //功能选项
	int field_count = 0; //结果列数
	while (true)
	{
		//执行查询
		if (mysql_query(&mysql, "select * from medicine") != 0)
		{
			break;
		}
		res = mysql_store_result(&mysql);//获取结果集
		field_count = mysql_num_fields(res);//获取列数
		field = mysql_fetch_field(res);//获取列
		//输出病人数据
		system("cls");
		printf("<= 医院管理系统>>管理员功能>药品信息 =>\n");
		printf("+-----------------------------------------------+\n");
		printf("|medicineNo  name    \tprice\tstorage\t\t|\n");
		printf("+-----------------------------------------------+\n");

		while (row = mysql_fetch_row(res))//获取遍历行结果
		{
			printf("|%-10s  ", row[0]);
			printf("%-10s\t", row[1]);
			printf("%-5s\t", row[2]);
			printf("%-7s\t\t|\n", row[3]);
		}
		printf("+-----------------------------------------------+\n");
		//功能部分
		printf("+------<选择功能>-------+\n");
		printf("|1.\t添加药品信息\t|\n");
		printf("|2.\t修改药品信息\t|\n");
		printf("|3.\t删除药品信息\t|\n");
		printf("|0.\t退出  \t\t|\n");
		printf("+-----------------------+\n");
		printf("输入选项<0-3>: ");
		scanf_s("%d", &select);
		if (select == 0)
		{
			return;
		}
		else if (select == 1)
		{
			string sql_insert = "insert into medicine(medicineNo,name,price,storage) values ( ";
			string medicineNo;
			string name;
			string price;
			string storage;
			printf("<输入要添加的药品信息>\n");
			printf("编号: ");
			cin >> medicineNo;
			printf("姓名: ");
			cin >> name;
			printf("价格: ");
			cin >> price;
			printf("storage: ");
			cin >> storage;
			string sql = sql_insert + "\'" + medicineNo + "\'," + "\'" + name + "\'," + "\'" + price + "\'," + "\'" + storage + "\')";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0)
			{
				printf("\n药品 %s 添加成功!\n\n", medicineNo.c_str());
			}
			else
			{
				printf("\n药品 %s 添加失败!\n\n", medicineNo.c_str());
			}
			system("pause");
		}
		else if (select == 2)
		{
			string sql_update = "update medicine set ";
			string target;
			string medicineNo;
			string name;
			string price;
			string storage;
			printf("输入要修改的药品编号: ");
			cin >> target;
			printf("<输入修改后的药品信息>\n");
			printf("编号: ");
			cin >> medicineNo;
			printf("姓名: ");
			cin >> name;
			printf("价格: ");
			cin >> price;
			printf("storage: ");
			cin >> storage;
			string sql = sql_update + "medicineNo = \'" + medicineNo + "\'," + "name = \'" + name + "\'," + "price = \'" + price + "\'," + "storage = \'" + storage + "\' where medicineNo = \'" + target + "\'";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0)
			{
				printf("\n药品 %s 修改成功!\n\n", medicineNo.c_str());
			}
			else
			{
				printf("\n药品 %s 修改失败!\n\n", target.c_str());
			}
			system("pause");
		}
		else if (select == 3)
		{
			string sql_del = "delete from medicine where medicineNo = ";
			string medicineNo;
			string sql;
			printf("输入要删除的药品编号: ");
			cin >> medicineNo;
			sql = sql_del + "\"" + medicineNo + "\"";
			cout << "< " << sql << " >\n";
			if (mysql_query(&mysql, sql.c_str()) == 0)
			{
				printf("\n药品 %s 删除成功\n\n", medicineNo.c_str());
			}
			else
			{
				printf("\n药品 %s 删除失败\n\n", medicineNo.c_str());
			}
			system("pause");
		}
		else
		{
			continue;
		}
	}
}
//管理员功能
void administrator(MYSQL& mysql)
{
	if (adm_login(mysql))//登录失败返回
	{
		return;
	}

	int select = 0;//管理员功能选项
	while (true)
	{
		system("cls");
		printf("<= 医院管理系统>>管理员功能 =>\n\n");
		printf("+------<选择功能>-------+\n");
		printf("|1.\t医院基本信息\t|\n");
		printf("|2.\t医生基本信息\t|\n");
		printf("|3.\t病人基本信息\t|\n");
		printf("|4.\t药品基本信息\t|\n");
		printf("|0.\t退出  \t\t|\n");
		printf("+-----------------------+\n");
		printf("输入选项<0-4>: ");
		scanf_s("%d", &select);
		if (select == 0)//退出
		{
			return;
		}
		else if (select == 1)//医院基本信息
		{
			adm_hospitalmsg(mysql);
		}
		else if (select == 2)//医生基本信息
		{
			adm_doctormsg(mysql);
		}
		else if (select == 3)//病人基本信息
		{
			adm_patientmsg(mysql);
		}
		else if (select == 4)//药品基本信息
		{
			adm_medicinemsg(mysql);
		}
		else
		{
			continue;
		}
	}

}






//医生登录
string doc_login(MYSQL& mysql)	
{	char account[20] = "";//账号
	char password[20] = "";//密码
	system("cls");
	printf("<= 医院管理系统>>医生登录 =>\n\n");
	printf("=--------------<登录>--------------=\n");
	printf("输入医生编号: ");
	scanf_s("%s", account, 20);
	printf("输入密码: ");
	scanf_s("%s", password, 20);
	printf("=----------------------------------=\n");
	MYSQL_RES* res = nullptr;
	MYSQL_ROW row = NULL;
	if (mysql_query(&mysql, "select doctorNo from doctor") != 0)
	{
		printf("error!\n");
		system("pause");
		return "\0";
	}
	else
	{
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			if (strcmp(account, row[0])== 0 )
			{
				if (strcmp(password, "123456") == 0 )
				{
					printf("登录成功!\n");
					printf("Loading");
					for (int i = 6;i > 0; i--)
					{
						printf(".");
						Sleep(400);
					}
					printf("\n");
					return account;
				}
			}
		}
	}
	printf("账号密码错误！\n");
	Sleep(1500);
	return "\0";
}

//医生排班情况
void doc_schedule(MYSQL& mysql,string doctorNo)
{
	MYSQL_RES* res;                  //返回结果
	MYSQL_ROW row;                   //结果行
	MYSQL_FIELD* field = nullptr;    //结果列
	int flag = 0;                    //标记
	int select = 0;                  //功能选项
	int field_count = 0;             //结果列数
	system("cls");
	printf("<= 医院管理系统>>医生功能>>排班情况(医生: %s) =>\n\n", doctorNo.c_str());
	string sql = "select * from schedule";
	if (mysql_query( &mysql,sql.c_str() ) != 0)
	{
		printf("查询不到排版情况\n");
		Sleep(2000);
		return;
	}
	else
	{
		res = mysql_store_result(&mysql);
		printf("\n排班情况如下:\n");
		printf("+---------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
		printf("|recordNo  doctorNo  roonNo  startTime\t\t\tendTime\t\t\tbillNo  prescriptionNo  diagnosis\t        recordTime\t\ttype    |\n");
		printf("+---------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
		while (row = mysql_fetch_row(res))
		{
			if (strcmp(doctorNo.c_str(), row[1]) == 0)
			{
				flag = 1;
				printf("|%-8s  ", row[0]);
				printf("%-8s  ", row[1]);
				printf("%-6s  ", row[2]);
				printf("%s\t", row[3]);
				printf("%s\t", row[4]);
				printf("%-6s  ", row[5]);
				printf("%-14s  ", row[6]);
				printf("%-20s\t", row[7]);
				printf("%s\t", row[8]);
				printf("%-8s|\n", row[9]);
			}
			else
			{
				continue;
			}
		}
		if (flag == 1)
		{
			printf("+---------------------------------------------------------------------------------------------------------------------------------------------------------------+\n\n");
			system("pause");
		}
		else
		{
			printf("!!!查询不到任务!!!\n");
			printf("\n<<目前没有排班任务>>\n\n");
			system("pause");
			return;
		}
		
	}
	return;
}

//医生接诊病人
void doc_patient(MYSQL& mysql, string doctorNo)
{
	MYSQL_RES* res;                  //返回结果
	MYSQL_ROW row;                   //结果行
	MYSQL_FIELD* field = nullptr;    //结果列
	int flag = 0;                    //标记
	int select = 0;                  //功能选项
	int field_count = 0;             //结果列数
	system("cls");
	printf("<= 医院管理系统>>医生功能>>接诊病人(医生: %s) =>\n\n", doctorNo.c_str());
	string sql = "select * from document";
	if (mysql_query(&mysql, sql.c_str()) != 0)
	{
		printf("查询不到接诊病人\n");
		Sleep(2000);
		return;
	}
	else
	{
		res = mysql_store_result(&mysql);
		printf("\n接诊病人如下:\n");
		printf("+---------------+\n");
		printf("| patientNo \t|\n");
		printf("+---------------+\n");
		while (row = mysql_fetch_row(res))
		{
			if (strcmp(doctorNo.c_str(), row[1]) == 0)
			{
				flag = 1;
				printf("| %-9s\t|\n", row[2]);
			}
			else
			{
				continue;
			}
		}
		if (flag == 1)
		{
			printf("+---------------+\n\n");
			system("pause");
		}
		else
		{
			printf("!!!查询不到病人!!!\n");
			printf("\n<<目前没有接诊病人>>\n\n");
			system("pause");
			return;
		}

	}
	return;
}

//医生开具处方
void doc_prescription(MYSQL& mysql, string doctorNo)
{
	MYSQL_RES* res;                  //返回结果
	MYSQL_ROW row;                   //结果行
	MYSQL_FIELD* field = nullptr;    //结果列
	int select = 0;                  //功能选项
	int field_count = 0;             //结果列数
	system("cls");
	printf("<= 医院管理系统>>医生功能>>开具处方(医生: %s) =>\n\n", doctorNo.c_str());
	string sql = "select * from prescription";
	if (mysql_query(&mysql, sql.c_str()) != 0)
	{
		printf("查询不到处方\n");
		Sleep(2000);
		return;
	}
	else
	{
		res = mysql_store_result(&mysql);
		printf("\n开具处方如下:\n");
		printf("+--------------------------------------------------------------------------------------------------------+\n");
		printf("|prescriptionNo  patientNo doctorNo number symptom\t\tsaveTime\t\thealPrice\t |\n");
		printf("+--------------------------------------------------------------------------------------------------------+\n");
		while (row = mysql_fetch_row(res))
		{
				printf("|%-15s ", row[0]);
				printf("%-9s ", row[1]);
				printf("%-8s ", row[2]);
				printf("%-6s ", row[3]);
				printf("%-20s", row[4]);
				printf("%-24s ", row[5]);
				printf("%-9s\t |\n", row[6]);
		}
	}
	printf("+--------------------------------------------------------------------------------------------------------+\n\n");
	system("pause");
	return;
}

//医生功能
void doctor(MYSQL& mysql)
{
	string doctorNo = doc_login(mysql);
	if (doctorNo.empty() )//登录失败返回
	{
		return;
	}

	int select = 0;//医生功能选项
	while (true)
	{
		system("cls");
		printf("<= 医院管理系统>>医生功能(医生: %s) =>\n\n", doctorNo.c_str() );
		printf("+------<选择功能>-------+\n");
		printf("|1.\t排班情况\t|\n");
		printf("|2.\t接诊病人\t|\n");
		printf("|3.\t开具处方\t|\n");
		printf("|0.\t退出  \t\t|\n");
		printf("+-----------------------+\n");
		printf("输入选项<0-3>: ");
		scanf_s("%d", &select);
		if (select == 0)//退出
		{
			return;
		}
		else if (select == 1)//排班情况
		{
			doc_schedule(mysql,doctorNo);
		}
		else if (select == 2)//接诊病人
		{
			doc_patient(mysql, doctorNo);
		}
		else if (select == 3)//开具处方
		{
			doc_prescription(mysql, doctorNo);
		}
		else
		{
			continue;
		}
	}
}

//病人登录
string pat_login(MYSQL& mysql)
{
	char account[20] = "";//账号
	char password[20] = "";//密码
	system("cls");
	printf("<= 医院管理系统>>病人登录 =>\n\n");
	printf("=--------------<登录>--------------=\n");
	printf("输入病人编号: ");
	scanf_s("%s", account, 20);
	printf("输入密码: ");
	scanf_s("%s", password, 20);
	printf("=----------------------------------=\n");
	MYSQL_RES* res = nullptr;
	MYSQL_ROW row = NULL;
	if (mysql_query(&mysql, "select patientNo from patient") != 0)
	{
		printf("error!\n");
		system("pause");
		return "\0";
	}
	else
	{
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			if (strcmp(account, row[0]) == 0)
			{
				if (strcmp(password, "123456") == 0)
				{
					printf("登录成功!\n");
					printf("Loading");
					for (int i = 6; i > 0; i--)
					{
						printf(".");
						Sleep(400);
					}
					printf("\n");
					return account;
				}
			}
		}
	}
	printf("账号密码错误！\n");
	Sleep(1500);
	return "\0";
}

//病人挂号
void pat_registration(MYSQL& mysql,string patientNo)
{
	int select = 0;                  //功能选项
	string registrationNo;
	string roomNo;
	string regTime;
	string sql_insert = "insert into registration(registrationNo,patientNo,roomNo,regTime) values (";
	system("cls");
	printf("<= 医院管理系统>>病人功能>>挂号(病人: %s) =>\n\n", patientNo.c_str());
	printf("-----------------------------------------------\n");
	printf("输入挂号编号: ");
	cin >> registrationNo;
	printf("输入科室号: ");
	cin >> roomNo;
	printf("输入预约时间: ");
	cin >> regTime;
	printf("-----------------------------------------------\n");
	string sql = sql_insert + "\'" + registrationNo + "\'," + "\'" + patientNo + "\'," + "\'" + roomNo + "\'," + "\'" + regTime + "\')";
	printf("%s\n", sql.c_str());
	if (mysql_query(&mysql, sql.c_str()) != 0 )
	{
		printf("挂号失败!\n");
		system("pause");
	}
	else
	{
		printf("挂号成功!\n");
		system("pause");
	}
}

//病人缴费
void pat_pay(MYSQL& mysql, string patientNo)
{
	int select = 0;                  //功能选项
	int field_count = 0;             //结果列数
	string billNo;
	string payTime;
	string totalCost;
	string sql_insert = "insert into bill(billNo,patientNo,payTime,totalCost) values (";
	system("cls");
	printf("<= 医院管理系统>>病人功能>>缴费(病人: %s) =>\n\n", patientNo.c_str());
	printf("-----------------------------------------------\n");
	printf("输入缴费编号: ");
	cin >> billNo;
	printf("输入缴费时间: ");
	cin >> payTime;
	printf("输入缴费金额: ");
	cin >> totalCost;
	printf("-----------------------------------------------\n");
	string sql = sql_insert + "\'" + billNo + "\'," + "\'" + patientNo + "\'," + "\'" + payTime + "\'," + "\'" + totalCost + "\')";
	printf("%s\n", sql.c_str());
	if (mysql_query(&mysql, sql.c_str()) != 0)
	{
		printf("缴费失败!\n");
		system("pause");
	}
	else
	{
		printf("缴费成功!\n");
		system("pause");
	}
}

//病人缴费记录
void pat_bill(MYSQL& mysql, string patientNo)
{
	MYSQL_RES* res;                  //返回结果
	MYSQL_ROW row;                   //结果行
	MYSQL_FIELD* field = nullptr;    //结果列
	int select = 0;                  //功能选项
	int field_count = 0;             //结果列数
	system("cls");
	printf("<= 医院管理系统>>病人功能>>缴费记录(病人: %s) =>\n\n", patientNo.c_str());
	string sql = "select * from bill";
	if (mysql_query(&mysql, sql.c_str()) != 0)
	{
		printf("查询不到缴费记录\n");
		Sleep(2000);
		return;
	}
	else
	{
		res = mysql_store_result(&mysql);
		printf("\n缴费记录如下:\n");
		printf("+--------------------------------------------------+\n");
		printf("|billNo  patientNo  payTime\t\t totalcost |\n");
		printf("+--------------------------------------------------+\n");
		while (row = mysql_fetch_row(res))
		{
			printf("|%-6s  ", row[0]);
			printf("%-9s  ", row[1]);
			printf("%-20s", row[2]);
			printf(" %-9s |\n", row[3]);
		}
	}
	printf("+--------------------------------------------------+\n\n");
	system("pause");
	return;
}

//病人功能
void patient(MYSQL& mysql)
{
	string patientNo = pat_login(mysql);
	if ( patientNo.empty() )//登录失败返回
	{
		return;
	}
	int select = 0;//病人功能选项
	while (true)
	{
		system("cls");
		printf("<= 医院管理系统>>病人功能(病人: %s) =>\n\n",patientNo.c_str() );
		printf("+------<选择功能>-------+\n");
		printf("|1.\t挂号\t\t|\n");
		printf("|2.\t缴费\t\t|\n");
		printf("|3.\t缴费明细\t|\n");
		printf("|0.\t退出  \t\t|\n");
		printf("+-----------------------+\n");
		printf("输入选项<0-5>: ");
		scanf_s("%d", &select);
		if (select == 0)//退出
		{
			return;
		}
		else if (select == 1)//挂号
		{
			pat_registration(mysql, patientNo);
		}
		else if (select == 2)//缴费
		{
			pat_pay(mysql, patientNo);
		}
		else if (select == 3)//缴费明细
		{
			pat_bill(mysql, patientNo);
		}
		else
		{
			continue;
		}
	}
}






int main()
{
	MYSQL mysql;         //数据库

	mysql_init(&mysql);//初始化数据库
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");//设置编码
	//mysql_set_character_set(&mysql, "gbk");//设置编码
	LinkDatabase(mysql);//连接数据库
	hospital_system(mysql);//进入医院管理系统
}
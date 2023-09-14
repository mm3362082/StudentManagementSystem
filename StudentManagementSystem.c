/*学生管理系统，该系统能实现对学生姓名，学号，性别，所属学院，入学日期的增添，修改，删除，和查询四大功能*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <stdlib.h>

//学生数据库存放位置
#define SQL_PATH ("./student.db")

sqlite3 *students_sql=NULL;//数据库描述符
static int callback2(void *data2,int argc,char **argv,char **azColName);

void function_add()//增添学生功能块
{
    char num[20]={0};//学号
    char name[20]={0};//姓名
    int age=-1;//年龄
    char sex[5]={0};//性别
    char college[64]={0};//所属学院
    char class[64]={0};//所属班级
    int chinese=-1;//语文成绩
    int math=-1;//数学成绩
    int english=-1;//英语成绩

    //获取新学生的信息
    printf("请输入学生学号：\n");
    scanf("%s",num);
    printf("请输入学生姓名：\n");
    scanf("%s",name);
    printf("请输入学生年龄：\n");
    scanf("%d",&age);
    printf("请输入学生性别：\n");
    scanf("%s",sex);
    printf("请输入学生所属学院：\n");
    scanf("%s",college);
    printf("请输入学生所属班级)\n");
    scanf("%s",class);
    printf("请输入学生语文成绩)\n");
    scanf("%d",&chinese);
    printf("请输入学生数学成绩)\n");
    scanf("%d",&math);
    printf("请输入学生英语成绩)\n");
    scanf("%d",&english);
    printf("输入成功！\n");
    
    //添加到SQL语句中
    char sql_state[512];
    sprintf(sql_state,"insert into students values(0,'%s','123456','%s','%s','%d','%s','%s','%s',%d,%d,%d);",num,num,name,age,sex,college,class,chinese,math,english);
    char *errmsg = NULL;
    if( sqlite3_exec(students_sql,sql_state,NULL,NULL,&errmsg) != SQLITE_OK )
    {
        printf("%s",errmsg);
        sqlite3_free(errmsg);
        return ;
    }
    printf("权限:0,账号:%s,密码:123456\n学号:%s,姓名:%s,年龄:%d,性别:%s,所属学院:%s,所属班级:%s\n语文成绩:%d,数学成绩:%d,英语成绩:%d\n学生创建完毕\n",num,num,name,age,sex,college,class,chinese,math,english);
    return ;
}

void function_delete()//删除学生功能块
{
    char flag[10]={0};//索引
    char flag2=0;
    printf("请输入需要删除的学生的学号：\n");
    scanf("%s",flag);
    int flag3=atoi(flag);
    char sql_state[512]={0};
    sprintf(sql_state,"select * from students where id=%d;",flag3);
    char *errmsg=NULL;
    if( SQLITE_OK != sqlite3_exec(students_sql,sql_state,callback2,NULL,&errmsg) )
    {
        printf("%s",errmsg);
        sqlite3_free(errmsg);
        return ;
    }
    printf("确定删除?(Y/N):");
    getchar();
    scanf("%c",&flag2);
    if(flag2=='Y')
    {
        char sql_state2[512]={0};
        sprintf(sql_state2,"delete from students where id=%s;",flag);
        if( SQLITE_OK != sqlite3_exec(students_sql,sql_state2,NULL,NULL,&errmsg) )
        {
            printf("%s",errmsg);
            sqlite3_free(errmsg);
            return ;
        }
        printf("删除成功！\n");
    }
    return;

}

/*void function_recompose()//修改学生信息功能块
{
    char flag[10];//索引
    char function[];//命令
    printf("请输入需要修改学生的索引：\n");
    scanf("%s",&flag);
    int flag2=atoi(flag);
    if(flag<0||flag>=num)
    {
        printf("索引不存在\n");
        return;
    }
    printf("姓名：%s\n学号:%d\n性别:%s\n所属学院:%s\n入学日期:%s\n",Student[flag].name,Student[flag].number,Student[flag].sex,Student[flag].college,Student[flag].date);
    printf("请输入需要修改的内容\n1.姓名\n2.学号\n3.性别\n4.所属学院\n5.入学日期\n0.退出\n");
    while(1)
    {
        scanf("%d",&function);
        switch(function)
        {
            case 1:
                printf("请输入姓名：\n");
                scanf("%s",Student[flag].name);
                break;
            case 2:
                printf("请输入学号：\n");
                scanf("%d",&Student[flag].number);
                break;
            case 3:
                printf("请输入性别：\n");
                scanf("%s",Student[flag].sex);
                break;
            case 4:
                printf("请输入所属学院：\n");
                getchar();
                gets(Student[num].college);
                break;
            case 5:
                printf("请输入入学日期：\n");
                scanf("%s",Student[flag].date);
                break;
            case 0:
                printf("已退出修改\n");
                return;
                break;
            default:
                printf("无效操作\n");
                break;

        }
    }

}
*/

void function_inquire()//查询学生信息功能块
{
    char flag[10]={0};//索引
    printf("请输入需要查询学生的索引(0表示查询所有)：");
    scanf("%s",flag);
    getchar();
    if( !strcmp(flag,"0") )
    {
        char *sql_state="select * from students;";
        char *errmsg = NULL;
        if( SQLITE_OK != sqlite3_exec(students_sql,sql_state,callback2,NULL,&errmsg) )
        {
            printf("%s\n",errmsg);
        }
        sqlite3_free(errmsg);
    }
    else
    {
        char sql_state[512]={0};
        int flag2=atoi(flag);
        sprintf(sql_state,"select * from students where id=%d",flag2);
        char *errmsg = NULL;
        if( SQLITE_OK != sqlite3_exec(students_sql,sql_state,callback2,NULL,&errmsg) )
        {
            printf("%s\n",errmsg);
        }
        sqlite3_free(errmsg);
    }
    
   
}

static int callback1(void *data, int argc, char **argv, char **azColName)
{
    if(argc<=0)
    {
        printf("登录错误,密码不正确或账号不存在\n");
        return -1;
    }

    //flag等于1时为管理员系统
    if( !strcmp(argv[0],"1") )
    {
        int function;//命令
        while(1)
        {
            printf("Welcome to the Student Management system\n");
            printf("请选择你你想要使用的功能\n");
            printf("1.增添学生\n");
            printf("2.删除学生\n");
            printf("3.修改学生信息\n");
            printf("4.查询学生\n");
            printf("0.退出\n");

            scanf("%d",&function);
            getchar();
            switch(function)
            {
                case 1:
                    function_add();
                    break;
                case 2:
                    function_delete();
                    break;
                /*case 3:
                    function_recompose();
                    break;*/
                case 4:
                    function_inquire();
                    break;
                case 0:
                    printf("bye bye!");
                    return 0;
                default:
                    printf("错误行为，请重新输入\n");
                    break;

            }
        }
        return 0;
    }
    
    //flag等于0时为学生系统
    else if( !strcmp(argv[0],"0") )
    {
        printf("学号：%s\n",argv[3]);
        printf("姓名：%s\n",argv[4]);
        printf("年龄：%s\n",argv[5]);
        printf("性别：%s\n",argv[6]);
        printf("所属学院：%s\n",argv[7]);
        printf("所属班级：%s\n",argv[8]);
        printf("语文成绩：%s\n",argv[9]);
        printf("数学成绩：%s\n",argv[10]);
        printf("英语：%s\n",argv[11]);
        return 0;
    }

    //否则账户为封禁状态
    else
    {
        printf("账号已被封禁\n");
        return 0;
    }
    
    return 0;
}

static int callback2(void *data2,int argc,char **argv,char **azColName)
{
    if(argc<=0)
    {
        printf("未查询到学生\n");
        return -1;
    }
    for(int i=0;i<argc;i++)
    {
        printf("%s : %s\n",azColName[i],argv[i]);
    }
    return 0;
}

int main()//主函数
{
    char account[20]={0};
    char password[15]={0};
    /* 1.打开数据库 */
    if( SQLITE_OK != sqlite3_open(SQL_PATH,&students_sql) )
    {
        perror("Open SQL failed");
        return -1;
    } 
    /* 2.定义SQL语句，并置零 */
    char sql_state[512]={0};
   
    //登录界面
    
    printf("账号：");
    scanf("%s",account);
    printf("密码：");
    getchar();
    scanf("%s",password);

    /* 生成完整的SQL语句 */
    strcpy(sql_state,"select * from students where account='");
    strcat(sql_state,account);
    strcat(sql_state,"' and password='");
    strcat(sql_state,password);
    strcat(sql_state,"';");
    //exec操作
    char *errmsg = NULL;
    if( SQLITE_OK != sqlite3_exec(students_sql,sql_state,callback1,NULL,&errmsg) )
    {
        printf("%s\n",errmsg);
        sqlite3_free(errmsg);
    }
    sqlite3_close(students_sql);
    return 0;
}
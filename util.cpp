#include "util.h"
#include<stdio.h>
#include<stdlib.h>

void errif(bool condition,const char*errmsg)
{
    if(condition)
    {
        perror(errmsg);//打印错误的系统信息
        exit(EXIT_FAILURE);//安全退出
    }
}
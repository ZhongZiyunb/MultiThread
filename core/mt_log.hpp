/*
 * @Descripttion: 
 * @version: 
 * @Author: zzy
 * @Date: 2023-01-19 20:17:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-01-19 20:40:26
 */


#ifndef MT_LOG_HPP
#define MT_LOG_HPP

#include <stdarg.h>
#include <stdio.h>
#define INFO 0
#define WARN 1
#define ERROR 2

static void log_msg(int level,const char* format,...) {

    switch (level)
    {
    case INFO:
        va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
        va_start(args, format); //使args指向可变参数的第一个参数  
        vprintf(format, args);  //必须用vprintf等带V的  
        va_end(args);       //结束可变参数的获取

        break;
    case WARN:
        
        break;
    case ERROR:
        
        break;
    default:
        break;
    }
}



#define LOG_I(format, ...) printf("%s %s ",__FILE__,__LINE__); log_msg(INFO,format,...);
#define LOG_W(format, ...) printf("%s %s ",__FILE__,__LINE__); log_msg(WARN,format,...);
#define LOG_E(format, ...) printf("%s %s ",__FILE__,__LINE__); log_msg(ERROR,format,...);

#endif
/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-28 00:34:10
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-01-12 22:50:03
 */
// 这边是一个多线程框架
// 开发者可以基于该多线程框架的范式开发多线程程序
// 线程池维护
// 
// 日志管理库
// 线程管理库
// 传统视觉处理
// 神经网络处理
#include <stdio.h>
#include <iostream>
#include <functional>

void test_print(int a,int b,int c) {

    printf("a=%i,b=%i,c=%i\n",a,b,c);
}
class A {
private:
    int id;
public:
    A():id(0){
        printf("调用默认构造函数\n");
    }
    A(int a):id(a){
        printf("调用普通构造函数\n");
    }
    A(const A & obj) {
        id = obj.id;
        printf("调用拷贝构造函数\n");
    }
    A(const A && obj) {
        id = obj.id;
        printf("调用移动构造函数\n");
    }

    void test() {
        
    }
    A& operator=(const A & a) {
        this->id = a.id;
        std::cout << "调用赋值运算符" <<std::endl;
        return *this;
    }

    A operator+(const A & a) {
        A temp;
        temp.id = this->id + a.id;
        std::cout << "temp addr=" << &temp <<std::endl;
        return temp;
    }
    
    ~A() {
        std::cout << "调用析构函数" << std::endl;
    }
};


void forward(A & a) {
    printf("左值\n");
}

void forward(A && a) {
    printf("右值\n");
}


void common(A && a) {

    forward(std::forward<A>(a));
}


void use_A(A &a) {
    printf("use a\n");

}
// 先写一个线程池和线程间通信的框架
// 再写一个全局日志类
// 再部署一下神经网络
// 传统视觉也应用一下

int main() {

    auto func = std::bind(test_print,1,std::placeholders::_1,3);
    
    A a(10);
    use_A(a);
    A b = a;
    std::cout << "----" << std::endl;
    // auto func_2 = std::bind(common,std::placeholders::_1);
    // func(10);
    // func_2(std::move(a));
    A c;
    c = a + b; 
    std::cout << "c addr=" << &c <<std::endl;

    return 0;
}



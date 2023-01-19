/*
 * @Descripttion: 
 * @version: 
 * @Author: zzy
 * @Date: 2023-01-13 20:58:48
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-01-19 21:33:46
 */




#include "../mt_core.hpp"
#include <stdio.h>
#include <chrono>
#include "../mt_msg.hpp"
#include <iostream>

void func1() {
    for (int i=0;i<5;i++) {
        printf("func1()=%i\n",i);
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
}

void func2() {
    for (int i=0;i<5;i++) {
        printf("func2()=%i\n",i);
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
}


int func3() {
    
    printf("func2()=%i\n",3);
    return 3;
}

int func4() {
    
    printf("func2()=%i\n",4);
    return 4;
}


void msg_test1() {

    Publisher<int> pub("newbee");
    
    for (int i=0;i<100;i++) {
        pub.push(i);
        std::cout << "push data = " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}

void msg_test2() {

    Subscriber<int> sub("newbee");
    
    for (int i=0;i<100;i++) {
        auto data = sub.pop();
        std::cout << "pop data = " << data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
}

int tt_val = 2;
int*  tt() {
    return &tt_val;
}

int main() {

    MThreadPool tp;

    tp.emplace_task(msg_test1);
    tp.emplace_task(msg_test2);
    // int A;
    // std::shared_ptr<int> B = std::make_shared<int>(std::move(A));
    // int* C = tt();
    // std::cout << "B=" << &B << " A="<< &A << std::endl;
    // std::cout << "C=" << C << " tt_val="<< &tt_val << std::endl;
    
    // tp.emplace_task(func1);
    // tp.emplace_task(func2);
    // auto ret3 = tp.emplace_task(func3);
    // auto ret4 = tp.emplace_task(func4);

    // printf("ret3=%i,ret4=%i",ret3.get(),ret4.get());
    // ThreadPool tp(4);
    
    // tp.enqueue(func1);
    // tp.enqueue(func2);


    return 0;
}
/*
 * @Descripttion: 
 * @version: 
 * @Author: zzy
 * @Date: 2023-01-13 22:37:07
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-01-19 22:13:09
 */




#ifndef MT_MSG_HPP
#define MT_MSG_HPP



// 由字符串和类型唯一确定一组消息器
// 作为订阅器和接收器的接口

#include <map>
#include <string>
#include <queue>
#include <mutex>
#include <iostream>
// 消息队列
template<typename T>
class MessageQueue {
private:
    std::queue<T> _msg_que;

public:



};

// T是消息类型
template<typename T>
class MTMsgManager{
private:

    static std::map<std::string,std::queue<T>> _obj_map;
public:
    
    static std::mutex mtx;
    
    // 禁止使用构造函数
    MTMsgManager() = delete;

    static inline bool exist(std::string id) {
        return _obj_map.find(id) == _obj_map.end() ? false:true;
    }

    // 放入某个队列小小
    static bool push_queue_msg(std::string id, T&& msg) {
        if (exist(id)) {
            _obj_map[id].push(std::move(msg));
        }
    }

    // 返回某个消息队列数据
    static T pop_queue_msg(std::string id) {
        if (exist(id)) {
            std::cout << " here 4 que size= "<< _obj_map[id].size() << std::endl;
            if (!_obj_map[id].empty()) {
                auto msg = _obj_map[id].front();
                _obj_map[id].pop();
                return msg;
            }
            return T();
            // TODO: else 分支
        }
    }

    // 创建一个消息队列
    static void create_queue(std::string id) {
        if (!exist(id)) {
            _obj_map[id] = std::queue<T>();
            std::cout << "id=" << id << std::endl;
            std::cout << _obj_map[id].size() << std::endl;
            // return _obj_map[id];
        }
        else {
            // return _obj_map[id];
        }
    }

};

template<typename T>
std::mutex MTMsgManager<T>::mtx;

template<typename T>
std::map<std::string,std::queue<T>> MTMsgManager<T>::_obj_map;

// TODO 增加线程安全
// 订阅者
template<typename T>
class Subscriber {
private:
    
    std::string id;
    std::queue<T> que;
    
public:
    Subscriber() = delete;
    Subscriber(std::string _id):id(_id){
        bind(id);
    }

    bool bind(std::string id) {
        std::unique_lock<std::mutex> lck(MTMsgManager<T>::mtx);
        MTMsgManager<T>::create_queue(id);
        // printf("id=%c,addr=%i\n",id,&this->que);
        std::cout << "id=" << id << std::endl;
        
        // std::cout << "addr=" << this->que << std::endl;
        return true;
    }

    T pop() {
        std::unique_lock<std::mutex> lck(MTMsgManager<T>::mtx);
        
        // if (!this->que.empty()) {
            // auto data = this->que->front();
            // this->que->pop();
            // std::cout << " here 1 " << std::endl;
            auto data = MTMsgManager<T>::pop_queue_msg(id);
            // std::cout << " here 3 " << std::endl;

            // auto data = MTMsgManager<T>::pop_queue_msg(id);
            return data;
        // }
    }

};


// 发布者
template<typename T>
class Publisher {
private:

    std::string id;
    std::queue<T> que;
public:
    Publisher() = delete;
    Publisher(std::string _id):id(_id) {
        bind(_id);
    }
    
    bool bind(std::string id) {
        std::unique_lock<std::mutex> lck(MTMsgManager<T>::mtx);
        MTMsgManager<T>::create_queue(id);
        return true;
    }

    bool push(T data) {
        std::unique_lock<std::mutex> lck(MTMsgManager<T>::mtx);
        std::cout << " here 2 " << std::endl;
        // this->que->push(std::move(data));
        MTMsgManager<T>::push_queue_msg(id,std::move(data));
        return true;
    }
};




#endif
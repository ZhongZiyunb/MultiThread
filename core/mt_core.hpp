/*
 * @Descripttion: 
 * @version: 
 * @Author: zzy
 * @Date: 2022-06-28 00:35:52
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-01-15 20:14:13
 */


#ifndef MT_CORE_HPP
#define MT_CORE_HPP

#include <future>
#include <mutex>
#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <condition_variable>

#define DEFAULT_THREAD_NUM 4 // 默认的线程个数

class MThreadPool {
private:
    
    int trd_num;    // 开启的线程数量
    std::mutex mtx; // 锁，用于线程池操作
    std::condition_variable cv; // 条件变量，用于线程池操作同步
    std::queue<std::function<void()>> tasks; // 任务队列，存放函数对象
    std::vector<std::thread> workers;
    bool stop_flag;
    // 初始化线程池
    void init_thread_pool() {
        for (int i=0;i<trd_num;i++) {
            std::thread trd([this](){
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lck(mtx);
                        while (tasks.empty() && !stop_flag) {
                            cv.wait(lck);
                        }
                        // cv.wait(lck,[this](){
                        //     return stop_flag || !tasks.empty();
                        // });

                        if (stop_flag) return; // 如果结束标志则退出
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                        task();
                }

            });
            workers.emplace_back(std::move(trd));
            // thread 和 packege_task 的拷贝构造函数是禁止使用的！由此体现出移动构造函数的作用
            
        }
    }
    
public:

    MThreadPool():trd_num(DEFAULT_THREAD_NUM),stop_flag(false) {
        init_thread_pool();
    }
    MThreadPool(int num):trd_num(num),stop_flag(false) {
        init_thread_pool();
    }

    // 放入函数对象
    // 模板情况下最好放&& 因为可以适应于任何类型
    template<typename T, typename ...Args>
    auto emplace_task(T&& Fn, Args&& ... args) -> std::future<decltype(Fn(args ...))> {

        using retType = decltype(Fn(args ...));
        // 由于package_task对象禁用拷贝构造，因此通过指针将函数封装成 void() 类型
        auto task_func = std::make_shared<std::packaged_task<retType()>>(std::bind(std::forward<T>(Fn),std::forward<Args>(args)...));
        std::future<retType> fut = task_func->get_future();
    
        std::function<void()> common_task([task_func](){(*task_func)();}); 
        {
            std::unique_lock<std::mutex> lck(mtx);
            // tasks.emplace([task_func](){ (*task_func)(); });
            tasks.emplace(std::move(common_task));
        }

        cv.notify_all();
        return fut;
    }; 
        
    void stop_thread_pool() {
        std::unique_lock<std::mutex> lck(mtx);
        stop_flag = true;
    }
    
    ~MThreadPool() {
        
        printf("线程销毁\n");
        {
            std::unique_lock<std::mutex> lck(mtx);
            stop_flag = true;
        }
        // 唤醒所有的线程 每个拿完锁之后就会退出
        cv.notify_all();

        // 在被销毁的时候阻塞 保证任务执行完之前 线程池还在
        for(std::thread &w:workers) {
            w.join();
        }
    }

};

#endif
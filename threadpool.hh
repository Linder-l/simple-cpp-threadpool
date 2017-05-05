//
// Created by linder on 4/19/17.
//

#ifndef THREADPOOL_HH
# define THREADPOOL_HH

#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class work {
public:
  work(int);
  ~work();
  void  working();
  static unsigned int count;
private:
  int workID;
  static pthread_mutex_t workLock;
};

class threadpool {
public:
  threadpool(int);
  ~threadpool();
  void startThreads(pthread_t *, int);
  void getWork(work *);
  void thread_pool(int, const string &);
private:
  bool loadWork(work *&);
  static void *threadLaunch(void *);
  static pthread_mutex_t        workQueue_lock;
  static pthread_cond_t         workQueue_cond;
  queue<work*>                  workQueue;
  int                           numOfThreads;
};

#endif // !THREADPOOL_HH

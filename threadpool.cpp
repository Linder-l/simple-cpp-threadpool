//
// Created by linder on 4/19/17.
//

#include "threadpool.hh"

using namespace std;

unsigned int work::count = 0;

pthread_mutex_t work::workLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t threadpool::workQueue_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t threadpool::workQueue_lock = PTHREAD_MUTEX_INITIALIZER;

work::work(int id)
{
    workID = id;
}

work::~work()
{

}

void	work::working()
{
    pthread_mutex_lock(&workLock);
    /**
    ***  Do some job
    **/
    count++;
    pthread_mutex_unlock(&workLock);
    usleep(100);
}

threadpool::threadpool(int nbThreads)
{
    numOfThreads = nbThreads;
}

threadpool::~threadpool()
{
    while(!workQueue.empty())
        workQueue.pop();
}

void	threadpool::startThreads(pthread_t *threads, int nbThreads)
{
    int	i = 0;

    while (i < nbThreads) {
        pthread_create(&threads[i], NULL, &threadpool::threadLaunch, (void *)this);
        i++;
    }
}

void threadpool::getWork(work* work)
{
    pthread_mutex_lock(&workQueue_lock);
    workQueue.push(work);
    pthread_mutex_unlock(&workQueue_lock);
    pthread_cond_signal(&workQueue_cond);
}

bool		threadpool::loadWork(work*& task)
{
    pthread_mutex_lock(&workQueue_lock);
    while(workQueue.empty())
      pthread_cond_wait(&workQueue_cond, &workQueue_lock);
    task = workQueue.front();
    workQueue.pop();
    pthread_mutex_unlock(&workQueue_lock);
    return (true);
}

void		*threadpool::threadLaunch(void *param)
{
    work	*task = NULL;
    threadpool	*my_thread = (threadpool *)param;

    while(my_thread->loadWork(task)) {
      if(task)
	task->working();
      delete task;
      task = NULL;
    }
    return (NULL);
}

void			thread_pool(int nbThreads, int nbWorks)
{
    unsigned int	i = 0;
    threadpool		*myPool = new threadpool(nbThreads);
    pthread_t		threads[nbThreads];
    
    myPool->startThreads(threads, nbThreads);
    while (i < nbWorks) {
      work *newWork = new work(i);
      myPool->getWork(newWork);
      i++;
    }
    while (work::count < nbWorks);
}

int	main(void)
{
  thread_pool(5, 10);
  /**
  *** add some parrametre depending 
  *** of what you want to do 
   **/
}

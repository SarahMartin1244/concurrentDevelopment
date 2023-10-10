#include "Barrier.h"
#include <iostream>

/*! Student Name: Sarah Martin 
    Description: This program demonstrates the use of a Barrier using Semaphores
    Date: 10/10/2023

*/

/*! \class Barrier
    \brief A Barrier Implementation

   Uses C++11 features such as mutex and condition variables to implement a Barrier class using Semaphores

*/

/*! Barrier constructor*/
Barrier::Barrier()
{
  count = 0;
  threadNum = 0;
  condition = false;
  theMutex = std::make_shared<Semaphore>(1);
  innerLock = std::make_shared<Semaphore>(0);
  outerLock = std::make_shared<Semaphore>(1);
}

/*! Barrier with parameter constructor*/
Barrier::Barrier(int countOfThreads)
{
  count = countOfThreads;
  threadNum = 0;
  condition = false;
  theMutex = std::make_shared<Semaphore>(1);  /**< mutex to protect threadNum */
  innerLock = std::make_shared<Semaphore>(0); /**< inner lock to block threads until all threads arrive */
  outerLock = std::make_shared<Semaphore>(1); /**< outer lock to block threads until all threads finish */
}

/*! Barrier deconstructor*/
Barrier::~Barrier()
{
}

/*! sets count value*/
void Barrier::setCount(int x)
{
  this->count = x;
}

/*! returns count value*/
int Barrier::getCount()
{
  return this->count;
}

/*! waits for all the threads before starting the second half of code*/
void Barrier::waitForAll()
{
  theMutex->Wait();
  threadNum++;




  
  if (threadNum == count)  /**< if the last thread to arrive, release the outer lock */
  {
    outerLock->Wait(); /**< access the outer lock (blocks other threads) */
    innerLock->Signal();  /**< release the inner lock (allows other threads to access) */
  }
  theMutex->Signal(); /**< release the mutex (allows other threads to access threadNum) */

  innerLock->Wait();
  innerLock->Signal();

  theMutex->Wait();
  threadNum--;

  if (threadNum == 0)  /**< if the last thread to finish, release the outer lock */
  {
    innerLock->Wait();
    outerLock->Signal();
  }
  theMutex->Signal(); 

  outerLock->Wait(); /**< access the outer lock (blocks other threads) */
  outerLock->Signal(); /**< release the outer lock */
}

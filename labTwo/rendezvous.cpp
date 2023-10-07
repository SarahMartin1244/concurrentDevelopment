#include "Semaphore.h"
#include <thread>
#include <vector>
#include <iostream>

/**
 * @file rendezvous.cpp
 * @brief This program demonstrates a barrier synchronization mechanism implemented using threads and semaphores
 *
 * 
 * @StudentName: Sarah Martin
 * @studentID: C00257967
 * @date October 7, 2023
 */

/*! displays the first function in the barrier being executed */
void task(std::shared_ptr<int> arrivedTasks, std::shared_ptr<Semaphore> mutexSem, std::shared_ptr<Semaphore> barrierSem, int threadCount){

  std::cout << "first " << std::endl;
  //put barrier code here
  mutexSem->Wait(); // ensure only one thread can update the shared 'arrivedTasks' variable at a time
    *arrivedTasks = *arrivedTasks + 1; // Increment the number of arrived tasks

   // If all threads have arrived at the barrier, signal the barrier semaphore.
  if (*arrivedTasks == threadCount) {
    barrierSem->Signal(); // Signal after all threads have arrived at the barrier
  }
  // Release the mutex semaphore to allow other threads to update 'arrivedTasks'.
  mutexSem->Signal();
  barrierSem->Wait();
  barrierSem->Signal();
  
  std::cout << "second " << std::endl;
}




int main(void){
  std::shared_ptr<int> arrivedTasks;
  std::shared_ptr<Semaphore> mutexSem;
  std::shared_ptr<Semaphore> barrierSem;
  int threadCount = 5;
  mutexSem=std::make_shared<Semaphore>(1);
  barrierSem=std::make_shared<Semaphore>(0);
  arrivedTasks = std::make_shared<int>(0);
  /*!< An array of threads*/
  std::vector<std::thread> threadArray(threadCount);
  /*!< Pointer to barrier object*/

  for(int i=0; i < threadArray.size(); i++){
    threadArray[i]=std::thread(task,arrivedTasks,mutexSem,barrierSem,threadCount);
  }

  for(int i = 0; i < threadArray.size(); i++){
    threadArray[i].join();
  }
  
  return 0;
}


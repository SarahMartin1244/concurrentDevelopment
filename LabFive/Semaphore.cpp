#include "Semaphore.h"

/*! Student Name: Sarah Martin
    Description: Contains an implementation of a Semaphore class using C++11 features such as mutex and condition variables
    Date: 5/11/2023
*/

/*! \class Semaphore
    \brief A Semaphore Implementation

    This class uses C++11 features such as mutex and condition variables to implement a semaphore. Semaphores are synchronization primitives used to control access to shared resources in a multithreaded environment.

    Usage:
    - Create an instance of Semaphore with an initial count.
    - Use Wait to block a thread until the semaphore count becomes positive.
    - Use Signal to increment the semaphore count, allowing other threads to proceed.

    Example:
    \code
    Semaphore semaphore(1); // Initialize semaphore with count 1

    // Thread 1
    semaphore.Wait();
    // Perform critical section operations
    semaphore.Signal();

    // Thread 2
    semaphore.Wait();
    // Perform critical section operations
    semaphore.Signal();
    \endcode
*/

// Wait for the semaphore with a specified timeout
void Semaphore::Wait()
{
    std::unique_lock<std::mutex> lock(m_mutex); // Lock the mutex
    m_condition.wait(lock, [&]()->bool { return m_uiCount > 0; }); 
    --m_uiCount; 
}

// Wait for the semaphore with a specified timeout
template <typename R, typename P>
bool Semaphore::Wait(const std::chrono::duration<R, P>& crRelTime)
{
    std::unique_lock<std::mutex> lock(m_mutex); // Lock the mutex
    if (!m_condition.wait_for(lock, crRelTime, [&]()->bool { return m_uiCount > 0; })) {
        // If the timeout is reached and m_uiCount is still not greater than 0, return false
        return false;
    }
    --m_uiCount; 
    return true; 
}

// Signal the semaphore, incrementing the count and allowing waiting threads to proceed
void Semaphore::Signal()
{
    std::unique_lock<std::mutex> lock(m_mutex); 
    ++m_uiCount; 
    m_condition.notify_one(); 
}


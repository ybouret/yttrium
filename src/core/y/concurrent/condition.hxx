
namespace Yttrium
{
	namespace Concurrent
	{

		namespace Nucleus
		{

#if defined(Y_BSD)

			//__________________________________________________________________
			//
			//
			//
			//! pthread_condition wrapper
			//
			//
			//__________________________________________________________________
			class Condition
			{
			public:
				inline Condition() : cond()
				{
					Y_STATIC_ZVAR(cond);
					const int res = pthread_cond_init(&cond, NULL);
					if (res != 0) throw Libc::Exception(res, "pthread_cond_init");
				}

				inline ~Condition() noexcept
				{
					const int res = pthread_cond_destroy(&cond);
					if (res != 0)  Libc::CriticalError(res, "pthread_cond_destroy");
				}

				inline void wait(Mutex *m) noexcept
				{
					assert(0 != m);
					const int res = pthread_cond_wait(&cond, **m);
					if (res != 0) Libc::CriticalError(res, "pthread_cond_wait");
				}

				inline void signal() noexcept
				{
					const int res = pthread_cond_signal(&cond);
					if (res != 0) Libc::CriticalError(res, "pthread_cond_signal");
				}

				inline void broadcast() noexcept
				{
					const int res = pthread_cond_broadcast(&cond);
					if (res != 0) Libc::CriticalError(res, "pthread_cond_broadcast");
				}


				pthread_cond_t cond;

			private:
				Y_DISABLE_COPY_AND_ASSIGN(Condition);
			};
#endif

#if defined(Y_WIN)
			//==================================================================
			//
			//
			//! Legacy condition on windows
			//
			//
			//==================================================================
			class Condition
			{
			public:
				inline Condition() :
					cv_attr(),
					cv_lock(cv_attr),
					cv_waiting(0),
					cv_signals(0),
					cv_wait_sem(),
					cv_done_sem()
				{
				}

				inline ~Condition() noexcept
				{}

				inline void wait(Mutex *m) noexcept
				{
					/* Obtain the protection mutex, and increment the number of waiters.
					 This allows the signal mechanism to only perform a signal if there
					 are waiting threads.
					 */
					cv_lock.lock();
					++cv_waiting;
					cv_lock.unlock();

					/* Unlock the mutex, as is required by condition variable semantics */
					m->unlock();
					//mutex_api::unlock(m);


					/* Wait for a signal */
					cv_wait_sem.wait();


					/* Let the signaler know we have completed the wait, otherwise
					 the signaler can race ahead and get the condition semaphore
					 if we are stopped between the mutex unlock and semaphore wait,
					 giving a deadlock.  See the following URL for details:
					 http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue40.html
					 */

					cv_lock.lock();
					if (cv_signals > 0)
					{
						/* We always notify the signal thread that we are done */
						cv_done_sem.post();
						--cv_signals;
					}
					--cv_waiting;
					cv_lock.unlock();

					/* Lock the mutex, as is required by condition variable semantics */
					m->lock();
					//mutex_api::lock(m);
				}

				void signal() noexcept
				{
					cv_lock.lock();
					if (cv_waiting > cv_signals)
					{
						++cv_signals;
						cv_wait_sem.post();
						cv_lock.unlock();
						cv_done_sem.wait();
					}
					else
						cv_lock.unlock();
				}

				void broadcast() noexcept
				{
					/* If there are waiting threads not already signalled, then
					 signal the condition and wait for the thread to respond.
					 */
					cv_lock.lock();
					if (cv_waiting > cv_signals)
					{
						const int num_waiting = cv_waiting - cv_signals;
						cv_signals = cv_waiting;
						for (int i = 0; i < num_waiting; ++i)
						{
							cv_wait_sem.post();
						}
						/* Now all released threads are blocked here, waiting for us.
						 Collect them all.
						 */
						cv_lock.unlock();
						for (int i = 0; i < num_waiting; ++i)
						{
							cv_done_sem.wait();
						}
					}
					else
					{
						cv_lock.unlock();
					}
				}


			private:
				Y_DISABLE_COPY_AND_ASSIGN(Condition);
				MutexAttribute   cv_attr;      /*!< dummy mutex initializer      */
				Mutex            cv_lock;      /*!< condition lock               */
				int              cv_waiting;   /*!< waiting count                */
				int              cv_signals;   /*!< signals count                */
				Win32::Semaphore cv_wait_sem;  /*!< Win32 semaphore when waiting */
				Win32::Semaphore cv_done_sem;  /*!< Win32 semaphore when done    */

			};

#endif

		}

	}

}

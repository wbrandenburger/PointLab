/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017  Wolfgang Brandenburger. All rights reserved.
*
* THE BSD LICENSE
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#ifndef UTILS_THREADPOOL_H_
#define UTILS_THREADPOOL_H_

#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>

namespace utils
{
	class Threadpool
	{
	private:
	
		boost::asio::io_service io_service;
		boost::asio::io_service::work work;
		boost::thread_group threads;
		std::size_t available;
		boost::mutex mutex;

	public:

		/**
			Constructor
			
			@param[in] pool_ Number of active threads
		*/
		Threadpool(std::size_t pool_) : work(io_service), available(pool_)
		{
			for (std::size_t i = 0; i < pool_; ++i)
			{
				threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
			}
		}

		/**
			Destructor
		*/
		~Threadpool()
		{
		}

		/**
			Terminates io_service and threads
		*/
		void shutdown()
		{
			// Force all threads to return from io_service::run().
			io_service.stop();

			// Suppress all exceptions.
			try
			{
				threads.join_all();
			}
			catch (const std::exception&) {}
		}

		/**
			Start a thread with a specific task
			
			@param[in] task_ Function which will be invoked
			@return True if the function could be invoked			
		*/		
		template <typename Task> bool runTask(Task task_)
		{
			boost::unique_lock< boost::mutex > lock(mutex);

			// If no threads are available, then return.
			if (0 == available) return false;

			// Decrement count, indicating thread is no longer available.
			--available;

			// Post a wrapped task into the queue.
			io_service.post(boost::bind(&Threadpool::wrapTask, this, boost::function<void()>(task_)));

			return true;
		}
		
		/**
			Waits until all threads has been finished

			@return Return true if all tasks are completed
		*/
		bool waitTasks()
		{
			while (available != threads.size());
			
			return true;
		}
		
	private:
		
		/**
			Invoke the function task_
			
			@param[in] task_ Function which will be invoked
		*/	
		void wrapTask(boost::function<void()> task_)
		{
			// Run the user supplied task.
			try
			{
				task_();
			}
			// Suppress all exceptions.
			catch (const std::exception&) {}

			// Task has finished, so increment count of available threads.
			boost::unique_lock< boost::mutex > lock(mutex);
			++available;
		}
	};
}

#endif /* UTILS_THREADPOOL_H_ */
#include "Stdafx.h"
#include "Utility/Threading/Thread.h"

#include "Utility/Debug/Assert.h"

using namespace Phoenix;

FSafeThread::FSafeThread(FThread&& Thread)
	: Thread(std::move(Thread))
{
}

FSafeThread::~FSafeThread()
{
	F_Assert(!Thread.joinable(), "Thread should have been joined before destruction");
	if (Thread.joinable())
	{
		Join();
	}
}

FThread& FSafeThread::Get()
{
	return Thread;
}

void Phoenix::FSafeThread::Join()
{
	F_Assert(Thread.joinable(), "Thread is not joinable");
	Thread.join();
}

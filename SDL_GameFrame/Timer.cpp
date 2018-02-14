#include "Timer.h"

Timer* Timer::mTimer = 0;

Timer::Timer() 
{
}

Timer::~Timer()
{
}

void Timer::init()
{
	mSecondsPerCount = 0.0;
	mDeltaTime = -1.0;
	mBaseTime = 0;
	mPauseTime = 0;
	mPrevTime = 0;
	mCurrentTime = 0;
	mStopped = false;

	mFramesPerSec = mCount = 0;
	mOneSecCounter = 0.0f;

	INT64 countspersec;

	QueryPerformanceFrequency((LARGE_INTEGER*)&countspersec);

	mSecondsPerCount = 1.0 / (double)countspersec;

	Reset();
	Start();
}

void Timer::Tick()
{
	if (mStopped){ mDeltaTime = 0.0; return; }

	//Get the time this frame.

	INT64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrentTime = currTime;

	//Time difference between this frame and the previous.
	mDeltaTime = (mCurrentTime - mPrevTime) * mSecondsPerCount;
	//Prepare for next frame.
	mPrevTime = mCurrentTime;

	//Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
	//processor goes into a power save mode or we get shuffled to another
	//processor, then mDeltaTime can be negative.
	if(mDeltaTime < 0.0) { mDeltaTime = 0.0; }

	// fps logic
	// --- its not working it just returns 0.0 ---
	// --- need to fix --- later job ---
	//mFramesPerSec++;
	mCount++;

	if ((mOneSecCounter += (float)mDeltaTime) >= 1.0f)
	{
		mFramesPerSec = mCount;
		mCount = 0;
		mOneSecCounter = 0.0;
	}
}

float Timer::DeltaTime() const
{
	return (float)mDeltaTime;
}

int Timer::getFPS() const
{
	return mFramesPerSec;
}

void Timer::Reset()
{
	INT64 currentTime;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	mBaseTime = currentTime;
	mPrevTime = currentTime;
	mStopTime = 0;
	mStopped = false;
}

void Timer::Start()
{
	INT64 startTimer;

	QueryPerformanceCounter((LARGE_INTEGER*)&startTimer);
	//Accumulate the time elapsed between stop and start
	// if we are resuming timer from a stopped state

	if (mStopped)
	{
		// the accumulate the pause time
		mPauseTime += (startTimer - mStopTime);

		// since we are starting the timer back up
		// the current previous time is not valid 
		// as it occurred while paused
		// so reset it to the current time
		mPrevTime = startTimer;

		// no longer stopped
		mStopTime = 0;
		mStopped = false;
	}
}

float Timer::TotalTime() const
{
	// If we are stopped, do not count the time that has passed 
	// since we stopped. Moreover, if we previously already had 
	// a pause, the distance mStopTime - mBaseTime includes paused 
	// time,which we do not want to count. To correct this, we can 
	// subtract the paused time from mStopTime: 
	// 
	// previous paused time 
	// |<----------->| 
	// ---*------------*-------------*-------*-----------*------> time 
	// mBaseTime mStopTime mCurrTime

	// The distance mCurrTime - mBaseTime includes paused time, 
	// which we do not want to count. To correct this, we can subtract 
	// the paused time from mCurrTime: 
	// 
	// (mCurrTime - mPausedTime) - mBaseTime 
	// 
	// |<--paused time-->| 
	// ----*---------------*-----------------*------------*------> time 
	// mBaseTime mStopTime startTime mCurrTime 

	if (mStopped)
		return(float)(((mStopTime - mPauseTime) - mBaseTime) * mSecondsPerCount);
	else 
		return (float)(((mCurrentTime - mPauseTime) - mBaseTime) * mSecondsPerCount);
}
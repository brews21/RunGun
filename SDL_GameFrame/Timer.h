//////////////////////////////////////////////////////////
//	From this site http://www.rastertek.com/dx11tut15.html
//////////////////////////////////////////////////////////

#ifndef __Timer__
#define __Timer__

#include <windows.h>

class Timer
{
public:
	Timer();
	~Timer();

	static Timer* Singleton()
	{
		if (!mTimer)
		{
			mTimer = new Timer();
		}

		return mTimer;
	}

	void init();

	float GameTime() const; // in seconds
	float DeltaTime() const; // in seconds
	float TotalTime() const; // in seconds

	void Reset(); // call before message loop
	void Start(); // call when unpaused
	void Stop(); // call when paused
	void Tick(); // call very frame

	// fps stuff
	int getFPS() const;

private:
	// singleton
	static Timer* mTimer;

	double mSecondsPerCount, mDeltaTime;
	INT64 mBaseTime, mPauseTime, mStopTime, mPrevTime, mCurrentTime;

	bool mStopped;

	// fps stuff
	int mFramesPerSec, mCount;
	float mOneSecCounter;
};

#endif //__Timer__


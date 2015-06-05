#ifndef _updatescheduler_h_
#define _updatescheduler_h_

#include "threads.h"

namespace winsparkle
{
	class UpdateScheduler : public Thread
	{
	public:
		static void Init();
		static void DeInit();

		static void SetSchedulerFrequency(int frequency);
		static int GetSchedulerFrequency();

	protected:
		virtual void Run() override;
		virtual bool IsJoinable() const override { return false; }

		void Stop();

	private:
		UpdateScheduler(int intervalSeconds);

		int _intervalSeconds;
		long _delayCounts;
		bool _alive;

		static UpdateScheduler* _instance;
	};
}

#endif

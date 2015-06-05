#include "UpdateScheduler.h"
#include "updatechecker.h"
#include <winsparkle.h>
#include <ctime>
#include "settings.h"
#include <iostream>

namespace winsparkle
{
	UpdateScheduler* UpdateScheduler::_instance = nullptr;

	void UpdateScheduler::Init()
	{
		_instance = new UpdateScheduler(GetSchedulerFrequency());
		_instance->Start();
	}

	void UpdateScheduler::DeInit()
	{
		if (_instance != nullptr)
			_instance->Stop();
	}


	void UpdateScheduler::SetSchedulerFrequency(int frequencySeconds)
	{
		Settings::WriteConfigValue("SchedulerFrequency", frequencySeconds);
		if (_instance != nullptr)
		{
			_instance->_intervalSeconds = frequencySeconds;
			_instance->_delayCounts = frequencySeconds * 10000;
		}
	}

	int UpdateScheduler::GetSchedulerFrequency()
	{
		int frequencySeconds;
		Settings::ReadConfigValue("SchedulerFrequency", frequencySeconds, 600);
		return frequencySeconds;
	}

	UpdateScheduler::UpdateScheduler(int intervalSeconds) : Thread("WinSparkle Update Check Scheduler"),
		_intervalSeconds(intervalSeconds),
		_alive(true)
	{
	}

	void UpdateScheduler::Stop()
	{
		_alive = false;
	}

	void UpdateScheduler::Run()
	{
		bool checkUpdates;

		SignalReady();

		_delayCounts = _intervalSeconds * 10;

		try
		{
			while (_alive)
			{
				// Responsive equivalent to Sleep(_intervalSeconds * 1000);
				// Allows for the thread to exit more responsively.
				for (long i = 0; i < _delayCounts; i++)
				{
					if (!_alive)
						break;
					Sleep(100);
				}

				if (Settings::ReadConfigValue("CheckForUpdates", checkUpdates))
				{
					if (checkUpdates)
					{
						time_t lastCheck = 0;
						Settings::ReadConfigValue("LastCheckTime", lastCheck);
						const time_t currentTime = time(NULL);

						// Only check for updates in reasonable intervals:
						const int interval = win_sparkle_get_update_check_interval();
						if (currentTime - lastCheck >= interval)
						{
							// Run the check in background. Only show UI if updates
							// are available.
							UpdateChecker *check = new UpdateChecker();
							check->Start();

							// wait 10 minutes before continuing, but check to see if
							// we're shutting down every 100 ms.
							for (int i = 0; i < 6000; i++)
							{
								if (!_alive)
									break;

								Sleep(100); 
							}
						}
					}
				}
			}
		}
		catch (...)
		{

		}
	}
}
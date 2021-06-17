#include "MsvcAppender.h"
#include "humblelogging/formatter.h"

#ifdef _WIN32
#include "Windows.h"
#endif

using namespace humble::logging;

MsvcAppender::MsvcAppender()
	: Appender()
{
}

MsvcAppender::~MsvcAppender() = default;

void MsvcAppender::log(const humble::logging::LogEvent& logEvent)
{
#ifdef _WIN32
	MutexLockGuard lock(_mutex);
	if (!_formatter)
		return;
	
	const auto msg = _formatter->format(logEvent);
	OutputDebugStringA(msg.c_str());
#endif
}

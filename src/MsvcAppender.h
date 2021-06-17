#pragma once
#include "humblelogging/appender.h"

class MsvcAppender : public humble::logging::Appender
{
public:
	MsvcAppender();
	~MsvcAppender() override;

	void log(const humble::logging::LogEvent& logEvent) override;
};

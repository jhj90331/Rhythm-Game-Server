#pragma once
#include "stdafx.h"
#include <fstream>
#include "Singleton.h"
#include "Type.h"

#define SLog(arg, ...)				SystemLog::getInstance().log(arg, __VA_ARGS__);
#define SErrLog(arg, ...)			SystemLog::getInstance().log(arg, __VA_ARGS__); ::ExitProcess(0);

//-----------------------------------------------------------------//
class LogBase
{
public:
    LogBase(){}
    virtual ~LogBase(){}
    virtual void initialize() {}
    virtual void unInitialize() {}
    virtual void log(const WCHAR *logStr) = 0;
};

//-----------------------------------------------------------------//
class LogPrintf : public LogBase
{
public:
    LogPrintf();
    void log(const WCHAR *logStr);
};

//-----------------------------------------------------------------//
class LogFile : public LogBase
{
    std::wfstream   fs_;
    wstr_t			fileName_;
public:
    LogFile(xml_t *config);
    virtual ~LogFile();

    void initialize(){}
    void initialize(const WCHAR *logFileName);
    void log(const WCHAR *logStr);
};
//-----------------------------------------------------------------//
// �α� ���� ��ü
//-----------------------------------------------------------------//
class LogWriter
{
private:
    LogBase			*base_;
    wstr_t			prefix_;
public:
    LogWriter();
    virtual ~LogWriter();

    void setLogger(LogBase *base, const WCHAR *logPrefix);
    LogBase *logger();

    void log(const WCHAR *fmt, ...);
	void log(const WCHAR *fmt, va_list args);
};
typedef LogWriter* LogWriterPtr;

//-----------------------------------------------------------------//
// ���� �ý��� �α�
//-----------------------------------------------------------------//
class SystemLog : public Singleton<SystemLog>
{
private:
    LogWriter   logWrite_;
public:
	SystemLog();
    virtual ~SystemLog();
	
	void initialize(xml_t *config);
	void log(const WCHAR *fmt, ...);
};
#pragma once
//#include <windows.h>
class TIMECOUNT
{
public:
	TIMECOUNT(void);
	~TIMECOUNT(void);//Îö¹¹º¯Êý

private:
	LARGE_INTEGER STARTTIME;

	LARGE_INTEGER ENDTIME;

	LARGE_INTEGER CPU_SPEED;

public:
	double TIME;

public:
	void START();
	void END();
};



TIMECOUNT::TIMECOUNT(void)
{
	QueryPerformanceFrequency(&CPU_SPEED);
}

TIMECOUNT::~TIMECOUNT(void)
{
}

void TIMECOUNT::START()
{
	QueryPerformanceCounter(&STARTTIME);
}

void TIMECOUNT::END()
{
	QueryPerformanceCounter(&ENDTIME);

	TIME = ((double)ENDTIME.QuadPart - (double)STARTTIME.QuadPart) / (double)CPU_SPEED.QuadPart;

}
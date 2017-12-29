#ifndef _PINE_DATE_TIME_H_
#define _PINE_DATE_TIME_H_

// Class store time pass -> day, hour, minute, second, milisecond
#define DATE_START_COUNT_YEAR		2012
#define DATE_START_COUNT_MONTH		2
#define DATE_START_COUNT_DAY		3
class Pine_Date_Time
{
	int _milisecond;
	BYTE _second;
	BYTE _minute;
	BYTE _hour;
	INT32 _day;
public:
	Pine_Date_Time(){
		setNullDay();		
	};	

	Pine_Date_Time(INT32 day, BYTE hour, BYTE minute, BYTE second, int milisecond)
	{
		_day = day;
		_hour = hour;
		_minute = minute;
		_second = second;
		_milisecond = milisecond;
	};
	
	void setDateByDate(Pine_Date_Time value)
	{
		_day = value.getDay();
		_hour = value.getHour();
		_minute = value.getMinute();
		_second = value.getSecond();
		_milisecond = value.getMilisecond();
	};

	void setDate(INT32 day, BYTE hour, BYTE minute, BYTE second, int milisecond = 0)
	{
		_day = day;
		_hour = hour;
		_minute = minute;
		_second = second;
		_milisecond = milisecond;
	};

	void setDateByMinute(INT_TIME _total_minute);
    void setDateBYMilisecond(INT_TIME _milisecond);
	void setDateBYSecond(INT_TIME second);
	void setDateByLocalDate();
    void setDateByRealDate(int year, int month, int day, int hour, int minute, int second);

    void addDate(Pine_Date_Time date);
	//if time < 0 -> return false
    bool minusDate(Pine_Date_Time date);
    void setDateByCountDownDayFromDay(Pine_Date_Time _day);
    void printDate();

	INT_TIME GetCurrentTotalMiliSec();
	INT_TIME GetTotalMiliSec();
	INT_TIME GetTotalSec();
	INT_TIME GetTotalMin();
	INT_TIME GetTotalHour();

	int getMilisecond()
	{
		return _milisecond;
	};

	BYTE getSecond()
	{
		return _second;
	};

	BYTE getMinute()
	{
		return _minute;
	};

	BYTE getHour()
	{
		return _hour;
	};

	INT32 getDay()
	{
		return _day;
	};

    void setDay(INT32 day)
    {
        _day = day;
    };
    
	void UpdateTime();
    bool UpdateCountDown();
    
	bool isMinuteShow()
	{
		return (_day == 0 && _hour == 0);
	};

	bool isHourShow()
	{
		return (_day == 0 && _hour > 0);
	};

	bool isDayShow()
	{
		return (_day > 0);
	};
    
    bool isNullDay()
    {
        if (_hour == 255 && _minute == 255 && _second == 255)
            return true;
        return false;               
    };
    
    void setNullDay()
    {
        _day = 0;
        _milisecond = 0;
        _hour = 255;
        _minute = 255;
        _second = 255;
    };

	//store use 9 byte -> 4 + 1 + 1 + 1 + 2  
	void SetPineDateTimeAt(char* buffer,int offset);
	void GetPineDateTimeAt(char* buffer,int offset);

	//util
	
	static const char* GetStringTime(INT_TIME milisecond);
	static const char* GetStringTimeSecond(INT_TIME milisecond, int mode);
	Pine_Date_Time operator = (Pine_Date_Time value);
	bool operator > (Pine_Date_Time value);
	bool operator < (Pine_Date_Time value);
	bool operator == (Pine_Date_Time value);
	bool operator >= (Pine_Date_Time value);
	bool operator <= (Pine_Date_Time value);
	Pine_Date_Time operator + (Pine_Date_Time value);
	Pine_Date_Time operator += (Pine_Date_Time value);
	Pine_Date_Time operator - (Pine_Date_Time value);
	Pine_Date_Time operator -= (Pine_Date_Time value);
};

#endif
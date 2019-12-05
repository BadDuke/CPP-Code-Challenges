#ifndef WEATHER_DATA_PARSER
#define WEATHER_DATA_PARSER

#include <vector>
#define MONTHS_IN_YEAR 12
#define MAX_DAYS_IN_MONTH 31

class MonthData; // forward declarations
class DayData;

class YearData {

public:
	YearData();
	void initialize(int year);
	MonthData * getMonth(int index);
	bool isInitialized() const;

private:
	int m_year;
	std::vector <MonthData> m_months;
	bool m_initialized;
};

class MonthData {

public:
	MonthData();
	void initialize(int month);

	DayData * getDay(int index);

private:
	std::vector <DayData> m_days;
	bool m_initialized;
};

class DayData {

public:
	DayData();

	bool dayHasData() const;

	double getAvgTemp() const;

	bool isDayAbove100() const;
	bool isDayBelow0() const;

	int getMaxTemp() const;
	int getMinTemp() const;

	const std::vector<int> & getMaxTemps() const;
	const std::vector<int> & getMinTemps() const;

	void addTmax(int tmax);
	void addTmin(int tmin);

private:
	std::vector<int> m_maxTemps;
	std::vector<int> m_minTemps;
};

#endif
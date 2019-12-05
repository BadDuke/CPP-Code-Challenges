#define _CRT_SECURE_NO_WARNINGS // VS no deprecation

#include "DataTypes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <algorithm>

using namespace std;

YearData::YearData()
{
	m_initialized = false; // checks if year is initialized, used during data processing
	m_year = 0;
	m_months = std::vector<MonthData>(MONTHS_IN_YEAR);
}

void YearData::initialize(int year)
{
	if (m_initialized)
	{
		return;
	}

	m_year = year;

	for (int monthIndex = 0; monthIndex < m_months.size(); monthIndex++)
	{
		m_months[monthIndex].initialize(monthIndex);
	}

	m_initialized = true;
}

MonthData * YearData::getMonth(int index)
{
	if (index >= 0 || index < m_months.size())
	{
		return &m_months[index];
	}
	else {
		return 0;
	}
}

bool YearData::isInitialized() const
{
	return m_initialized;
}

MonthData::MonthData()
{
	m_initialized = false;
}

void MonthData::initialize(int month)
{
	if (m_initialized)
	{
		return;
	}

	m_days = vector<DayData>(MAX_DAYS_IN_MONTH);
	m_initialized = true;
}

DayData * MonthData::getDay(int index)
{
	if (index >= 0 || index < m_days.size())
	{
		return &m_days[index];
	}
	else {
		return 0;
	}
}

 DayData::DayData()
{
	m_maxTemps = vector<int>();
	m_minTemps = vector<int>();
}

bool DayData::dayHasData() const
{
	return (m_maxTemps.size() > 0 || m_minTemps.size() > 0);
}

double DayData::getAvgTemp() const
{
	double minTempSum = 0;
	double maxTempSum = 0;
	int tempCount = 0;

	for (int i = 0; i < m_minTemps.size(); i++)
	{
		minTempSum += m_minTemps[i];
		tempCount++;
	}

	if (tempCount == 0)
	{
		minTempSum = -9999;
	}
	else
	{
		minTempSum /= tempCount;
	}
	tempCount = 0;

	for (int i = 0; i < m_maxTemps.size(); i++)
	{
		maxTempSum += m_maxTemps[i];
		tempCount++;
	}

	if (tempCount == 0)
	{
		maxTempSum = -9999;
	}
	else
	{
		maxTempSum /= tempCount;
	}

	if (maxTempSum != -9999 && minTempSum != -9999)
	{
		return (minTempSum + maxTempSum) / 2;
	}
	else if (maxTempSum != -9999)
	{
			return maxTempSum;
	}
	else if (minTempSum != -9999)
	{
			return minTempSum;
	}
	else
	{
		return -9999;
	}
}

bool DayData::isDayAbove100() const
{
	for (int dayTemp : m_maxTemps)
	{
		if (dayTemp > 100)
		{
			return true;
		}
	}
	return false;
}

bool DayData::isDayBelow0() const
{
	for (int dayTemp : m_minTemps)
	{
		if (dayTemp < 0) {
			return true;
		}
	}
	return false;
}

int DayData::getMaxTemp() const
{
	vector<int>::const_iterator maxElement = max_element(m_maxTemps.begin(), m_maxTemps.end());

	if (maxElement != m_maxTemps.end())
	{
		return *maxElement;
	}
	else
	{
		return -9999;
	}
}

int DayData::getMinTemp() const
{
	vector<int>::const_iterator minElement = min_element(m_minTemps.begin(), m_minTemps.end());
	if (minElement != m_minTemps.end())
	{
		return *minElement;
	}
	else
	{
		return -9999;
	}
}

const std::vector<int>& DayData::getMaxTemps() const
{
	return m_maxTemps;
}

const std::vector<int>& DayData::getMinTemps() const
{
	return m_minTemps;
}


void DayData::addTmax(int tmax)
{
	// Only add the data if its valid
	if (tmax != -9999)
	{
		m_maxTemps.push_back(tmax);
	}
}

void DayData::addTmin(int tmin)
{
	// Only add the data if its valid
	if (tmin != -9999)
	{
		m_minTemps.push_back(tmin);
	}
}
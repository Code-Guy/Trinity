#include "tool.h"

using namespace std;

string Tool::GetHMSFormatStr(float t)//得到时-分-秒标准时间格式字符串
{
	int h = t / 3600;
	int m = (t - 3600 * h) / 60;
	int s = t - 3600 * h - 60 * m;

	std::string strH, strM, strS;
	char buffer[5];

	if (h == 0)
	{
		strH = "00";
	}
	else if (h < 10)
	{
		strH = std::string("0") + itoa(h, buffer, 10);
	}
	else
	{
		strH = itoa(h, buffer, 10);
	}

	if (m == 0)
	{
		strM = "00";
	}
	else if (m < 10)
	{
		strM = std::string("0") + itoa(m, buffer, 10);
	}
	else
	{
		strM = itoa(m, buffer, 10);
	}

	if (s == 0)
	{
		strS = "00";
	}
	else if (s < 10)
	{
		strS = std::string("0") + itoa(s, buffer, 10);
	}
	else
	{
		strS = itoa(s, buffer, 10);
	}

	return strH + ":" + strM + ":" + strS;
}
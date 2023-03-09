#ifndef SYSTICK_H
#define SYSTICK_H

#include <ctime>
#include <chrono>
#include <QString>

class SysTick
{
public:
    //! 获取时间字符串
    inline static QString getStr()
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm *ptm = localtime(&tt);
        char date[32];
        sprintf(date, "[%d-%02d-%02d_%02d:%02d:%02d] ",
                ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
                ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        return QString(date);
    }
};

#endif // SYSTICK_H

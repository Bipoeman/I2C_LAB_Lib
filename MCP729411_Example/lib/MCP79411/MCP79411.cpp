#include "MCP79411.h"

void MCP79411_RTC::init(TwoWire *wireInstance)
{
    rtc_address = MCP79411_ADDR;
    _wire = wireInstance;
    uint8_t data;
    readRegs(MCP79411_SECONDS_REG, &data, 1);
    int err = writeReg(MCP79411_SECONDS_REG, (1 << 7) | data);
    uint8_t getHourConfig;
    readRegs(MCP79411_HOURS_REG, &getHourConfig, 1);
    _isAm = (getHourConfig >> 5) & 1;
    _is12 = (getHourConfig >> 6) & 1;
    readRegs(MCP79411_DAY_REG,&data,1);
    data |= (0b111 << 3);
    writeReg(MCP79411_DAY_REG,data);
}

int MCP79411_RTC::getSeconds()
{
    uint8_t data;
    readRegs(MCP79411_SECONDS_REG, &data, 1);
    data &= (128 - 1);
    int tenSecs = data >> 4;
    int secs = data & 0x0F;
    return (tenSecs * 10) + secs;
}

int MCP79411_RTC::getMinutes()
{
    uint8_t data;
    readRegs(MCP79411_MINUTES_REG, &data, 1);
    data &= (128 - 1);
    int tenMins = data >> 4;
    int mins = data & 0x0F;
    return (tenMins * 10) + mins;
}

int MCP79411_RTC::getHours()
{
    uint8_t data;
    readRegs(MCP79411_HOURS_REG, &data, 1);
    _isAm = (data >> 5) & 1;
    _is12 = (data >> 6) & 1;
    data &= (32 - 1);
    int tenHours = (data >> 4) & (_is12 ? 1 : 3);
    int hour = data & 0x0F;
    return (tenHours * 10) + hour;
}

int MCP79411_RTC::isAM()
{
    return _isAm;
}
int MCP79411_RTC::is12Hour()
{
    return _is12;
}

int MCP79411_RTC::getDay()
{
    uint8_t data;
    readRegs(MCP79411_HOURS_REG, &data, 1);
    data &= (128 - 1);
    int day = data & 0x07;
    return day;
}

int MCP79411_RTC::getDate()
{
    uint8_t data;
    readRegs(MCP79411_DATE_REG, &data, 1);
    data &= (64 - 1);
    int tenDate = data >> 4;
    int date = data & 0x0F;
    return (tenDate * 10) + date;
}

int MCP79411_RTC::getMonth()
{
    uint8_t data;
    readRegs(MCP79411_MONTH_REG, &data, 1);
    data &= (32 - 1);
    int tenMonth = data >> 4;
    int month = data & 0x0F;
    return (tenMonth * 10) + month;
}

int MCP79411_RTC::getYear()
{
    uint8_t data;
    readRegs(MCP79411_YEAR_REG, &data, 1);
    _isLeap = (data >> 5) & 1;
    data &= (32 - 1);
    int tenYear = data >> 4;
    int year = data & 0x0F;
    return 2000 + (tenYear * 10) + year;
}

DateTime MCP79411_RTC::getDateTime()
{
    DateTime output;
    uint8_t data[10];
    readRegs(MCP79411_SECONDS_REG, data, 7);

    data[MCP79411_SECONDS_REG] &= (128 - 1);
    int tenSecs = data[MCP79411_SECONDS_REG] >> 4;
    int secs = data[MCP79411_SECONDS_REG] & 0x0F;
    output.seconds = (tenSecs * 10) + secs;

    data[MCP79411_MINUTES_REG] &= (128 - 1);
    int tenMins = data[MCP79411_MINUTES_REG] >> 4;
    int mins = data[MCP79411_MINUTES_REG] & 0x0F;
    output.minutes = (tenMins * 10) + mins;

    _isAm = (data[MCP79411_HOURS_REG] >> 5) & 1;
    _is12 = (data[MCP79411_HOURS_REG] >> 6) & 1;
    // data[MCP79411_HOURS_REG] &= (32 - 1);
    int tenHours = (data[MCP79411_HOURS_REG] >> 4) & (_is12 ? 1 : 3);
    int hour = data[MCP79411_HOURS_REG] & 0x0F;
    output.hours = (tenHours * 10) + hour;

    data[MCP79411_DAY_REG] &= (128 - 1);
    int day = data[MCP79411_DAY_REG] & 0x07;
    output.day = day;

    data[MCP79411_DATE_REG] &= (64 - 1);
    int tenDate = (data[MCP79411_DATE_REG] >> 4) & 0x03;
    int date = data[MCP79411_DATE_REG] & 0x0F;
    output.date = (tenDate * 10) + date;

    data[MCP79411_MONTH_REG] &= (32 - 1);
    int tenMonth = data[MCP79411_MONTH_REG] >> 4;
    int month = data[MCP79411_MONTH_REG] & 0x0F;
    output.month = (tenMonth * 10) + month;

    data[MCP79411_YEAR_REG] &= (256 - 1);
    int tenYear = data[MCP79411_YEAR_REG] >> 4;
    int year = data[MCP79411_YEAR_REG] & 0x0F;
    output.year = 2000 + (tenYear * 10) + year;

    return output;
}

int MCP79411_RTC::getOnBoardOscillator()
{
    uint8_t data;
    int err = readRegs(MCP79411_SECONDS_REG, &data, 1);
    return (data >> 7);
}

void MCP79411_RTC::setTime(int hour, int minute, int second)
{
    uint8_t readVal[3];
    readRegs(MCP79411_SECONDS_REG, readVal, 3);
    readVal[0] = (readVal[0] & ~(128 - 1)) | ((second % 10) | ((second / 10) << 4));
    readVal[1] = (readVal[1] & ~(128 - 1)) | ((minute % 10) | ((minute / 10) << 4));
    if (!_is12)
    {
        readVal[2] = (readVal[2] & ~(32 - 1)) | ((hour % 10) | ((hour / 10) << 4));
    }
    else
    {
        int isItPM = hour > 12;
        hour -= (isItPM * 12);
        readVal[2] = (readVal[2] & ~(64 - 1)) | ((hour % 10) | ((hour / 10) << 4) | (!isItPM << 5));
    }
    writeRegs(MCP79411_SECONDS_REG, readVal, 3);
}

void MCP79411_RTC::setDate(int date, int month, int year)
{
    uint8_t readVal[3];
    year -= 2000;
    readRegs(MCP79411_DATE_REG, readVal, 3);
    readVal[0] = (readVal[0] & ~(64 - 1)) |     ((date % 10)    | ((date / 10) << 4));
    readVal[1] = (readVal[1] & ~(32 - 1)) |     ((month % 10)   | ((month / 10) << 4));
    readVal[2] = (readVal[2] & ~(256 - 1)) |    ((year % 10)    | ((year / 10) << 4));
    writeRegs(MCP79411_DATE_REG,readVal,3);
    // writeReg(MCP79411_DATE_REG,*readVal);
    // writeReg(MCP79411_MONTH_REG,*(readVal+1));
    // writeReg(MCP79411_YEAR_REG,*(readVal+2));
}

void MCP79411_RTC::set24Hour()
{
    uint8_t data;
    readRegs(MCP79411_HOURS_REG, &data, 1);
    // _isAm = (data >> 5) & 1;
    data &= ~(1 << 6);
    writeReg(MCP79411_HOURS_REG, data);
    _is12 = 0;
    // DateTime temp = getDateTime();
    // setTime(temp.hours + (_isAm ? 0 : 12),temp.minutes,temp.seconds);
}

void MCP79411_RTC::setDay(int day)
{
    uint8_t data;
    readRegs(MCP79411_DAY_REG, &data, 1);
    data = (day & 7) | (data & ~(0b111));
    writeReg(MCP79411_DAY_REG, data);
}

void MCP79411_RTC::set12Hour()
{
    uint8_t data;
    readRegs(MCP79411_HOURS_REG, &data, 1);
    data |= 1 << 6;
    writeReg(MCP79411_HOURS_REG, data);
    _is12 = 1;
}

int MCP79411_RTC::writeReg(uint8_t reg, uint8_t data)
{
    return writeRegs(reg, &data, 1);
}

int MCP79411_RTC::writeRegs(uint8_t reg, uint8_t *data, int size)
{
    int error;
    _wire->beginTransmission(rtc_address);
    _wire->write(reg);
    for (int i = 0; i < size; i++)
    {
        _wire->write(data[i]);
    }
    error = _wire->endTransmission();
    return error;
}

int MCP79411_RTC::readRegs(uint8_t reg, uint8_t *data, int size)
{
    _wire->beginTransmission(rtc_address);
    _wire->write(reg);
    int err = _wire->endTransmission();
    _wire->requestFrom(rtc_address, size);
    while (_wire->available())
    {
        _wire->readBytes(data, size);
    }
    return err;
}

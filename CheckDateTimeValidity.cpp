struct DateTime{
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
};

//класс для проверки корретности даты
class CheckerDate {
public:
    CheckerDate(const DateTime& dt):dt_(&dt) {};

    void CheckDate(){
        CheckYear(dt_->year);
        CheckMonth(dt_->month);
        CheckDay(dt_->day);
        CheckMinute(dt_->minute);
        CheckSecond(dt_->second);
    }

private:
    const DateTime* dt_;

    void CheckYear(uint16_t year) {
        if (year < 1) {
            throw domain_error("year is too small"s);
        }
        if (year > 9999) {
            throw domain_error("year is too big"s);
        }
    }
    void CheckMonth(uint16_t month) {
        if (month < 1) {
            throw domain_error("month is too small"s);
        }
        if (month > 12) {
            throw domain_error("month is too big"s);
        }
    }
    void CheckDay(uint16_t day) {
        
        const bool is_leap_year = (dt_->year % 4 == 0) && !(dt_->year % 100 == 0 && dt_->year % 400 != 0);
        const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        
        if (day < 1) {
            throw domain_error("day is too small"s);
        }
        if (day > month_lengths[dt_->month - 1]) {
            throw domain_error("day is too big"s);
        }
    }
    void CheckMinute(uint16_t minute) {
        if (minute < 0) {
            throw domain_error("minute is too small"s);
        }
        if (minute > 59) {
            throw domain_error("minute is too big"s);
        }
    }
    void CheckSecond(uint16_t second) {
        if (second < 0) {
            throw domain_error("second is too small");
        }
        if (second > 59) {
            throw domain_error("second is too big"s);
        }
    }
};

void CheckDateTimeValidity(const DateTime& dt) {
    CheckerDate cheker(dt);
    cheker.CheckDate();
}

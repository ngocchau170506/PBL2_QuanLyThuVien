#pragma once
#include <cstddef>

class Utils
{
public:
    static int StringToIntManual(const char* str); // chuyển chuỗi thành số nguyên
    static int FindSubstringManual(const char* haystack, const char* needle); // kiểm tra nội dung chuỗi
    static void CopyStringManual(char* dest, const char* src, size_t dest_size); // sao chép tên file
    static int SplitLineManual(char* line, char fields[][100], int maxFields, char delimiter); //đọc file .txt
    static int CharArrayToIntManual(const char* str); // đọc id, năm từ file
    static void IntToStringManual(int num, char* buffer, size_t buffer_size); // chuyển số thành chuỗi
    static void GetCurrentDate(char* buffer, size_t buffer_size); // Lấy ngày hiện tại dưới dạng chuỗi
    static int CompareDates(const char* date1, const char* date2);
    static void GetCurrentDateManual(char* buffer, size_t buffer_size);
    static int CalculateDateDifference(const char* date1, const char* date2);
    static void ToUpperManual(char* str); 
    static void ToLowerManual(char* str); 
    static const int daysInMonth[13];

static bool IsLeap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static long long GetTotalDays(const char* date)
{
    char temp[5]; 
    
    Utils::CopyStringManual(temp, date, 3); // Lấy dd
    int d = Utils::CharArrayToIntManual(temp);

    // Tháng
    Utils::CopyStringManual(temp, date + 3, 3); // Lấy mm
    int m = Utils::CharArrayToIntManual(temp);

    // Năm
    Utils::CopyStringManual(temp, date + 6, 5); // Lấy yyyy
    int y = Utils::CharArrayToIntManual(temp);

    long long totalDays = 0;

    // 1. Cộng tổng số ngày từ các năm đã trôi qua
    for (int i = 1; i < y; i++) {
        totalDays += 365;
        if (IsLeap(i)) {
            totalDays++; // Cộng thêm ngày nhuận
        }
    }

    // 2. Cộng tổng số ngày từ các tháng đã trôi qua trong năm hiện tại
    for (int i = 1; i < m; i++) {
        totalDays += daysInMonth[i];
    }
    
    // 3. Cộng thêm 1 ngày nếu là năm nhuận và tháng > 2
    if (m > 2 && IsLeap(y)) {
        totalDays++;
    }

    // 4. Cộng số ngày trong tháng hiện tại
    totalDays += d;
    
    return totalDays;
}
    
};
void Utils::ToUpperManual(char* str)
{
    while (*str)
    {
        // Nếu ký tự nằm trong khoảng 'a' đến 'z', trừ đi 32 để thành in hoa
        if (*str >= 'a' && *str <= 'z')
        {
            *str = *str - 32;
        }
        str++;
    }
}

void Utils::ToLowerManual(char* str)
{
    while (*str)
    {
        // Nếu ký tự nằm trong khoảng 'A' đến 'Z', cộng thêm 32 để thành in thường
        if (*str >= 'A' && *str <= 'Z')
        {
            *str = *str + 32;
        }
        str++;
    }
}
const int Utils::daysInMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int Utils::CalculateDateDifference(const char* date1, const char* date2)
{

    
    long long days1 = GetTotalDays(date1);
    long long days2 = GetTotalDays(date2);

    // Ép kiểu về int vì chênh lệch thường không quá giới hạn của int
    return (int)(days1 - days2); 
}

void Utils::GetCurrentDateManual(char* buffer, size_t buffer_size)
{
    // NGÀY MẶC ĐỊNH CHO HỆ THỐNG (Có thể thay đổi thủ công)
    const char* defaultDate = "19/11/2025"; 
    
    // Sao chép ngày mặc định vào buffer
    CopyStringManual(buffer, defaultDate, buffer_size);
}

int Utils::CompareDates(const char* date1, const char* date2)
{
    // Cần phải xử lý từ lớn đến bé: Năm, Tháng, Ngày
    // Định dạng: dd/mm/yyyy

    // 1. So sánh NĂM (yyyy)
    int year1 = CharArrayToIntManual(date1 + 6);
    int year2 = CharArrayToIntManual(date2 + 6);
    if (year1 != year2) return year1 - year2;

    // 2. So sánh THÁNG (mm)
    // Bắt đầu từ vị trí 3, chỉ lấy 2 ký tự (chú ý đến hàm CharArrayToIntManual của bạn)
    char monthStr1[3] = {date1[3], date1[4], '\0'};
    char monthStr2[3] = {date2[3], date2[4], '\0'};
    int month1 = CharArrayToIntManual(monthStr1);
    int month2 = CharArrayToIntManual(monthStr2);
    if (month1 != month2) return month1 - month2;

    // 3. So sánh NGÀY (dd)
    // Bắt đầu từ vị trí 0, chỉ lấy 2 ký tự
    char dayStr1[3] = {date1[0], date1[1], '\0'};
    char dayStr2[3] = {date2[0], date2[1], '\0'};
    int day1 = CharArrayToIntManual(dayStr1);
    int day2 = CharArrayToIntManual(dayStr2);
    return day1 - day2; // Trả về < 0 nếu date1 < date2, > 0 nếu date1 > date2
}

void Utils::IntToStringManual(int num, char* buffer, size_t buffer_size)
{
    if (buffer_size == 0) return;
    
    // Trường hợp đặc biệt: num = 0
    if (num == 0) {
        if (buffer_size > 1) {
            buffer[0] = '0';
            buffer[1] = '\0';
        } else if (buffer_size == 1) {
            buffer[0] = '\0';
        }
        return;
    }

    // Xử lý số âm
    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        // Chuyển sang dương để xử lý từng chữ số
        num = -num; 
    }

    int i = 0;
    // Chuyển số thành chuỗi (bị đảo ngược)
    while (num != 0 && i < buffer_size - 1) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Thêm dấu trừ
    if (isNegative && i < buffer_size - 1) {
        buffer[i++] = '-';
    }
    buffer[i] = '\0'; // Kết thúc chuỗi

    // Đảo ngược chuỗi để có thứ tự đúng
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

int Utils::FindSubstringManual(const char* haystack, const char* needle)
{
    if (!*needle) return 0;

    while (*haystack)
    {
        const char* h = haystack;
        const char* n = needle;
        while (*n && *h == *n)
        {
            h++;
            n++;
        }
        if (!*n) return 1;
        haystack++;
    }
    return 0;
}

void Utils::CopyStringManual(char* dest, const char* src, size_t dest_size)
{
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int Utils::SplitLineManual(char* line, char fields[][100], int maxFields, char delimiter)
{
    int fieldIndex = 0;
    int charIndex = 0;
    
    while (*line != '\0' && fieldIndex < maxFields)
    {
        if (*line == delimiter)
        {
            fields[fieldIndex][charIndex] = '\0';
            fieldIndex++;
            charIndex = 0;
        }
        else
        {
            if (charIndex < 99)
            {
                fields[fieldIndex][charIndex] = *line;
                charIndex++;
            }
        }
        line++;
    }
    fields[fieldIndex][charIndex] = '\0';
    return fieldIndex + 1;
}


int Utils::CharArrayToIntManual(const char* str)
{
    int res = 0;
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            res = res * 10 + (*str - '0');
        } else {
            break;
        }
        str++;
    }
    return res * sign;
}
// ----------------------------------------------------------------------------------
// PHƯƠNG THỨC LẤY NGÀY HIỆN TẠI (GetCurrentDate)
// ----------------------------------------------------------------------------------
void Utils::GetCurrentDate(char* buffer, size_t buffer_size)
{
    if (buffer_size < 11) { // Định dạng YYYY-MM-DD\0 cần ít nhất 11 ký tự
        if (buffer_size > 0) buffer[0] = '\0';
        return;
    }

    time_t now = time(0);
    // Chuyển đổi sang cấu trúc thời gian cục bộ (local time)
    tm *ltm = localtime(&now);

    // Sử dụng sprintf để định dạng chuỗi YYYY-MM-DD
    // Ghi kết quả vào buffer. sprintf trả về số ký tự đã ghi (không kể '\0')
    int written = sprintf(buffer, "%02d/%02d/%04d", 
                          ltm->tm_mday,
                          1 + ltm->tm_mon,
                          1900 + ltm->tm_year);      // Ngày trong tháng

    // Đảm bảo chuỗi kết thúc bằng null, mặc dù sprintf đã làm điều này
    if (written < buffer_size) {
        buffer[written] = '\0';
    } else {
        buffer[buffer_size - 1] = '\0';
    }
}

// Triển khai Utils::StringToIntManual(const char* str)
// Giả sử hàm này xử lý việc kiểm tra tính hợp lệ của toàn bộ chuỗi input.
int Utils::StringToIntManual(const char* str)
{
    // Cần bỏ qua các khoảng trắng đầu/cuối nếu có
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    
    // Sử dụng logic chuyển đổi của CharArrayToIntManual
    int res = 0;
    int sign = 1;
    const char* start = str;

    if (*str == '-') {
        sign = -1;
        str++;
    }

    // Yêu cầu phải có ít nhất một chữ số
    if (!(*str >= '0' && *str <= '9')) {
        return -1; // Trả về -1 nếu không phải là số hợp lệ
    }
    
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            res = res * 10 + (*str - '0');
        } else if (*str == '\n' || *str == '\r') {
             break; // Cho phép kết thúc bằng ký tự xuống dòng
        } else {
            // Nếu gặp ký tự không phải số, đây là input không hợp lệ
            return -1; 
        }
        str++;
    }
    return res * sign;
}
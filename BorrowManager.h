// #pragma once
// #include <iostream>
// #include <fstream>
// #include "BorrowedItem.h"
// #include "BookManager.h"
// #include "UserManager.h"
// #include "Utils.h"


// using namespace std;

// class BorrowManager
// {
// private:
//     BorrowedItem records[1000];
//     int recordCount;
//     char filePath[100];

//     // Helpers xử lý ngày (dùng Utils)
//     static long long DateToTotalDays(const char* date); // wrapper -> Utils::GetTotalDays
//     static void TotalDaysToDate(long long totalDays, char* outDate, size_t outSize);
//     static void AddDaysToDate(const char* date, int daysToAdd, char* outDate, size_t outSize);

// public:
//     BorrowManager();

//     void LoadFromFile();
//     void SaveToFile() const;

//     void BorrowBook(UserManager& userMgr, BookManager& bookMgr);
//     void ReturnBook(UserManager& userMgr, BookManager& bookMgr);

//     void ShowAllBorrowRecords() const;
//     void ShowOverdueBooks(const BookManager& bookMgr) const;

//     int CalculateFine(const char* dueDate, const char* returnDate) const;
// };

// // ---------------- constructor ----------------
// BorrowManager::BorrowManager()
// {
//     recordCount = 0;
//     Utils::CopyStringManual(filePath, "borrow.txt", sizeof(filePath));
//     LoadFromFile();
// }

// // ----------------- helpers ngày -----------------
// // wrapper sang Utils::GetTotalDays
// long long BorrowManager::DateToTotalDays(const char* date)
// {
//     return Utils::GetTotalDays(date);
// }

// // chuyển totalDays -> "dd/mm/yyyy"
// // Lưu ý: Utils::daysInMonth và Utils::IsLeap có thể truy cập trực tiếp
// void BorrowManager::TotalDaysToDate(long long totalDays, char* outDate, size_t outSize)
// {
//     // totalDays là số ngày kể từ 1/1/1 (theo cách GetTotalDays tính)
//     // Ta sẽ lặp qua các năm trừ dần
//     int year = 1;
//     while (true)
//     {
//         int daysInYear = 365 + (Utils::IsLeap(year) ? 1 : 0);
//         if (totalDays > daysInYear)
//         {
//             totalDays -= daysInYear;
//             year++;
//         }
//         else break;
//     }

//     // totalDays hiện là số ngày trong năm (1..365/366)
//     int month = 1;
//     while (month <= 12)
//     {
//         int dim = Utils::daysInMonth[month];
//         if (month == 2 && Utils::IsLeap(year)) dim++; // Feb leap
//         if (totalDays > dim)
//         {
//             totalDays -= dim;
//             month++;
//         }
//         else break;
//     }

//     int day = (int)totalDays; // 1-based

//     // format "dd/mm/yyyy"
//     // Tạo buffer tạm và dùng Utils::IntToStringManual / CopyStringManual
//     char tmp[12];
//     // day
//     char part[6];
//     Utils::IntToStringManual(day, part, sizeof(part));
//     if (day < 10) {
//         // ensure leading zero
//         char s[3] = { '0', part[0], '\0' };
//         Utils::CopyStringManual(tmp, s, sizeof(tmp));
//     } else {
//         Utils::CopyStringManual(tmp, part, sizeof(tmp));
//     }
//     Utils::CopyStringManual(outDate, tmp, outSize);
//     // add '/'
//     size_t len = 0; while (outDate[len] != '\0') len++;
//     if (len + 1 < outSize) { outDate[len] = '/'; outDate[len+1] = '\0'; }

//     // month
//     char monthPart[6];
//     Utils::IntToStringManual(month, monthPart, sizeof(monthPart));
//     if (month < 10) {
//         char s[3] = { '0', monthPart[0], '\0' };
//         Utils::CopyStringManual(tmp, s, sizeof(tmp));
//     } else {
//         Utils::CopyStringManual(tmp, monthPart, sizeof(tmp));
//     }
//     // append month
//     len = 0; while (outDate[len] != '\0') len++;
//     size_t toCopy = 0; while (tmp[toCopy] != '\0') toCopy++;
//     // append tmp into outDate
//     for (size_t i = 0; i <= toCopy && (len + i + 1) < outSize; ++i)
//         outDate[len + i] = tmp[i];
//     // add '/'
//     len = 0; while (outDate[len] != '\0') len++;
//     if (len + 1 < outSize) { outDate[len] = '/'; outDate[len+1] = '\0'; }

//     // year
//     char yearPart[8];
//     Utils::IntToStringManual(year, yearPart, sizeof(yearPart));
//     // append yearPart
//     len = 0; while (outDate[len] != '\0') len++;
//     size_t ylen = 0; while (yearPart[ylen] != '\0') ylen++;
//     for (size_t i = 0; i <= ylen && (len + i) < outSize; ++i)
//         outDate[len + i] = yearPart[i];
// }

// // add daysToAdd to date (format dd/mm/yyyy)
// void BorrowManager::AddDaysToDate(const char* date, int daysToAdd, char* outDate, size_t outSize)
// {
//     long long total = DateToTotalDays(date);
//     total += daysToAdd;
//     TotalDaysToDate(total, outDate, outSize);
// }

// // ---------------- Load/Save ----------------
// void BorrowManager::LoadFromFile()
// {
//     ifstream inFile(filePath);
//     if (!inFile.is_open()) return;

//     char line[300];
//     while (inFile.getline(line, sizeof(line)))
//     {
//         char fields[5][100];
//         int cnt = Utils::SplitLineManual(line, fields, 5, ',');
//         if (cnt < 5) continue;

//         records[recordCount].bookID = Utils::CharArrayToIntManual(fields[0]);
//         records[recordCount].userID = Utils::CharArrayToIntManual(fields[1]);
//         Utils::CopyStringManual(records[recordCount].dateBorrow, fields[2], sizeof(records[recordCount].dateBorrow));
//         Utils::CopyStringManual(records[recordCount].dateDue, fields[3], sizeof(records[recordCount].dateDue));
//         Utils::CopyStringManual(records[recordCount].dateReturn, fields[4], sizeof(records[recordCount].dateReturn));

//         recordCount++;
//         if (recordCount >= 1000) break;
//     }
//     inFile.close();
// }

// void BorrowManager::SaveToFile() const
// {
//     ofstream outFile(filePath);
//     for (int i = 0; i < recordCount; ++i)
//     {
//         outFile << records[i].bookID << ","
//                 << records[i].userID << ","
//                 << records[i].dateBorrow << ","
//                 << records[i].dateDue << ","
//                 << records[i].dateReturn << "\n";
//     }
//     outFile.close();
// }

// // ---------------- BorrowBook ----------------
// void BorrowManager::BorrowBook(UserManager& userMgr, BookManager& bookMgr)
// {
//     int userID, bookID;
//     cout << "\nNhap ID nguoi muon sach: ";
//     cin >> userID;
//     cout << "Nhap ID sach: ";
//     cin >> bookID;

//     Person* p = userMgr.GetUserByID(userID);
//     if (!p) {
//         cout << "Nguoi dung khong ton tai.\n";
//         return;
//     }

//     Book* b = bookMgr.GetBookByID(bookID);
//     if (!b) {
//         cout << "Sach khong ton tai.\n";
//         return;
//     }

//     if (b->getAvailableQuantity() <= 0) {
//         cout << "Sach het.\n";
//         return;
//     }

//     // cap nhat so luong sach
//     b->setAvailableQuantity(b->getAvailableQuantity() - 1);
//     bookMgr.SaveBooksToFile();

//     // tao record
//     BorrowedItem &it = records[recordCount];
//     it.bookID = bookID;
//     it.userID = userID;

//     Utils::GetCurrentDateManual(it.dateBorrow, sizeof(it.dateBorrow));
//     AddDaysToDate(it.dateBorrow, 7, it.dateDue, sizeof(it.dateDue)); // han mac dinh 7 ngay
//     Utils::CopyStringManual(it.dateReturn, "NONE", sizeof(it.dateReturn));

//     recordCount++;
//     SaveToFile();

//     cout << "Muon sach thanh cong. Han tra: " << it.dateDue << "\n";
// }

// // ---------------- ReturnBook ----------------
// void BorrowManager::ReturnBook(UserManager& userMgr, BookManager& bookMgr)
// {
//     int userID, bookID;
//     cout << "\nNhap ID nguoi tra sach: ";
//     cin >> userID;
//     cout << "Nhap ID sach: ";
//     cin >> bookID;

//     for (int i = 0; i < recordCount; ++i)
//     {
//         if (records[i].userID == userID &&
//             records[i].bookID == bookID &&
//             Utils::FindSubstringManual(records[i].dateReturn, "NONE")) // dateReturn == "NONE"
//         {
//             // tra sach: tang sl
//             Book* b = bookMgr.GetBookByID(bookID);
//             if (b) {
//                 b->setAvailableQuantity(b->getAvailableQuantity() + 1);
//                 bookMgr.SaveBooksToFile();
//             }

//             // ghi ngay tra
//             char today[11];
//             Utils::GetCurrentDateManual(today, sizeof(today));
//             Utils::CopyStringManual(records[i].dateReturn, today, sizeof(records[i].dateReturn));
//             SaveToFile();

//             // tinh phat
//             int fine = CalculateFine(records[i].dateDue, records[i].dateReturn);
//             if (fine > 0)
//                 cout << "Ban tra tre " << fine << " ngay.\n";
//             else
//                 cout << "Tra dung han.\n";

//             return;
//         }
//     }

//     cout << "Khong tim thay giao dich muon hop le.\n";
// }

// // ---------------- ShowAllBorrowRecords ----------------
// void BorrowManager::ShowAllBorrowRecords() const
// {
//     if (recordCount == 0) {
//         cout << "Chua co giao dich muon tra.\n";
//         return;
//     }
//     for (int i = 0; i < recordCount; ++i) {
//         cout << "UserID: " << records[i].userID
//              << " | BookID: " << records[i].bookID
//              << " | Muon: " << records[i].dateBorrow
//              << " | Han: " << records[i].dateDue
//              << " | Tra: " << records[i].dateReturn << "\n";
//     }
// }

// // ---------------- ShowOverdueBooks ----------------
// void BorrowManager::ShowOverdueBooks(const BookManager& bookMgr) const
// {
//     char today[11];
//     Utils::GetCurrentDateManual(today, sizeof(today));
//     bool found = false;
//     cout << "\n===== DANH SACH QUA HAN =====\n";
//     for (int i = 0; i < recordCount; ++i) {
//         // if not returned and due < today
//         if (Utils::FindSubstringManual(records[i].dateReturn, "NONE")) {
//             if (Utils::CompareDates(records[i].dateDue, today) < 0) {
//                 found = true;
//                 int overdueDays = Utils::CalculateDateDifference(today, records[i].dateDue);
//                 cout << "User " << records[i].userID << " | Book " << records[i].bookID;
//                 const Book* bk = bookMgr.GetBookByID(records[i].bookID);
//                 if (bk) cout << " (" << bk->getTitle() << ")";
//                 cout << " | Han: " << records[i].dateDue << " | QH: " << overdueDays << " ngay\n";
//             }
//         }
//     }
//     if (!found) cout << "Khong co sach qua han.\n";
// }

// // ---------------- CalculateFine ----------------
// int BorrowManager::CalculateFine(const char* dueDate, const char* returnDate) const
// {
//     // neu returnDate == "NONE" thi dung ngay hien tai
//     char cmpToday[11];
//     if (Utils::FindSubstringManual((char*)returnDate, "NONE")) {
//         Utils::GetCurrentDateManual(cmpToday, sizeof(cmpToday));
//         return Utils::CalculateDateDifference(cmpToday, dueDate);
//     } else {
//         return Utils::CalculateDateDifference(returnDate, dueDate);
//     }
// }

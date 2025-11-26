#pragma once
#include "Person.h"
#include "Utils.h"
#include "BorrowedItem.h"
#include "BookManager.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>


using namespace std;

class UserManager
{
private:
    Person users[100];
    int userCount;
    char userFilePath[100];

    bool IsUserIDExist(int id);
    bool IsValidStudentID(const char idStr[]);
    
public:
    UserManager();
    ~UserManager();

    void LoadUsersFromFile();
    void SaveUsersToFile() const;

    void AddUser();
    void ShowAllUsers() const;
    void DeleteUserByID(int id);
    void UpdateUserByID(int id);
    void UpdateByID(int id);
    // void ShowOverdueUsers(const BookManager& bm) const;

    int GetUserCount() const { return userCount; }
    Person* GetUserByID(int userID);
};

// void UserManager::ShowOverdueUsers(const BookManager& bm) const
// {
//     cout << "\n========== DANH SACH NGUOI DUNG QUA HAN ==========\n";

//     char currentDate[11];
//     Utils::GetCurrentDateManual(currentDate, sizeof(currentDate));
//     cout << "Ngay he thong hien tai: " << currentDate << "\n";

//     bool foundOverdue = false;
//     char bufferID[20];

//     for (int i = 0; i < userCount; ++i) {
//         User tempUser;
//         Utils::IntToStringManual(users[i].getID(), bufferID, sizeof(bufferID));
//         tempUser.LoadUserByID(bufferID);

//         int borrowedCount = tempUser.getCurrentBorrowedCount();
//         const BorrowedItem* borrowedArray = tempUser.getBorrowedBooks();

//         if (tempUser.getID() == 0 || borrowedCount == 0) {
//             continue;
//         }

//         bool userOverdue = false;

//         for (int j = 0; j < borrowedCount; ++j) {
//             const BorrowedItem& item = borrowedArray[j];

//             if (Utils::CompareDates(item.dateDue, currentDate) < 0) {
//                 int overdueDays = Utils::CalculateDateDifference(currentDate, item.dateDue);

//                 userOverdue = true;

//                 if (!foundOverdue) {
//                     cout << "--------------------------------------------------------\n";
//                     foundOverdue = true;
//                 }

//                 if (j == 0) {
//                     cout << "👤 DOC GIA: ID " << users[i].getID() << " - " << users[i].getName() << "\n";
//                 }

//                 cout << "   - Sach ID: " << item.getBookID();
//                 const Book* bookPtr = bm.GetBookByID(item.getBookID());
//                 if (bookPtr) {
//                     cout << " (" << bookPtr->getTitle() << ")";
//                 }
//                 cout << "\n   - Ngay Den Han: " << item.dateDue;
//                 cout << " | QUA HAN: " << overdueDays << " ngay\n";
//             }
//         }

//         if (userOverdue) {
//             cout << "--------------------------------------------------------\n";
//         }
//     }

//     cout << "==================================================\n";
//     if (!foundOverdue) {
//         cout << "Khong co nguoi dung nao bi qua han tai thoi diem nay.\n";
//     }
// }

Person* UserManager::GetUserByID(int userID)
{
    for (int i = 0; i < userCount; ++i)
    {
        if (users[i].getID() == userID)
        {
            return &users[i];
        }
    }
    return nullptr;
}

UserManager::UserManager()
{
    userCount = 0;
    Utils::CopyStringManual(userFilePath, "users.txt", sizeof(userFilePath));
    LoadUsersFromFile();
}

UserManager::~UserManager()
{
    SaveUsersToFile();
}

bool UserManager::IsUserIDExist(int id)
{
    for (int i = 0; i < userCount; ++i)
        if (users[i].getID() == id)
            return true;
    return false;
}

bool UserManager::IsValidStudentID(const char idStr[])
{
    int len = strlen(idStr);
    if (len != 9) return false;
    for (int i = 0; i < len; ++i)
        if (!isdigit(idStr[i])) return false;
    return true;
}

void UserManager::LoadUsersFromFile()
{
    ifstream inFile(userFilePath);
    if (!inFile.is_open()) return;

    char line[300];
    while (inFile.getline(line, sizeof(line)))
    {
        char fields[6][100];
        int fieldCount = Utils::SplitLineManual(line, fields, 6, ',');

        for (int i = 0; i < fieldCount; i++) {
            int len = strlen(fields[i]);
            while (len > 0 && (fields[i][len - 1] == '\n' ||
                               fields[i][len - 1] == '\r' ||
                               fields[i][len - 1] == ' ')) {
                fields[i][--len] = '\0';
            }
        }

        int id = Utils::CharArrayToIntManual(fields[0]);
        users[userCount].setID(id);
        users[userCount].setPassword(fields[1]);
        users[userCount].setName(fields[2]);
        users[userCount].setDate(fields[3]);
        users[userCount].setPhone(fields[4]);
        users[userCount].setEmail(fields[5]);
        userCount++;
    }
    inFile.close();
}

void UserManager::SaveUsersToFile() const
{
    ofstream outFile(userFilePath);
    for (int i = 0; i < userCount; ++i)
    {
        outFile << users[i].getID() << ","
                << users[i].getPassword() << ","
                << users[i].getName() << ","
                << users[i].getDate() << ","
                << users[i].getPhone() << ","
                << users[i].getEmail() << "\n";
    }
    outFile.close();
}

void UserManager::AddUser()
{
    char choice;
    do {
        cout << "\n--- THEM NGUOI DUNG MOI ---\n";

        char idStr[50];
        int tempID = 0;

        do {
            cout << "Nhap ma sinh vien: ";
            cin.getline(idStr, sizeof(idStr));

            if (!IsValidStudentID(idStr)) {
                cout << "Loi: Ma so sinh vien phai gom dung 9 chu so!\n";
                continue;
            }
            tempID = Utils::CharArrayToIntManual(idStr);
            if (IsUserIDExist(tempID)) {
                cout << "Loi: Ma so sinh vien " << tempID << " da ton tai! Vui long nhap ma khac.\n";
                continue;
            }
            break;
        } while (true);

        users[userCount].setID(tempID);
        users[userCount].InputAccountDetails();
        users[userCount].Show();

        userCount++;
        cout << "Them nguoi dung thanh cong! (Tong so: " << userCount << ")\n";
        SaveUsersToFile();

        cout << "Ban co muon them nguoi dung khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        choice = toupper(choice);

    } while (choice == 'Y');
}

void UserManager::ShowAllUsers() const
{
    if (userCount == 0)
    {
        cout << "Chua co nguoi dung nao!\n";
        return;
    }

    for (int i = 0; i < userCount; ++i)
    {
        cout << "Nguoi dung thu " << i + 1 << ":\n";
        users[i].Show();
    }
}

void UserManager::DeleteUserByID(int id)
{
    char choice;
    do {
        int pos = -1;
        for (int i = 0; i < userCount; ++i)
            if (users[i].getID() == id) { pos = i; break; }

        if (pos == -1)
        {
            cout << "Khong tim thay nguoi dung co ID " << id << endl;
        }
        else
        {
            cout << "\nBan co chac chan muon xoa nguoi dung sau khong?\n";
            users[pos].Show();
            cout << "Ban co chac chan muon xoa nguoi nay khong? (Y/N): ";
            char confirm;
            cin >> confirm;
            cin.ignore(100, '\n');
            confirm = toupper(confirm);

            if (confirm == 'Y')
            {
                for (int i = pos; i < userCount - 1; ++i)
                    users[i] = users[i + 1];
                userCount--;
                cout << "Xoa nguoi dung thanh cong!\n";
                SaveUsersToFile();
            }
            else
            {
                cout << "Da huy thao tac xoa.\n";
            }
        }

        cout << "Ban co muon xoa nguoi dung khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        choice = toupper(choice);

        if (choice == 'Y')
        {
            cout << "Nhap ID nguoi dung can xoa: ";
            if (!(cin >> id)) { cin.clear(); cin.ignore(100, '\n'); break; }
        }

    } while (choice == 'Y');
}

void UserManager::UpdateUserByID(int id)
{
    char choice;
    do {
        bool found = false;
        for (int i = 0; i < userCount; ++i)
        {
            if (users[i].getID() == id)
            {
                users[i].Update();
                SaveUsersToFile();
                found = true;
                break;
            }
        }
        if (!found)
            cout << "Khong tim thay nguoi dung co ID " << id << endl;

        cout << "Ban co muon sua nguoi dung khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        choice = toupper(choice);

        if (choice == 'Y')
        {
            cout << "Nhap ID nguoi dung can sua: ";
            if (!(cin >> id)) { cin.clear(); cin.ignore(100, '\n'); break; }
        }

    } while (choice == 'Y');
}

void UserManager::UpdateByID(int id)
{
    char choice;
    do {
        bool found = false;
        for (int i = 0; i < userCount; ++i)
        {
            if (users[i].getID() == id)
            {
                users[i].Update();
                SaveUsersToFile();
                found = true;
                break;
            }
        }
        if (!found)
            cout << "Khong tim thay nguoi dung co ID " << id << endl;

    } while (choice == 'Y');
}

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring> 
#include <limits> 
#include "Admin.h" 
#include "User.h" 

using namespace std;

class UserManager; 
class BookManager; 

class Login
{
private:
    UserManager& userManager;
    BookManager& bookManager;
    
    string Trim(const string& str); 
public:
    Login(UserManager& um, BookManager& bm) : userManager(um), bookManager(bm) {}
    
    int CompareString(const char* s1, const char* s2) const;
    bool DangNhap(const string& inputID, const string& inputPassword, const string& roleChoice, char* roleOutput); 
    void ShowStartMenu();
};

// Định nghĩa các hàm tiện ích ngay trong file .h
string Login::Trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

int Login::CompareString(const char* s1, const char* s2) const 
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        i++;
    }
    return s1[i] - s2[i];
}

bool Login::DangNhap(const string& inputID, const string& inputPassword, const string& roleChoice, char* roleOutput)
{
    string filename;
    
    if (roleChoice == "admin") {
        filename = "admin.txt";
    } else if (roleChoice == "reader") {
        filename = "users.txt";
    } else {
        return false;
    }

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Loi: Khong the mo file " << filename << endl;
        return false;
    }

    string line;
    getline(file, line); // Bỏ qua dòng tiêu đề

    while (getline(file, line))
    {
        stringstream ss(line);
        string segment;
        string fields[2];
        int count = 0;
        while (getline(ss, segment, ',') && count < 2)
        {
            fields[count++] = Trim(segment); 
        }
        if (fields[0] == inputID && fields[1] == inputPassword)
        {
            strncpy(roleOutput, roleChoice.c_str(), 10);
            roleOutput[9] = '\0';
            file.close();
            return true;
        }
    }
    file.close();
    roleOutput[0] = '\0'; 
    return false;
}


void Login::ShowStartMenu()
{
    // Cần đảm bảo rằng các file UserManager.h và BookManager.h đã được include 
    // TRƯỚC khi gọi ShowStartMenu() (ví dụ: trong main.cpp hoặc Login.cpp)
    
    int choice;
    string id, password;
    char role[10]; 
    string roleChoice; 
    
    do {
        cout << "\n========= HE THONG QUAN LY THU VIEN =========\n";
        cout << "1. Dang Nhap voi tu cach ADMIN\n";
        cout << "2. Dang Nhap voi tu cach DOC GIA\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); choice = -1;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1 || choice == 2) 
        {
            roleChoice = (choice == 1) ? "admin" : "reader"; 
            bool loggedIn = false;
            
            while (!loggedIn) {
                cout << "\n--- DANG NHAP (" << roleChoice << ") ---\n";
                cout << "Nhap ID: "; getline(cin, id); 
                if (id == "0" || id == "exit") {
                    cout << "Quay lai menu chinh...\n";
                    break; 
                }
                cout << "Mat Khau: "; getline(cin, password);
                if (password == "0" || password == "exit") {
                    cout << "Quay lai menu chinh...\n";
                    break;
                }
                id = Trim(id);
                password = Trim(password);
                
                if (DangNhap(id, password, roleChoice, role)) 
                {
                    cout << "\nDang nhap thanh cong! Vai tro: " << role << ".\n";
                    loggedIn = true;

                    if (CompareString(role, "admin") == 0) 
                    {
                        Admin adminManager; 
                        // Giả định Admin::Menu() không cần tham số
                        adminManager.Menu(); 
                    } 
                    else if (CompareString(role, "reader") == 0) 
                    {
                        User userTask;
                        userTask.LoadUserByID(id);
                        
                        // FIX LỖI: Gọi Menu với tham chiếu đã được lưu trữ
                        userTask.Menu(this->userManager, this->bookManager);
                        
                        cout << "[Chuyen huong den Menu DOC GIA]\n";
                    }
                } 
                else 
                {
                    cout << "ID hoac mat khau khong dung! Vui long nhap lai.\n";
                }
            }
        }
        else if (choice == 0) 
        {
            cout << "Tam biet.\n";
        }
        else 
        {
            cout << "Lua chon khong hop le!\n";
        }

    } while (choice != 0);
}
#pragma once
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

class BorrowedItem {
private:
    int bookID;
    char borrowDate[20]; // Dùng char array để đồng bộ với kiểu dữ liệu cũ (Person)

public:

    char dateBorrowed[11]; 
    char dateDue[11];      
    // Constructor mặc định
    BorrowedItem() : bookID(0) {
        strcpy(borrowDate, "");
    }
    
    // Getter cho BookID
    int getBookID() const { 
        return bookID; 
    }
    
    // Setter cho BookID
    void setBookID(int id) { 
        bookID = id; 
    }

    // Getter cho Ngày mượn
    const char* getBorrowDate() const { 
        return borrowDate; 
    }

    // Setter cho Ngày mượn
    void setBorrowDate(const char date[]) { 
        strncpy(borrowDate, date, 19);
        borrowDate[19] = '\0'; // Đảm bảo kết thúc chuỗi
    }
    
    // Bạn có thể thêm các thông tin khác như DueDate, ReturnDate... nếu cần.
};
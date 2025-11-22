#pragma once
#include <iostream>
#include "UserManager.h"
#include "BookManager.h"
#include "Utils.h"
#include "User.h"
using namespace std;

class Admin
{
private:
    UserManager userManager;
    BookManager bookManager;

    bool AskReturnToMenu();
    void UserMenu();
    void BookMenu();
    void BorrowBookMenu();
    void HandleBorrowBook();
    void HandleReturnBook();
public:
    void Menu();
};

bool Admin::AskReturnToMenu()
{
    char choice;
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    cin >> choice;
    cin.ignore(100, '\n');
    if (choice == 'Y' || choice == 'y')
        return true;
    return false;
}

void Admin::UserMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY NGUOI DUNG =========\n";
        cout << "1. Them nguoi dung\n";
        cout << "2. Xem danh sach nguoi dung\n";
        cout << "3. Sua thong tin nguoi dung\n";
        cout << "4. Xoa nguoi dung\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            userManager.AddUser();
        else if (choice == 2)
        {
            userManager.ShowAllUsers();
            AskReturnToMenu();
        }
        else if (choice == 3)
        {
            int id;
            cout << "Nhap ID nguoi dung can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.UpdateUserByID(id);
            AskReturnToMenu();
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID nguoi dung can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.DeleteUserByID(id);
            AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::BookMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY SACH =========\n";
        cout << "1. Them sach\n";
        cout << "2. Xem danh sach sach\n";
        cout << "3. Tim sach theo ten\n";
        cout << "4. Sua thong tin sach\n";
        cout << "5. Xoa sach\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            bookManager.AddBook();
        else if (choice == 2)
        {
            bookManager.ShowAllBooks();
            AskReturnToMenu();
        }
        else if (choice == 3)
        {
            bookManager.SearchBookByTitle();
            AskReturnToMenu();
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID sach can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.UpdateBookByID(id);
            AskReturnToMenu();
        }
        else if (choice == 5)
        {
            int id;
            cout << "Nhap ID sach can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.DeleteBookByID(id);
            AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::BorrowBookMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY MUON/TRA SACH =========\n";
        cout << "1. Them thong tin muon sach\n";
        cout << "2. Them thong tin tra sach\n";
        cout << "3. Xem tinh trang sach trong thu vien\n";
        cout << "4. Xem sach dang duoc muon boi nguoi dung\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
        {
            HandleBorrowBook();
            AskReturnToMenu();
        }
        else if (choice == 2)
        {
            HandleReturnBook();
            AskReturnToMenu();
        }
        else if (choice == 3)
        {
            bookManager.ShowAllBooks();
            AskReturnToMenu();
        }
        else if (choice == 4)
        {
            char userID_str[10];
            cout << "Nhap ID nguoi dung can xem: ";
            cin.getline(userID_str, 10);
            int userID = Utils::CharArrayToIntManual(userID_str);

            Person* personPtr = userManager.GetUserByID(userID);
            User* user = static_cast<User*>(personPtr);

            if (user)
                user->ShowBorrowedBooks(bookManager);
            else
                cout << "Khong tim thay nguoi dung voi ID: " << userID << endl;

            AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::HandleBorrowBook()
{
    cout << "\n========= XU LY MUON SACH =========\n";
    int userID = 0, bookID = 0;
    char bufferID[20];

    cout << "Nhap ID nguoi dung muon sach: ";
    if (!(cin >> userID)) {
        cout << "Nhap ID nguoi dung khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    cout << "Nhap ID sach can muon: ";
    if (!(cin >> bookID)) {
        cout << "Nhap ID sach khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    User tempUser;
    Utils::IntToStringManual(userID, bufferID, sizeof(bufferID));
    tempUser.LoadUserByID(bufferID);

    if (tempUser.getID() == 0) {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    Book* book = bookManager.GetBookByID(bookID);

    if (!book) {
        cout << "Loi: Khong tim thay sach voi ID: " << bookID << "\n";
        return;
    }

    if (book->getQuantity() <= 0) {
        cout << "Loi: Sach '" << book->getTitle() << "' da het trong kho.\n";
        return;
    }

    if (tempUser.BorrowBook(bookID)) {
        book->setQuantity(book->getQuantity() - 1);
        cout << "MUON SACH THANH CONG!\n";
        cout << "Sach: '" << book->getTitle() << "'. So luong con lai: " << book->getQuantity() << "\n";
        bookManager.SaveBooksToFile();
    }
}

void Admin::HandleReturnBook()
{
    cout << "\n========= XU LY TRA SACH =========\n";
    int userID = 0, bookID = 0;
    char bufferID[20];

    cout << "Nhap ID nguoi dung tra sach: ";
    if (!(cin >> userID)) {
        cout << "Nhap ID nguoi dung khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    cout << "Nhap ID sach can tra: ";
    if (!(cin >> bookID)) {
        cout << "Nhap ID sach khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    User tempUser;
    Utils::IntToStringManual(userID, bufferID, sizeof(bufferID));
    tempUser.LoadUserByID(bufferID);

    if (tempUser.getID() == 0) {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    Book* book = bookManager.GetBookByID(bookID);

    if (tempUser.ReturnBook(bookID)) {
        if (book) {
            book->setQuantity(book->getQuantity() + 1);
            cout << "TRA SACH THANH CONG!\n";
            cout << "Sach: '" << book->getTitle() << "'. So luong hien tai: " << book->getQuantity() << "\n";
            bookManager.SaveBooksToFile();
        } else {
            cout << "TRA SACH THANH CONG.\n";
        }
    } else {
        cout << "Loi: Nguoi dung ID " << userID << " khong muon sach ID " << bookID << "\n";
    }
}

void Admin::Menu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= HE THONG QUAN LY ADMIN =========\n";
        cout << "1. Quan ly nguoi dung\n";
        cout << "2. Quan ly sach\n";
        cout << "3. Quan ly muon/tra sach\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            UserMenu();
        else if (choice == 2)
            BookMenu();
        else if (choice == 3)
            BorrowBookMenu();
        else if (choice == 0)
            cout << "Thoat chuong trinh.\n";
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

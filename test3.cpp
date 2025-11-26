#include<iostream>
using namespace std;
template<class I, class U>
class Exam
{
    I prop_x;
    U prop_y;
};
int main()
{
    Exam<char, char> obj_cc;
    Exam<int, int> obj_ii;
    cout << sizeof(obj_cc) << sizeof(obj_ii);
    return 0;
}
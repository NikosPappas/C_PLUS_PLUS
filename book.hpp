#ifndef BOOK_HP
#define BOOK_HP
#include <iostream>
#include <string>
#include <vector>
class Book{
    private:
        std::string title;
        int pages;
    public:
        Book();
        Book(const std::string&,int);
        void setTitle(const std::string&);
        std::string getTitle(void) const;
        void setPages(int );
        int getPages(void)const;
        void printData()const;

};
#endif

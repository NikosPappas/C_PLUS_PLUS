#include "book.hpp"

Book::Book(){
    title="";
    pages=0;
}
Book::Book(const std::string &utitle,int upages){
    title=utitle;
    pages=upages;
}
void Book::setPages(int upages){
    pages=upages;
}
int Book::getPages(void) const{
    return pages;
}
void Book::setTitle(const std::string &utitle){
    title=utitle;
}
std::string Book::getTitle(void) const{
    return title;
}
void Book::printData(void) const{
    std::cout<<"Title: "<<getTitle()<<std::endl;
    std::cout<<"Pages: "<<getPages()<<std::endl;
}



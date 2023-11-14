#include "book.hpp"

int main(void){
    Book defaultBook;
    std::cout<<"Default constructor invoked."<<std::endl;
    Book myBook{"My Book Title",123};
    std::cout<<"Printing book info..."<<std::endl;
    defaultBook.printData();
    std::cout<<"The book title is: "<<defaultBook.getTitle()<<std::endl;
    std::cout<<"The book pages are: "<<defaultBook.getPages()<<std::endl;
    std::cout<<"Setting the new Title...."<<std::endl;
    myBook.setTitle("New Book Title");
    std::cout<<"Setting the new number of pages..."<<std::endl;
    myBook.setPages(456);
    std::cout<<std::endl<<"Creating an inventory of books..."<<std::endl;
    std::vector<Book> v={
        Book{"Sample Book Title 1",100},
        Book{"Sample Book Title 2",200},
        Book{"Sample Book Title 3",300},
        Book{"Sample Book Title 4",400},
        Book{"Sample Book Title 5",500},
    };
    for(auto index:v){
        index.printData();
    }
    return 0;
}

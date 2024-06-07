#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;

class Shape{
    public:
        double getArea();
};
class Triangle:public Shape{
    private:
        double a;
        double b;
        double c;
    public:
        Triangle(double x,double y,double z){
            assert(x>=0 && x<=y+z);
            assert(y>=0 && y<=z+x);
            assert(z>=0 && z<=y+x);
            a=x;
            b=y;
            c=z;
        }
        double getArea(void) const{
           double s=(a+b+c)/2;
           double t=s*(s-a)*(s-b)*(s-c);
           return sqrt(t);
        }
        
};
class Rectangle:public Shape{
    private:
        double x;
        double y;
    public:
        Rectangle(double xx,double yy){
            assert(xx>=0);
            assert(yy>=0);
            x=xx;
            y=yy;
        };
        void setX(double x){
            this->x=x;
        }
        double getX(void) const{
            return x;
        }
        void setY(double y){
            this->y=y;
        }
        double getY(void) const{
            return y;
        }
        double getArea(void) const{
            double ret=x*y;
            return ret;
        }
};
class Circle:public Shape{
    private:
        double radius;
    public:
        Circle(double r){
            assert(r>=0);
            radius=r;
        };
        void setRadius(double r){
            this->radius=r;
        }
        double getRadius(void) const{
            return this->radius;
        }
        double getArea(void) const{
            const double pi=3.1415;
            double ret=2*pi*radius;
            return ret;
        }
};
void circleArea(void);
void rectangleArea(void);
void triangleArea(void);
int main(int argc,char *argv[]){
    while(true){
        cout<<"Welcome to the super area calculator"<<endl;
        cout<<"1.Calculate the Area of a Circle"<<endl;
        cout<<"2.Calculate the Area of a Rectangle"<<endl;
        cout<<"3.Calculate the Area of a Triangle"<<endl;
        cout<<"4.Quit"<<endl;
        int input;
        cin>>input;
        switch(input){
            case 1:
                circleArea();
                break;
            case 2:
                rectangleArea();
                break;
            case 3:
                triangleArea();
                break;
            case 4:
                exit(0);
                break;
            default:
                cout<<"Error"<<endl;
                break;
        }
    }
    return 0;
}
void triangleArea(){
    double a;
    double b;
    double c;
    cout<<"Type the first side: ";
    cin>>a;
    cout<<"Type the second side: ";
    cin>>b;
    cout<<"Type the third size: ";
    cin>>c;
    Triangle t{a,b,c};
    cout<<"The triangle area is "<<t.getArea()<<endl;
}
void rectangleArea(){
    double x;
    double y;
    cout<<"Type the length: ";
    cin>>x;
    cout<<"Type the widht: ";
    cin>>y;
    Rectangle r{x,y};
    cout<<"The rectangle area is "<<r.getArea()<<endl;
}
void circleArea(){
    double radius;
    cout<<"Type the radius: ";
    cin>>radius;
    Circle c{radius};
    cout<<"The circle area is "<<c.getArea()<<endl;
}

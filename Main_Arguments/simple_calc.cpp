#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
using namespace std;

class Number{
	private:
		double n;
	public:
		Number():n(0){};
		Number(double x):n(x){};
	        void set_number(double x){
			n=x;
		}
		double get_number(void){
			return n;
		}
};
class Calculator{
	private:
		Number n1;
		Number n2;
	public:
		Calculator(){
			n1.set_number(0);
			n2.set_number(0);
		}
		Calculator(double x,double y){
			n1.set_number(x);
			n2.set_number(y);
		}
		double sum(Number n,Number y){
			double sum=0.0;
                        sum=n.get_number()+y.get_number();
			return sum;
		}
		double diff(Number n,Number y){
			return n.get_number()-y.get_number();
		}
		double product(Number n,Number y){
			return n.get_number()*y.get_number();
		}
		double div(Number n,Number y){
			assert(y.get_number()!=0.0);
			return n.get_number()/y.get_number();
		}
};
int main(int argc,char *argv[]){
	if(argc!=4){
		cout<<"Usage ./a.out number_1 operator number_2"<<endl;
		return -1;
	}
	Number n1;
	Number n2;
	Calculator c;
	n1.set_number(atof(argv[1]));
	n2.set_number(atof(argv[3]));
	if(strcmp(argv[2],"+")==0){
		cout<<"The sum is "<<c.sum(n1,n2)<<endl;
	}
	else if(strcmp(argv[2],"-")==0){
		cout<<"Teh difference is "<<c.diff(n1,n2)<<endl;
	}
	else if(strcmp(argv[2],"x")==0){
		cout<<"The product is "<<c.product(n1,n2)<<endl;
	}
	else if(strcmp(argv[2],"/")==0){
		cout<<"The division is "<<c.div(n1,n2)<<endl;
	}
	else{
		cout<<"Bye Bye"<<endl;
		return 0;
	}
	return 0;
}

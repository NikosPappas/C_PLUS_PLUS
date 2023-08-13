#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdbool>
using namespace std;
int main(int argc,char *argv[]){

	double amount,betting_amount;
	int user_guess,roulete_guess;
	string answer;
	bool cont=true;
	cout<<"Type your amount please: "<<endl;
	cin>>amount;
	while(cont){

		cout<<"Enter money to bet: ";
		cin>>betting_amount;
		if(betting_amount>amount){
			cout<<"Betting amount is greater than total amount"<<endl;
			exit(1);
		}
		cout<<"Guess a number please: ";
		cin>>user_guess;
		roulete_guess=rand()%10+1;
		if(user_guess==roulete_guess){
			cout<<"Congratulations.You won."<<endl;
			amount=amount+betting_amount*10;
		}
		else{
			cout<<"Sorry.Next time"<<endl;
			amount=amount-betting_amount;
		}
		if(amount==0)
			break;
		else{
			cout<<"Continue?(y/n)";
			cin>>answer;
			if(answer=="y")
				cont=true;
			else
				cont=false;
		}
		
	}

	cout<<"Total amount of money: "<<amount<<endl;
	return 0;
}

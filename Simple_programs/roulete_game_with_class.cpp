#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

class Person{
	protected:
		string name;
	public:
		Person():name(""){};
		Person(string s):name(s){};
		void set_name(string s){
			name=s;
		}
		string get_name(void){
			return name;
		}
};
class Player:public Person{
	private:
		string message;
		double amount;
	public:
		Player():amount(0.0),Person(){};
		Player(string s,double d,string m):amount(d),message(m),Person(s){};
		void set_amount(double d){
			amount=d;
		}
		double get_amount(void){
			return amount;
		}
		string get_message(void){
			return message;
		}
};
class Dealer:public Person{
	private:
		string message;
	public:
		Dealer():message(""),Person(){};
		Dealer(string m,string s):message(m),Person(s){};
		void set_message(string s){
			message=s;
		}
		string get_message(void){
			return message;
		}
};
class Game{
	protected:
		string game_name;
	public:
		Game():game_name(""){};
		Game(string s):game_name(s){};
		void set_game_name(string s){
			game_name=s;
		}
		string get_name_game(void){
			return game_name;
		}
};
class Straight:public Game{
	private:
		string description;
	public:
		Straight():description(""),Game(){};
		Straight(string s,string name):description(s),Game(name){};
	        void set_description(string s){
			description=s;
		}
		string get_description(void){
			return description;
		}
};
int main(int argc,char *argv[]){
	Dealer d("Bet your amount on number  please ","Dealer");
	Player p("Player",5000.00,"I bet ");
	Straight s("Bet on a single number between 0 and 36","Straight");
	string answer;
	double number;
	double bet_amount;
	bool ok=true;
	int roullete_number;
	srand(time(NULL));
	cout<<s.get_description()<<endl;
	do{
		cout<<d.get_message()<<"on number: "<<endl;
		cin>>bet_amount>>number;
		if(bet_amount>p.get_amount()){
				cout<<"Cannot bet more money than you have"<<endl;
				ok=false;
		}
                if(number<0 || number>36){
			cout<<"Number is not between 0 and 36"<<endl;
			ok=false;
		}
		roullete_number=rand()%35+1;
		if(number==roullete_number){
			cout<<"You win."<<endl;
			p.set_amount(p.get_amount()+p.get_amount()*10);
		}
		else{
			cout<<"You lost "<<bet_amount<<endl;
			p.set_amount(p.get_amount()-bet_amount);
		}
		cout<<"Do you wish to continue: (y/n)?";
		cin>>answer;
		if(answer=="y")
			ok=true;
		else
			ok=false;
	}while(ok);

        cout<<"Total player's amount: "<<p.get_amount()<<endl;

	return 0;
}

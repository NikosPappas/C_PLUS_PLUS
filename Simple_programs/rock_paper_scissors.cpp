#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Shape{
	private:
		string name;
	public:
		Shape():name(""){};
		Shape(string s){
			name=s;
		}
};
class Rock:public Shape{
	private:
		string ss;
	public:
		Rock():Shape(){};
		Rock(string s):Shape(s){};
		string get_name(){
			return ss;
		}
		void set_name(string s){
			ss=s;
		}
};
class Paper:public Shape{
	private:
		string sp;
	public:
		Paper():Shape(){};
		Paper(string s):sp(s),Shape(s){};
		string get_name(void){
			return sp;
		}
		void set_name(string s){
			sp=s;
		}
		
};
class Scissors:public Shape{
	private:
		string scs;
	public:
		Scissors():Shape(){}
		Scissors(string s):scs(s),Shape(s){};
		string get_name(void){
			return scs;
		}
		void set_name(string s){
			scs=s;
		}


};
class Player{
	private:
		string name;
	public:
		Player(string s){
			name=s;
		r.set_name(s+" rock");
		p.set_name(s+" papper");
		sc.set_name(s+" scissors");
		}
		Rock r;
		Paper p;
		Scissors sc;
		string get_name(void){
			return name;
		}

};
int main(int argc,char *argv[]){
	Player user("User");
	Player pc("Pc");
	int loop=3;
	int user_points=0;
	int pc_points=0;
	int user_input;
	int pc_random_number;
	srand(time(NULL));
	for(int i=0;i<loop;i++){
		cout<<"Type 1 for rock,2 for papper,3 scissors"<<endl;
		cin>>user_input;
		pc_random_number=rand()%3+1;
		switch(pc_random_number){
			case 1://rock
				if(user_input==1){
					cout<<user.r.get_name()<<" equals "<<pc.r.get_name()<<endl;
				}
				else if(user_input==2){
					cout<<user.p.get_name()<<" wins "<<pc.r.get_name()<<endl;
					user_points++;
				}
				else{
					cout<<user.sc.get_name()<< " loses from "<<pc.sc.get_name()<<endl;
					pc_points++;
				}
				break;
			case 2://papper
			       if(user_input==1){
				       cout<<user.r.get_name()<<" loses from "<<pc.p.get_name()<<endl;
				       pc_points++;
			       }
			       else if(user_input==2){
				       cout<<user.p.get_name()<<" equals "<<pc.p.get_name()<<endl;
			       }
			       else{
				       cout<<user.sc.get_name()<<" wins to"<<pc.p.get_name()<<endl;
				       user_points++;
			       }
			       break;
			 case 3://scissors
				if(user_input==1){
					cout<<user.r.get_name()<<" wins to "<<pc.sc.get_name()<<endl;
					user_points++;
				}
				else if(user_input==2){
                                        cout<<user.p.get_name()<<" loses to "<<pc.sc.get_name()<<endl;
					pc_points++;
				}
				else{
					cout<<user.sc.get_name()<<" equals "<<pc.sc.get_name()<<endl;

				}
                               break;

		}

	}
	if(user_points>pc_points){
		cout<<"User wins."<<endl;
	}
	else if(user_points<pc_points){
		cout<<"Computer wins."<<endl;
	}
	else{
		cout<<"Equal points"<<endl;
	}
	cout<<"Total score: "<<endl;
	cout<<"User: "<<user_points<<endl;
	cout<<"Pc:   "<<pc_points<<endl;
	return 0;
}

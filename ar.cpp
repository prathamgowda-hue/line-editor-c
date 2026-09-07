#include<iostream>
#include<iomanip>
#include<string>
using namespace std;
class bank{
    private:
        string number;
        string name;
        double balance;

        public:
          bank()
          {
            number=" ";
            name=" ";
            balance=0.0;
          }
          bank(string n,string na,double b)
          { 
            number=n;
            name=na;
            balance=b;
          }
          ~bank()
          {}

          void deposit(double amount)
          {
            if(amount>0)
            {
                balance+=amount;
                cout<<"Deposited:$"<<fixed<<setprecision(2)<<amount<<endl
                <<"New Balance:$"<<fixed<<setprecision(2)<<balance<<endl;
            }
            else
            {
                cout<<"Invalid deposit amount."<<endl;
            }
          }

          void withdraw(double amount)
          {
            if(amount<=0)
            {
                cout<<"Invalid withdrawaal amount."<<endl;
            }
            else if(amount>balance)
            {
                cout<<"Insufficient funds for withdrawal."<<endl;
            }
            else
            {
                balance-=amount;
                cout<<"Withdrawn:$"<<fixed<<setprecision(2)<<amount
                <<endl<<"New balance:$"<<fixed<<setprecision(2)<<balance<<endl;
            }
          }

          void display()const
          {
            cout<<"Account number:"<<number<<endl;
            cout<<"Account holder name:"<<name<<endl;
            cout<<"Account balance:$"<<fixed<<setprecision(2)<<balance<<endl;
          }
        };
        int main()
        {
            int n;
            cout<<"Enter the number of accounts to create:";
            cin>>n;
            bank* accounts=new bank[n];

            for(int i=0;i<n;i++)
            {
                string n;
                string na;
                double b;
                cout<<"Enter account number for account "<<i+1<<":";
                cin>>n;
            }
        }

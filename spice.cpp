#include <vector>
#include <sstream>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std; 

int MOSFET_number = 0;
int intermediate_wire = 0; 

int op; 
/* op = 1: NOR */
/* op = 2: NAND */
/* op = 3: NOT */
                                //////functions signatures/////
void NOR(char &in1, char &in2, char &out);//function executes the nor by caling the right PDN and PUP functions
void NAND(char &in1, char &in2, char &out);//function executes the nand by caling the right PDN and PUP functions
void NOT(char &in1, char &out);//function executes the not by caling the right PDN and PUP functions
void PDN(char &in1, char &in2, char &out);//function executes the  PDN 
void PUN(char &in1, char &in2, char &out);////function executes the  PDN 
string to_postfix(string & infix);
bool a_perceed_b(char & a, char & b);//to keep the execution based on the priority 

int main()

{
	string in_usr, expression, postfix, net_list = "";
    cout<<"enter the boolean expression"<<endl;
	getline(cin, in_usr); //to get the entered terms 
	postfix = to_postfix(in_usr); 

    cout<<"The data statment "<<endl;

	int str_flag = postfix.length(); //to loop over all the entered terms 
	for(int i=0; i<str_flag;i++)
	{
		if (postfix[i] == '&') //check for and operation 
		{
			op = 2;
			char temp = ' '; 
			char out; 
			NAND(postfix[i - 2], postfix[i - 1], temp);
			op = 3;
			NOT(temp, out);
			string rep(1, out);
			postfix.replace(i - 2, i+1, rep);

		}
		if (postfix[i] == '|') //check for or 
		{
            op = 1;
			char temp = ' '; 
			char out; 
			NOR(postfix[i - 2], postfix[i - 1], temp);
			op = 3;
			NOT(temp, out);
			string rep(1, out);
			postfix.replace(i - 2, i+1, rep);
		}
		if (postfix[i] == '`') //check for not
		{
            char temp = ' '; 
			char out; 
			op = 3;
			NOT(postfix[i - 1], out);
			string rep(1, out);
			postfix.replace(i - 2, i+1, rep);

		}
	}


	system("pause"); 
	return 0; 
}

void NOR (char &in1, char &in2, char &out)
{
	PUN(in1, in2, out);
	PDN(in1, in2, out);
	op = 0; 
}

void NAND (char &in1, char &in2, char &out)
{
	PUN(in1, in2, out);
	PDN(in1, in2, out);
	op = 0;
}

void NOT(char &in1, char &out)
{
	char GND = '0'; 
	PUN(in1, GND, out);
	PDN(in1, GND, out);
	op = 0;
}

void PDN (char &in1, char &in2, char &out)//executes each operation PDN part
{ 
	if (op == 1)   //NOR
	{   
		MOSFET_number++; ///to add the new mosfts and keep track of their count to print the correct connections
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
		MOSFET_number++; 
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in2 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
	}
	if (op == 2)   //NAND
	{
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire++ << "  " << in1 << "  " << intermediate_wire  << "  " << intermediate_wire  << "  NMOS" << endl;
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in2 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
	}
	if (op == 3)   //NOT
	{
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
	}
}


void PUN (char &in1, char &in2, char &out)//executes each operation PUN part//the addition is done PUN to obtain the correct output based on the module diagram
{
	if (op == 1)   //NOR
	{   MOSFET_number++; ///to add the new wires, which can be the output, and keep track of their count to print the correct connections
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
		MOSFET_number++;
		intermediate_wire++;
        out=intermediate_wire+48;
        /// the -- in the intermediate_wire below affects the next intermediate_wire
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire--<< "  " << in2 << "  " << intermediate_wire << "  " << intermediate_wire << "  PMOS" << endl;
	        intermediate_wire++; ///to get the correct char according to the ascii table 

    }
	if (op == 2)   //NAND
	{
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in2 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
        out=intermediate_wire+48; ///to get the correct char according to the ascii table 

    }
    
	if (op == 3)   //NOT
	{
		MOSFET_number++;
		intermediate_wire++;
        out=intermediate_wire+48; ///to get the correct char according to the ascii table 
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
	}
}

string to_postfix(string & infix)//function to store and sort the input trems correctly based on their priority
{
	string postfix;
	char top = 0;
	stack <char> Stack1;
	for (unsigned int i = 0; i < infix.length(); i++)
	{
		if (infix[i] == '(')
			Stack1.push(infix[i]);
		else if (infix[i] == ')' && !Stack1.empty())
		{
			top = Stack1.top();
			Stack1.pop();
			while (top != '(' && !Stack1.empty())
			{
				postfix += top;
				top = Stack1.top();
				Stack1.pop();
			}
		}
		else if (infix[i] == '`' || infix[i] == '&' || infix[i] == '|')
		{
			if (!Stack1.empty())
				top = Stack1.top();
			while (!Stack1.empty() && a_perceed_b(top, infix[i]))
			{
				top = Stack1.top();
				Stack1.pop();
				postfix += top;
				if (!Stack1.empty())
					top = Stack1.top();
			}
			Stack1.push(infix[i]);
		}
		else
			postfix += infix[i];
	}
	while (!Stack1.empty())
	{
		top = Stack1.top();
		Stack1.pop();
		postfix += top;
	}
	return postfix;
}
bool a_perceed_b(char & a, char & b)//function to make sure that expressions are executed with respect to their proprty 
{
	bool flag = false;

	
	if (a == '`' &&  b == '&' ||
		a == '`' &&  b == '|' ||
		a == '&' &&  b == '|'
		)
		flag = true;

	return flag;
}

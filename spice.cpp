#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <stack>

using namespace std;

int MOSFET_number = 0;
int intermediate_wire = 0; 
int intermediate_out = 0;
int op; 
/* op = 1: NOR */
/* op = 2: NAND */
/* op = 3: NOT */

void NOR(char &in1, char &in2, char &out);
void NAND(char &in1, char &in2, char &out);
void NOT(char &in1, char &out);
void PDN(char &in1, char &in2, char &out);
void PUN(char &in1, char &in2, char &out);
string to_postfix(string & infix);
bool a_perceed_b(char & a, char & b);

int main()
{
	string in_usr, expression, postfix, net_list = "";
	getline(cin, in_usr);
	postfix = to_postfix(in_usr);
	cout << postfix << endl;
	//postfix.replace(0, 3, "israa");

	int str_flag = postfix.length(); 
	for(int i=0; i<str_flag;i++)
	{
		if (postfix[i] == '&')
		{
			op = 2;
			char temp = ' '; 
			char out; 
			NAND(postfix[i - 2], postfix[i - 1], temp);
			op = 3;
			NOT(temp, out);
			string rep(1, out);
			postfix.replace(i - 2, i+1, rep);
			cout << temp << endl; 
		}
		if (postfix[i] == '|')
		{
		}
		if (postfix[i] == '`')
		{

		}
	}

	cout << postfix << endl; 

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

void PDN (char &in1, char &in2, char &out)
{ 
	if (op == 1)   //NOR
	{
		MOSFET_number++; 
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in1 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
		MOSFET_number++; 
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in2 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
	}
	if (op == 2)   //NAND
	{
		MOSFET_number++;
		intermediate_wire++; 
		intermediate_out++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in1 << "  " << intermediate_wire << "  " << intermediate_wire << "  NMOS" << endl;
		MOSFET_number++;
		intermediate_out++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in2 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
	}
	if (op == 3)   //NOT
	{
		MOSFET_number++;
		intermediate_wire++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "0" << "  " << "0" << "  NMOS" << endl;
	}
}

void PUN (char &in1, char &in2, char &out)
{
	if (op == 1)   //NOR
	{
		MOSFET_number++;
		intermediate_wire++;
		cout << " M" << MOSFET_number << "  " << intermediate_wire << "  " << in1 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  " << out << "  " << in2 << "  " << intermediate_wire << "  " << intermediate_wire << "  PMOS" << endl;
	}
	if (op == 2)   //NAND
	{
		MOSFET_number++;
		intermediate_wire++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in2 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
	}
	if (op == 3)   //NOT
	{
		MOSFET_number++;
		intermediate_wire++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_wire << "  " << in1 << "  " << "VDD" << "  " << "VDD" << "  PMOS" << endl;
	}
}

string to_postfix(string & infix)  
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


bool a_perceed_b(char & a, char & b)
{
	bool flag = false;

	if (a == '`' &&  b == '&' ||
		a == '`' &&  b == '|' ||
		a == '&' &&  b == '|'
		)
		flag = true;

	return flag;
}

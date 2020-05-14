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

void NOR(string &in1, string &in2, string &out);
void NAND(string &in1, string &in2, string &out);
void NOT(string &in1, string &out);
void PDN(string &in1, string &in2, string &out);
void PUN(string &in1, string &in2, string &out);
string to_postfix(string & infix);
bool a_perceed_b(char & a, char & b);

int main()
{
	string in_usr, expression, postfix;
	getline(cin, in_usr);
	postfix = to_postfix(in_usr);

	for (int i = 0; i < postfix.length(); i++)
	{
		if (postfix[i] == '&')
		{
			string temp = " ", out = " ";
			string in1(1, postfix[i - 2]);
			string in2(1, postfix[i - 1]);
			op = 2;
			NOR(in1, in2, temp);
			op = 3;
			NOT(temp, out);
			postfix.replace(i - 2, i + 1, out);
		}
		if (postfix[i] == '|')
		{
			string temp = " ", out = " ";
			string in1(1, postfix[i - 2]);
			string in2(1, postfix[i - 1]);
			op = 1;
			NOR(in1, in2, temp);
			op = 3;
			NOT(temp, out);
			postfix.replace(i - 2, i + 1, out);
		}
		if (postfix[i] == '`')
		{
			string out = " ";
			string in1(1, postfix[i - 1]);
			op = 3;
			NOT(in1, out);
			postfix.replace(i - 1, i + 1, out);
		}
	}

	system("pause");
	return 0;
}


void NOR(string &in1, string &in2, string &out)
{
	PUN(in1, in2, out);
	PDN(in1, in2, out);
	op = 0;
}

void NAND(string &in1, string &in2, string &out)
{
	PUN(in1, in2, out);
	PDN(in1, in2, out);
	op = 0;
}

void NOT(string &in1, string &out)
{
	string GND = "0";
	PUN(in1, GND, out);
	PDN(in1, GND, out);
	op = 0;
}

void PDN(string &in1, string &in2, string &out)
{
	if (op == 1)   //NOR
	{
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in1 << "  " << "0" << "  "
		<< "0" << "  NMOS" << endl;
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in2 << "  " << "0" << "  " 
		<< "0" << "  NMOS" << endl;
		out = 'Y' + to_string(intermediate_out);
	}
	if (op == 2)   //NAND
	{
		MOSFET_number++;
		intermediate_wire++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in1 << "  " << intermediate_wire 
		<< "  " << intermediate_wire << "  NMOS" << endl;
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  " << intermediate_wire << "  " << in2 << "  " << "0" << "  " << "0" 
		<< "  NMOS" << endl;
		out = 'Y' + to_string(intermediate_out);
	}
	if (op == 3)   //NOT
	{
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in1 << "  " << "0" << "  " << "0" 
		<< "  NMOS" << endl;
		out = 'Y' + to_string(intermediate_out);
	}
}

void PUN(string &in1, string &in2, string &out)
{
	if (op == 1)   //NOR
	{
		MOSFET_number++;
		intermediate_wire++;
		cout << " M" << MOSFET_number << "  " << intermediate_wire << "  " << in1 << "  " << "VDD" << "  " 
		<< "VDD" << "  PMOS" << endl;
		MOSFET_number++;
		intermediate_out++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in2 << "  " << intermediate_wire 
		<< "  " << intermediate_wire << "  PMOS" << endl;
	}
	if (op == 2)   //NAND
	{
		MOSFET_number++;
		intermediate_out++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in1 << "  " << "VDD" << "  " 
		<< "VDD" << "  PMOS" << endl;
		MOSFET_number++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in2 << "  " << "VDD" << "  " 
		<< "VDD" << "  PMOS" << endl;
	}
	if (op == 3)   //NOT
	{
		MOSFET_number++;
		intermediate_out++;
		cout << " M" << MOSFET_number << "  Y" << intermediate_out << "  " << in1 << "  " << "VDD" << "  "
		<< "VDD" << "  PMOS" << endl;
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

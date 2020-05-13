#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <stack>
using namespace std; 

string to_postfix(string & infix)
{
	string postfix;
	char top = 0;
	stack <char> Stack1;

	for (int i = 0; i < infix.length(); i++)
	{
		if (infix[i] == '(')
			Stack1.push(infix[i]);
		else if (infix[i] == ')' && !Stack1.empty())
		{
			top = Stack1.top();
			Stack1.pop();
			while ( top != '(' && !Stack1.empty() )
			{
				postfix += top;
				top = Stack1.top();
				Stack1.pop();
			}
		}
		else if (infix[i] == '\'' || infix[i] == '&' || infix[i] == '|')
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

	if (a == '\'' &&  b == '&' ||
		a == '\'' &&  b == '|' ||
		a == '&' &&  b == '|'
		/*||
		a == '\'' &&  b == '\'' ||
		a == '&' &&  b == '&'   ||
		a == '|' &&  b == '|'
		*/
		)
		flag = true;

	return flag;
}

int main()

{


return 0; 
}

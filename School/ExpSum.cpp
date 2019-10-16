//一个能处理四则运算的程序，实现语言C++，支持嵌套括号，可以处理实数，源码见下面：
#include "stdafx.h"
#include "mylib.h"
#include <iostream>
#include <cmath>
using namespace std;
#define MAX EXPLEN

/*欢迎模块*/
class Entry
{
public:
	static void welcome()
	{
		;
	}
};

/*输入模块*/
class Input
{
public:
	Input()
	{
		for (int i = 0; i < MAX; i++)
			Str_input[i] = '\0';
	}
	char Str_input[MAX];
	void inStr()
	{
		cin >> Str_input;
	}
};

/*输出模块*/
class Output
{
public:
	Output()
	{
		result = 0;
	}
	void getRes(double res)
	{
		result = res;
	}
	void printRes()
	{
		printf("%lf", result);
	}
private:
	double result;
};

/*计算用的存储结构*/
template <class Type>
class STACK {                 //定义栈类
private:
	Type base[MAX];
	int Size;
public:
	STACK() { Size = 0; };
	void push(Type a)     //入栈
	{
		base[Size] = a;
		Size++;
	}
	Type pop()            //出栈
	{
		return base[--Size];
	}
	int size()
	{
		return Size;
	}
};


/*计算的模块*/
class Calculate_Cla
{
public:
	bool IsData(char);
	bool IsSym(char);
	int IsPar(char);
	bool Check(char *);
	int setPri(char);                 //判断符号的优先极别
	double ToData(char*);               //把字符串转化为数值
	double Call(double, double, char);    //具体按符号计算
	int GetMatch(char* buffer, int pos); //利用栈找到匹配的括号 
	void Opr(STACK<char>&, STACK<double>&, int&); //利用栈计算
	double Calculate(char*, double&);   //字符串的读入及调配

};
bool Calculate_Cla::IsData(char ch)      //判断输入计算的数字是否为0-9
{
	return ((ch >= '0'&&ch <= '9') || ch == '.') ? true : false;
}
bool Calculate_Cla::IsSym(char ch)      //判断是否输入非法运算符
{
	return (ch == '+' || ch == '-' || ch == '*' || ch == '/') ? true : false;
}
int Calculate_Cla::IsPar(char ch)
{
	if (ch == '(')
		return 1;
	if (ch == ')')
		return -1;
	return 0;
}
bool Calculate_Cla::Check(char *ch)
{
	int a = 0;
	for (int i = 0; i < strlen(ch); i++)
		if (ch[i] == '.')
			a++;
	if (a > 1)
		return false;
	return true;
}
int Calculate_Cla::setPri(char ch)          //符号的优先极别
{
	switch (ch)
	{
	case '+':
		return 0;
	case '-':
		return 0;
	case '*':
		return 1;
	case '/':
		return 1;
	default:
		return -1;
	}
}
double Calculate_Cla::ToData(char* ch)   //将数字转化为数值 
{
	int i, j, sumn = 0;
	double sum = 0.0;
	if (!Check(ch)) return 0.0;
	for (i = 0; i < strlen(ch); i++)             //读入整数部分 
	{
		if (ch[i] != '.')
			sumn = sumn * 10 + (ch[i] - '0');
		else break;
	}
	if (i < strlen(ch))
		for (j = i + 1; j < strlen(ch); j++)        //小数部分 
			sum = sum * 10 + (ch[j] - '0');
	sum /= pow(10.0, (double)(strlen(ch) - 1 - i));
	return (sum + sumn);                      //返回值 
}
double Calculate_Cla::Call(double sum, double data, char ch)
{
	double ans = 0.0;
	switch (ch)
	{
	case '+':
		ans = sum + data;
		break;
	case '-':
		ans = sum - data;
		break;
	case '*':
		ans = sum * data;
		break;
	case '/':
		if (data != 0.0)
			ans = sum / data;
		else
		{
			return INFINITY;
		}
		break;
	default:ans = 0.0;
		break;
	}
	return ans;
}
int Calculate_Cla::GetMatch(char* buffer, int pos)     //利用栈找到匹配的括号 
{
	STACK<char> Temp;
	int i;
	for (i = pos; i < strlen(buffer); i++)
	{
		if (IsPar(buffer[i]) == 1)
			Temp.push('0');
		if (IsPar(buffer[i]) == -1)
		{
			Temp.pop();
			if (Temp.size() == 0) return i;
		}
	}
	return -1;
}
void Calculate_Cla::Opr(STACK<char>& symbol, STACK<double>& data, int& mark)
{
	double sum;
	while (symbol.size() != 0)
	{
		char tem = symbol.pop();
		int temp = setPri(tem);
		symbol.push(tem);
		if (temp < mark)
			break;
		else {
			sum = Call(data.pop(), data.pop(), symbol.pop());
			data.push(sum);
		}
	}
}
double Calculate_Cla::Calculate(char* buffer, double& sum)   //字符串读入和各个函数调配
{
	STACK<double> data;
	STACK<char> symbol;
	double ans;
	char temp[MAX];
	int ct = 0, mark = 0, tp = 0;
	data.push(sum);
	while (ct <= strlen(buffer))
	{
		if (IsData(buffer[ct]))            //如果是数字或小数点 
		{
			while (ct < strlen(buffer) && IsData(buffer[ct]))
				temp[tp++] = buffer[ct++];
			temp[tp] = '\0';
			tp = 0;                         //读到非数字也非小数为止 
			ans = ToData(temp);             //把读到的字符串转化为数 
			data.push(ans);

			if (ct == strlen(buffer))        //已经独到字符串末尾 
			{
				mark = 0;
				Opr(symbol, data, mark);    //计算
				sum = data.pop();           //此时data栈中还剩一个数据，即是结果 
				return sum;               //返回结果 
			}
			else {
				int mark = setPri(buffer[ct]);
				Opr(symbol, data, mark);     //计算 
			}
		}
		else if (IsSym(buffer[ct]))         //如果是运算符 
			symbol.push(buffer[ct++]);     //运算符入symbol栈 
		else
		{
			char BF[100]; int k = 0;          //如果都不是，则只能是括号
			while (IsPar(buffer[ct]) != 1 && ct <= strlen(buffer))
				BF[k++] = buffer[ct++];
			BF[k] = '\0';
			if (IsPar(buffer[ct]) == 1)       //一旦读到左括号，寻找它匹配的右括号 
			{
				int i, j;
				char Temp[100];
				for (i = ct + 1, j = 0; i < GetMatch(buffer, ct); i++, j++)
					Temp[j] = buffer[i];     //把这对括号中的字符串存入Temp 
				Temp[j] = '\0';
				data.push(Calculate(Temp, sum)); //递归调用Calculate直到没有括号 
						   //然后开始计算，值层层返回最后将最终结果放入data栈 
				ct += (strlen(Temp) + 1);       //跳过已经处理完的字符 
				if (ct + 1 == strlen(buffer))    //这里考虑字符串以括号结尾的情况 
				{
					mark = 0;
					Opr(symbol, data, mark);
					sum = data.pop();
					return sum;
				}
				else
				{
					mark = setPri(buffer[ct + 1]); //不是的话继续计算 
					Opr(symbol, data, mark);
				}
				ct++;                           //读入下一个字符 
			}
		}
	}
	return 0.;
}


/*检查输入表达式正确性模块*/
class CheckStr
{
public:
	static int check(char *str)
	{
		int i;
		STACK<char> Temp;
		for (i = 0; i < strlen(str); i++)
		{
			char t = str[i];
			if (!((int(str[i]) <= 57 && int(str[i]) >= 48) || str[i] == '(' || str[i] == ')' || str[i] == '*'
				|| str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '.'))       //检测是否含有非法字符
				return 2;
			//------------除0检测------------
			char *rel = strstr(str,"/0"); //首次出现地址，strstr保存的是ddabc
			if (rel != NULL)
				return 2;         //根据返回子字符串匹配结果输出索引位
			//------------除0检测------------
			else if (str[i] == '(')
				Temp.push('0');
			else if (str[i] == ')')
			{
				if (Temp.size() <= 0)                                      //检测括号是否匹配，右括号是否过多
					return 1;
				else
					Temp.pop();
			}
		}
		if (Temp.size() != 0)                                                //检测括号是否匹配，左括号是否过多
			return 1;
		return 0;
	}
};

double ExpSum(char * Expression)
{
	Entry::welcome();                           //欢迎模块
	double sum = 0.0;
	cout.precision(12);

	Input in;
	Calculate_Cla cl;
	Output out;

	while (1)
	{
		int res = CheckStr::check(Expression); //判断模块 
		if (res == 0)
			break;
		else if (res == 1)
			return FERROR;
		else if (res == 2)
			return FERROR;
		else
		{
		}
	}
	out.getRes(cl.Calculate(Expression, sum)); //计算模块
	//out.printRes();                          //输出模块 
	return sum;
}
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

const int MAX = 20;

/////////////////////////////////////////////////////////////////////
//Алгоритм "решето Сундарама". Выбирает все простые числа
//до заданного (случайно сгенерированного).
int sundaram(int n)
{
	int *a = new int[n], i, j, k;
	memset(a, 0, sizeof(int) * n);
	for (i = 1; 3 * i + 1 < n; i++)
	{
		for (j = 1; (k = i + j + 2 * i*j) < n && j <= i; j++)
			a[k] = 1;
	}
	//Выбирает из списка простых чисел ближайшее к заданному.
	for (i = n - 1; i >= 1; i--)
		if (a[i] == 0)
		{
			return (2 * i + 1);
			break;
		}
	delete[] a;
}
/////////////////////////////////////////////////////////////////////
//Алгоритм Евклида. Алгоритм для нахождения наибольшего
//общего делителя двух целых чисел. Используется для проверки
//чисел на взаимопростоту.
int gcd(int a, int b)
{
	int c;
	while (b)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return abs(a);
}

int main()
{
	cout << "Please wait... Key generation procces." << endl << endl;
	// Генерация двух чисел и выбор двух простых чисел.
	int p = rand() % 100;
	int q = rand() % 100;
	int p_prime = sundaram(p);
	int q_prime = sundaram(q);
	p_prime = 19;
	q_prime = 29;
	cout << "Prime p = " << p_prime << endl;
	cout << "Prime q = " << q_prime << endl;


	//Находим число n.
	unsigned int n = p_prime*q_prime;
	cout << "n = " << n << endl;


	//Генерация числа d и проверка его на взаимопростоту
	//с числом ((p_simple-1)*(q_simple-1)).
	int d, d_simple = 0;
	while (d_simple != 1)
	{
		d = rand() % 100;
		d_simple = gcd(d, ((p_prime - 1)*(q_prime - 1)));
	}
	cout << "d = " << d << endl;


	//Определение числа e, для которого является истинным
	//соотношение (e*d)%((p_simple-1)*(q_simple-1))=1.
	unsigned int e = 0, e_simple = 0;
	while (e_simple != 1)
	{
		e += 1;
		e_simple = (e*d) % ((p_prime - 1)*(q_prime - 1));
	}
	cout << "e = " << e << endl;


	//Сгенерированные ключи.
	cout << '{' << setw(12) << e << ',' << setw(12) << n << '}' << " - Open key" << endl;
	cout << '{' << setw(12) << d << ',' << setw(12) << n << '}' << " - Secret key" << endl << endl;


	//Ввод шифруемых данных.
	string message = "";
	char tmp = ' ';
	cout << "Enter message(CTRL+Z, Enter for end): ";
	while (true)
	{
		tmp = getchar();
		if (tmp != 26 && tmp != -1)
			message += tmp;
		else
			break;
	}
	const int size = message.size();

	string message_encrypt = "";
	string message_decrypt = "";

	int c;
	vector<int> codes_encr;
	for (int j = 0; j < size; ++j)
	{
		c = 1;
		unsigned int i = 0;
		while (i < e)
		{
			c = c*message[j];
			c = c%n;
			++i;
		}
		message_encrypt += char(c);
		codes_encr.push_back(c);
	}
	cout << "Encrypted message: " << message_encrypt << endl;

	cout << "Encrypted codes" << endl;
	for (int x : codes_encr)
	{
		cout << " " << x << " ";
	}

	int m;
	for (int j = 0; j < size; ++j)
	{
		m = 1;
		unsigned int i = 0;
		while (i < d)
		{
			m = m*codes_encr[j];
			m = m%n;
			++i;
		}
		message_decrypt += char(m);
	}
	cout << "\nDecrypted message: " << message_decrypt << endl;

	system("pause");
	return 0;
}


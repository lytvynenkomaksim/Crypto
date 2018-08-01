#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>



using namespace std;
static const int KEY_LENGHT = 256 / 8;
static const int MESSAGE_BLOCK_SIZE = 64;
static const int KEY_BLOCK_SIZE = 32;
static const int SHIFT = 11;

// Dividing message to 64 bit blocks, add 0 to last block if needed
vector<string> toBlocks(string message, double block_size)
{
	string temp = "";
	int count = 0;
	unsigned int i = 0; // number of chars in message
	if (message.size() > 0)
	{
		vector<string> message_blocks(ceil(message.size() / block_size));
		for (unsigned int j = 0; j < message_blocks.size(); ++j)
		{
			count = 0;
			temp = "";
			while (count < block_size)
			{
				if (i == message.length())
				{
					temp += "0";
				}
				else
				{
					temp += message[i];
					++i;
				}
				++count;
			}
			message_blocks[j] = temp;
		}
		return message_blocks;
	}
	else
	{
		vector<string> message_blocks;
		for (int i = 0; i < block_size; ++i)
		{
			temp += '0';
		}
		message_blocks.push_back(temp);
		return message_blocks;
	}
}

// Converting string to binary
string toBinary(string message) 
{
	string binary;
	for (unsigned int i = 0; i < message.length(); ++i)
	{
		binary += bitset<8>(message[i]).to_string();
	}
	return binary;
}

// Converting string with binary to int (for S-blocks)
int toInt(string binary) 
{
	return stoi(binary, nullptr, 2);
}

// Converting string with binary to string with ASCII
string toASCII(string message_binary)
{
	string result = "";
	stringstream sstream(message_binary);
	while (sstream.good())
	{
		std::bitset<8> bits;
		sstream >> bits;
		char c = char(bits.to_ulong());
		result += c;
	}

	return result;
}

string XOR(string value1, string value2)
{
	string result = "";
	for (unsigned int i = 0; i < value1.length(); ++i)
	{
		if (value1[i] != value2[i])
			result += "1";
		else
			result += "0";
	}
	return result;
}

// Shifting by "shift" bits
string Shift(string message, int shift) 
{
	string result = "";
	string temp = message;
	for (int j = 0; j < shift; ++j)
	{
		result = "";
		for (unsigned int i = 0; i < message.length() - 1; ++i)
		{
			result += temp[i + 1];
		}
		result += temp[0];
		temp = result;
	}
	return result;
}

// S blocks transformation
string STransform(string message)
{
	string result = "";
	vector<string> blocks(8);
	string temp = "";
	for (int j = 0; j < 8; ++j)
	{
		temp = "";
		for (int i = 0; i < 4; ++i)
		{
			temp += message[j + i];
		}
		blocks[j] = temp;
	}

	static const vector<vector<int>> S =
	{
		 { 4,10,9,2,13,8,0,14,6,11,1,12,7,15,5,3 }, //S1

		 { 14,11,4,12,6,13,15,10,2,3,8,1,0,7,5,9 }, // S2

		 { 5,8,1,13,10,3,4,2,14,15,12,7,6,0,9,11 },// S3

		 { 7,13,10,1,0,8,9,15,14,4,6,12,11,2,5,3 },// S4

		 { 6,12,7,1,5,15,13,8,4,10,9,14,0,3,11,2 },// S5

		 { 4,11,10,0,7,2,1,13,3,6,8,5,9,12,15,14 }, // S6

		 { 13,11,4,1,3,15,5,9,0,10,14,7,6,8,2,12 },// S7

		 { 1,15,13,0,5,7,10,4,9,2,3,14,6,11,8,12 },// S8
	};

	int S_index = -1;
	for (unsigned int i = 0; i < blocks.size(); ++i)
	{
		S_index = toInt(blocks[i]);
		result += bitset<4>(S[i][S_index]).to_string();
	}

	return result;
}

string mod32(string block, string key)
{
	string temp="";
	int size = block.length();
	bool add = false;

	for (int i = size-1; i >= 0; --i)
	{
		if (block[i] == key[i])
		{
			if (add == false)
			{
				temp += '0';
				if (block[i] == '1')
				{
					add = true;
				}
			}
			else
			{
				temp += '1';
				if (block[i] == '0')
				{
					add = false;
				}
			}
		}
		else
		{
			if (add == true)
				temp += '0';
			else
			{
				temp += '1';
				add = false;
			}
		}
	}
	string result="";
	for (int i = size-1; i >= 0; --i)
	{
		result += temp[i];
	}

	return result;
}

string f(string L, string key)
{
	string result = "";
	string temp1 = "";
	string temp2 = "";
	temp1 = mod32(L, key);
	temp2 = STransform(temp1);
	result = Shift(temp2, SHIFT);
	return result;
}

vector<string> Key_generator(vector<string> keys)
{
	vector<string> result;
	for (unsigned int j = 0; j < 3; ++j)
	{
		for (unsigned int i = 0; i < 8; ++i)
		{
			result.push_back(keys[i]);
		}
	}
	for (unsigned int i = 0; i < 8; ++i)
	{
		result.push_back(keys[7 - i]);
	}
	return result;
}

int main()
{
	string message = "";
	string message_binary = "";
	vector<string> message_binary_blocks;
	string message_encrypted = "";
	string message_binary_encrypted = "";
	vector<string> message_binary_encrypted_blocks;
	string message_decrypted = "";
	string message_binary_decrypted = "";
	string key = "";
	string key_binary;
	vector<string> key_binary_blocks;
	vector<string> key_binary_blocks_ordered;
	vector<string> key_temps;


	
	message = "q w e r t y u i ";
	key = "qwertyuiopasdfghjklzxcvbnm123456";	  // 32 symbols = 256 bits

	cout << "message: " << message << "\n\nkey: " << key << endl;
	message_binary = toBinary(message);
	cout << "\n\nbinary message: " << message_binary << endl;
	message_binary_blocks = toBlocks(message_binary, MESSAGE_BLOCK_SIZE);
	key_binary = toBinary(key);
	cout << "\n\nbinary key: " << key_binary << endl;
	key_binary_blocks = toBlocks(key_binary, KEY_BLOCK_SIZE);

	for (int i = 0; i < key_binary_blocks.size(); ++i)
	{
		key_binary_blocks_ordered.push_back(key_binary_blocks[key_binary_blocks.size()-1-i]);
	}
	key_temps = Key_generator(key_binary_blocks_ordered);

	/////////////////////////////// Encryption ////////////////////////////
	string temp_key = "";
	for (string x : message_binary_blocks)
	{
		string A = "";
		string B = "";
		for (int i = 0; i < 32; ++i)
			B += x[i];
		for (int i = 32; i < 64; ++i)
			A += x[i];

		string Ai = "";
		string Bi = "";
		for (int i = 0; i < 32; ++i)
		{
			Bi = A;
			temp_key = key_temps[i];
			string temp = "";
			temp = f(A, temp_key);
			Ai = XOR(B, temp);

			A = Ai;
			B = Bi;
		}
		message_binary_encrypted += Ai + Bi; 
	}
	cout << "\n\nEncrypted message binary: " << message_binary_encrypted << endl;

	message_encrypted = toASCII(message_binary_encrypted);
	cout << "\n\nEncrypted message: " << message_encrypted << endl;

	message_binary_encrypted_blocks = toBlocks(message_binary_encrypted, MESSAGE_BLOCK_SIZE);

	/////////////////////////////// Decryption ////////////////////////////
	for (string x : message_binary_encrypted_blocks)
	{
		string A = "";
		string B = "";
		for (int i = 0; i < 32; ++i)
			B += x[i];
		for (int i = 32; i < 64; ++i)
			A += x[i];

		string Ai = "";
		string Bi = "";
		for (int i = 0; i < 32; ++i)
		{
			
			Bi = A;
			temp_key = key_temps[31-i];
			string temp = "";
			temp = f(A, temp_key);
			Ai = XOR(B, temp);
			A = Ai;
			B = Bi;
		}
		message_binary_decrypted += Ai + Bi;
	}
	cout << "\n\nDecrypted message binary: " << message_binary_decrypted << endl;

	message_decrypted = toASCII(message_binary_decrypted);
	cout << "\n\nDecrypted message: " << message_decrypted << endl;

	system("pause");
	return 0;
}
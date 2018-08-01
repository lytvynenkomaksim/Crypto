/* This is a Case Study project. It's a simple DES(Data Encryption Standard) algorithm, mode - electronic code book(Simplest, standard mode)
Made by Maksym Lytvynenko*/


#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>



using namespace std;

// Converting string to binary, returns string with zeros and ones. 
string toBinary(string message) 
{
	string binary;
	for (unsigned int i = 0; i < message.length(); ++i)
	{
		binary += bitset<8>(message[i]).to_string();
	}
	return binary;
}

// Converts string of zeros and ones to int
int toInt(string binary)
{
	return stoi(binary, nullptr, 2);
}

// Converts string of zeros and ones to text(sequence of ASCII) 
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

// Initial transformation of 64 bit block of text
string IP(string message)
{
	string transformed_message = { message[57],message[49],message[41],message[33],message[25],message[17],message[9],message[1],
						message[59],message[51],message[43],message[35],message[27],message[19],message[11],message[3],
						message[61],message[53],message[45],message[37],message[29],message[21],message[13],message[5],
						message[63],message[55],message[47],message[39],message[31],message[23],message[15],message[7],
						message[56],message[48],message[40],message[32],message[24],message[16],message[8],message[0],
						message[58],message[50],message[42],message[34],message[26],message[18],message[10],message[2],
						message[60],message[52],message[44],message[36],message[28],message[20],message[12],message[4],
						message[62],message[54],message[46],message[38],message[30],message[22],message[14],message[6] };
	return transformed_message;
}

// Initial Key transformation, and divide key to C and D blocks, C and D 56-bit long
vector<string> toKey56(string key) 
{
	string key56C = { key[56],key[48],key[40],key[32],key[24],key[16],key[8],
					 key[0],key[57],key[49],key[41],key[33],key[25],key[17],
					 key[9],key[1],key[58],key[50],key[42],key[34],key[26],
					 key[18],key[10],key[2],key[59],key[51],key[43],key[35] };

	string key56D = { key[62],key[54],key[46],key[38],key[30],key[22],key[14],
					key[6],key[61],key[53],key[45],key[37],key[29],key[21],
					key[13],key[5],key[60],key[52],key[44],key[36],key[28],
					key[20],key[12],key[4],key[27],key[19],key[11],key[3] };

	return vector<string>{ key56C, key56D };
}

// Creating Sub-keys from C and D Key blocks
string SubKey(vector<string> key) 
{
	string temp = key[0] + key[1];
	string sub_key = { temp[13],temp[16],temp[10],temp[23],temp[0],temp[4],
					   temp[2],temp[27],temp[14],temp[5],temp[20],temp[9],
					   temp[22],temp[18],temp[11],temp[3],temp[25],temp[7],
					   temp[15],temp[6],temp[26],temp[19],temp[12],temp[1],
					   temp[40],temp[51],temp[30],temp[36],temp[46],temp[54],
					   temp[29],temp[39],temp[50],temp[44],temp[32],temp[47],
					   temp[43],temp[48],temp[38],temp[55],temp[33],temp[52],
					   temp[45],temp[41],temp[49],temp[35],temp[28],temp[31] };

	return sub_key;
}

// Shifting Key block C and D
vector<string> Shift(vector<string> key, int shift) 
{
		string shiftedC = "";
		string shiftedD = "";
		string tempC = key[0];
		string tempD = key[1];
		for (int j = 0; j < shift; ++j)
		{
			shiftedC = "";
			shiftedD = "";
			for (unsigned int i = 0; i < key[0].length() - 1; ++i)
			{
				shiftedC += tempC[i + 1];
				shiftedD += tempD[i + 1];
			}
			shiftedC += tempC[0];
			shiftedD += tempD[0];
			tempC = shiftedC;
			tempD = shiftedD;
		}
		return vector<string>{shiftedC, shiftedD};
}

// expand 32 bit R block to 48 bit
string Expand(string R) 
{
	return string{
		 R[31], R[0], R[1], R[2], R[3], R[4],
		 R[3],R[4],R[5],R[6],R[7],R[8],
		 R[7],R[8],R[9],R[10],R[11],R[12],
		 R[11],R[12],R[13],R[14],R[15],R[16],
		 R[15],R[16],R[17],R[18],R[19],R[20],
		 R[19],R[20],R[21],R[22],R[23],R[24],
		 R[23],R[24],R[25],R[26],R[27],R[28],
		 R[27],R[28],R[29],R[30],R[31],R[0] };
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
		for (int i = 0; i < 6; ++i)
		{
			temp += message[j + i];
		}
		blocks[j] = temp;
	}

	static const vector<vector<vector<int>>> S =
	{
		{ {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, //S1
		{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
		{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
		{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 } },

		{ {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10}, // S2
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9} },

		{ {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},// S3
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12} },

		{ {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},// S4
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14} },

		{ {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},// S5
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3} },

		{ {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11}, // S6
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13} },

		{ {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},// S7
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12} },

		{ {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},// S8
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11} }
	};

	int row = -1;
	int col = -1;
	for (unsigned int i = 0; i < blocks.size(); ++i)
	{
		row = toInt(string{ blocks[i][0], blocks[i][5] });
		col = toInt(string{ blocks[i][1],  blocks[i][2], blocks[i][3], blocks[i][4] });
		result += bitset<4>(S[i][row][col]).to_string();
	}

	return result;
}

// Returning string Value1 XOR Value2
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

// Applying to R block after S blocks computing
string Pblock(string S) 
{
	return string{ S[15],S[6],S[19],S[20],
				   S[28],S[11],S[27],S[16],
				   S[0],S[14],S[22],S[25],
				   S[4],S[17],S[30],S[9],
				   S[1],S[7],S[23],S[13],
				   S[31],S[26],S[2],S[8],
				   S[18],S[12],S[29],S[5],
				   S[21],S[10],S[3],S[24] };
}

// Final transformation, applied to L+R (encrypted message) after 16 cycles
string Final(string m) 
{
	return string{ m[39],m[7],m[47],m[15],m[55],m[23],m[63],m[31],
		m[38],m[6],m[46],m[14],m[54],m[22],m[62],m[30],
		m[37],m[5],m[45],m[13],m[53],m[21],m[61],m[29],
		m[36],m[4],m[44],m[12],m[52],m[20],m[60],m[28],
		m[35],m[3],m[43],m[11],m[51],m[19],m[59],m[27],
		m[34],m[2],m[42],m[10],m[50],m[18],m[58],m[26],
		m[33],m[1],m[41],m[9],m[49],m[17],m[57],m[25],
		m[32],m[0],m[40],m[8],m[48],m[16],m[56],m[24] };
}

// Dividing message to 64 bit blocks, add 0 to last block if needed
vector<string> toBlocks(string message)
{
	vector<string> message_blocks(ceil(message.size() / 64.0));
	string temp = "";
	int count = 0;
	unsigned int i = 0; // number of element in message
	for (int j = 0; j < message_blocks.size(); ++j)
	{
		count = 0;
		temp = "";
		while (count < 64)
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

string f(string L, string key)
{
	string result = "";
	string temp = "";
	string L48 = Expand(L);
	temp = XOR(L48, key);
	result = Pblock(STransform(temp));

	return result;
}

int main()
{
	string encrypted_message_binary = "";   // encrypted message in binary format
	string encrypted_message = "";
	string decrypted_message_binary = "";	// decrypted message in binary format
	string decrypted_message = "";
	vector<int> shift_encrypt = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 }; // each turn we will shift key, for decryption we shift it in reverse order
	vector<string> message_blocks;	// your message divided to 64-bit blocks
	string message = "";	// your input message
	string message_binary = ""; //your input message in binary format
	string key = "";
	string key_binary = "";
	vector<string> key56; // your key will be transformed to 56-bit key
	vector<string> key56_shifted;// You probably want to change architecture to avoid recalculation of for each block, you also don't need so many variables for key
	vector<string> key56_decrypt;// I did it this way, to keep it simple and clear. Over calculating couple of variables for couple of times + couple of addition variables 
								// is not that bad =)
	char tmp = ' ';

	
	message = "abcdabcd";	// It's message to be encrypted
	key = "qwertyui";		// It's key to use for message encryption
	

	cout << "\nmessage: " << message << "\nkey: " << key << endl;
	message_binary = toBinary(message);
	cout << "binary message: " << message_binary << endl;
	message_blocks = toBlocks(message_binary);
	key_binary = toBinary(key);
	cout << "binary key: " << key_binary << endl;
	key56 = toKey56(key_binary);
	cout << "key56 initial: " << key56[0] << key56[1] << endl;


	for (string x : message_blocks)
	{
		
		string L = "";
		string R = "";
		string Ri = "";
		string Li = "";
		string temp_key = "";
		x = IP(x);
		for (int i = 0; i < 32; ++i)
			L += x[i];
		for (int i = 32; i < 64; ++i)
			R += x[i];
		key56_decrypt.clear();
		key56_shifted = key56;
		for (int i = 0; i < 16; ++i)
		{
			Li = R;
			
			temp_key = SubKey(key56_shifted);
			key56_decrypt.push_back(temp_key);
			key56_shifted = Shift(key56_shifted, shift_encrypt[i]);
			string temp = f(R, temp_key);
			Ri = XOR(L, temp);

			L = Li;
			R = Ri;
		}

		encrypted_message_binary += Final(string(L + R));
	}

	cout << "\n\n\n\n Encrypted message_binary: " << encrypted_message_binary << endl;
	encrypted_message = toASCII(encrypted_message_binary);
	cout << "\n\n\n\n Encrypted message: " << encrypted_message << endl;



	////////////////////////////////////// Decryption ////////////////////////////////////////////
	message_blocks = toBlocks(encrypted_message_binary);
	for (string x : message_blocks)
	{
		string L = "";
		string R = "";
		string Ri = "";
		string Li = "";
		string temp_key = "";
		x = IP(x);
		for (int i = 0; i < 32; ++i)
			L += x[i];
		for (int i = 32; i < 64; ++i)
			R += x[i];

		for (int i = 0; i < 16; ++i)
		{
			Ri = L;
			temp_key = key56_decrypt[15 - i]; // Using Keys in reversed order
			string temp = f(L, temp_key);
			Li = XOR(R, temp);
			L = Li;
			R = Ri;
		}
		decrypted_message_binary += Final(string(L + R));
	}



	cout << "\n\n\n\n Decrypted message_binary: " << decrypted_message_binary << endl;
	decrypted_message = toASCII(decrypted_message_binary);
	cout << "\n\n\n\n Decrypted message: " << decrypted_message << endl;


	system("pause");
	return 0;
}
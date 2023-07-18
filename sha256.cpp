#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>
#include <vector>
#include <bit>
#include <format>

using namespace std;

//first 32 bits of first 8 prime numbers 
uint32_t _simple_num[] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 
				0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

//first 32 bits of fractional root numbers of 64 first prime numbers
uint32_t _root_num[] = {0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

 

vector<bool> _decToBinary(int n){
    vector<bool> binaryNum;

    int i = 0;
    while (n > 0) {
        binaryNum.push_back(n % 2);
        n = n / 2;
        i++;
    }
	vector<bool> temp;
	int m = 0;
    for (int j = i - 1; j >= 0; j--){
         temp.push_back(binaryNum[j]);
		 m++;
	}
	return temp;
}

vector<bool> _toBinary(string str){
	vector<bool> tempvect;
	for (std::size_t i = 0; i < str.size(); i++)
	{
		bitset<8> temp = bitset<8>(str.c_str()[i]);
		for(int k = 7; k >= 0; --k)
			tempvect.push_back(temp[k]);
	}
	return tempvect;
}
	
string sha256(string msg){
	vector<bool> m = _toBinary(msg);
	int size = m.size();
	m.push_back(1);
	int zeros = (448 - 1 - size) % 512;
	for(int i = 0; i < zeros; i++)
		m.push_back(0);
	vector<bool> sizeBin = _decToBinary(size);
	for(int i = 0; i < sizeBin.size(); i++)
		m.push_back(sizeBin[i]);
	for(int i = 0; i < m.size(); i += 512){
		uint32_t w[64];

		for(int k = 0; k < 16; k++){
			string bitstring = "";
			for(int l = 0; l < 32; l++){
				bitstring += to_string(m[k+l+i]);
			}
			bitset<32> tempbit(bitstring);
			w[k] = tempbit.to_ulong();
		}

		uint32_t s0;
		uint32_t s1;
		for(int k = 16; k < 64; k++){
			s0 = (rotr(w[k-15], 7)) ^ (rotr(w[k-15], 18)) ^ (w[k-15] >> 3);
			s1 = (rotr(w[k-2], 17)) ^ (rotr(w[k-2], 19)) ^ (w[k-2] >> 10);
			w[k] = w[k-16] + s0 + w[k-7] + s1;
		}

		uint32_t a = _simple_num[0];
		uint32_t b = _simple_num[1];
		uint32_t c = _simple_num[2];
		uint32_t d = _simple_num[3];
		uint32_t e = _simple_num[4];
		uint32_t f = _simple_num[5];
		uint32_t g = _simple_num[6];
		uint32_t h = _simple_num[7];

		for(int k = 0; k < 64; k++){
			uint32_t S1 = (rotr(e, 6)) ^ (rotr(e, 11) ^ (rotr(e, 25)));
			uint32_t ch = (e & f) ^ (~e & g);
			uint32_t temp1 = h + S1 + ch + _root_num[k] + w[k];
			uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint32_t temp2 = S0 + maj;
			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}
		_simple_num[0] = _simple_num[0] + a;
		_simple_num[1] = _simple_num[1] + b;
		_simple_num[2] = _simple_num[2] + c;
		_simple_num[3] = _simple_num[3] + d;
		_simple_num[4] = _simple_num[4] + e;
		_simple_num[5] = _simple_num[5] + f;
		_simple_num[6] = _simple_num[6] + g;
		_simple_num[7] = _simple_num[7] + h;
	}
	string hash = "";
	vector<bitset<8>> temp;
	for(int i = 0; i < 8; i++)
		hash += format("{:x}", _simple_num[i]);
	return hash;
}

int main(int args, char *argv[]){
	string msg;
	if(args > 1)
		msg = argv[1];
	else
		msg = "";
	cout << args << endl;
	cout << sha256(msg) << endl;
	vector<bool> temp = _decToBinary(448);
	return 0;
}

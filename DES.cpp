#include<iostream>
#include<string>
#include<vector>
#include<cassert>
using namespace std;

string num2bin(int num, int precision=1);
string hex2bin(char hex);
char bin2ASCII(string bin);
string bin2Str(string bins);
string bin2hexstr(string bins);
char num2hex(short num);
string strStrip(string str);

string Xor(string bin1, string bin2);
class DES
{
private:
    static string InitialPermutation(string cipher_64bit);
    static string Round(string cipher_64bit, string key_48bit);
    static string FinalPermutation(string cipher_64bit);

    static string F_func(string cipher_32bit, string subkey_48bit);
    static string expansion(string cipher_32bit);
    static string s_box(string bin_6bit, short id);
    static string permuatation(string bin_32bit);

    static vector<string> subKey_gen(string key_64bit);
    static string PC_1(string key_64bit);
    static string transform(string& key_56bit, short round);
    static string leftShift(string key_28bit, short round);
    static string PC_2(string key_56bit);
public:
    static string encrypt(string plain_64bit, string key_64bit);
    static string decrypt(string cipher_64bit, string key_64bit);
};

enum DES_mode {encrypt, decrypt};

int main()
{
    string key;
    string text;
    const short BITS_PER_BLOCK = 64;

    DES_mode mode = encrypt;

    while(cin >> key)
    {
        // key = "52415020474f4420";

        string key_bin = "";
        for(char k: key)
        {
            key_bin += hex2bin(k);
        }

        cin.ignore();
        getline(cin, text);

        if(mode == encrypt)
        {
            string plain = text;

            vector<string> plain_bins;
            string plain_binbuffer = "";
            for(char c: plain)
            {
                plain_binbuffer += num2bin(c, 8);

                if(plain_binbuffer.length() >= BITS_PER_BLOCK)
                {
                    plain_bins.push_back(plain_binbuffer);
                    plain_binbuffer.clear();
                }
            }
            if(plain_binbuffer.empty() == false)
            {
                while(plain_binbuffer.length() < BITS_PER_BLOCK)
                {
                    plain_binbuffer += num2bin(' ', 8);
                }
                plain_bins.push_back(plain_binbuffer);
                plain_binbuffer.clear();
            }

            string cipher = "";
            for(string plain_bin64: plain_bins)
            {
                string cipher_bin = DES::encrypt(plain_bin64, key_bin);
                cipher += bin2hexstr(cipher_bin);
            }
            cout << cipher << endl;
        }
        else if(mode == decrypt)
        {
            string cipher = text;
            // cipher = "b7625c9fe4cb43b3";

            vector<string> cipher_bins;
            string cipher_binbuffer = "";
            for(char c: cipher)
            {
                cipher_binbuffer += hex2bin(c);

                if(cipher_binbuffer.length() >= BITS_PER_BLOCK)
                {
                    cipher_bins.push_back(cipher_binbuffer);
                    cipher_binbuffer.clear();
                }
            }

            string plain = "";
            for(string cipher_bin64: cipher_bins)
            {
                string plain_bin = DES::decrypt(cipher_bin64, key_bin);
                plain += bin2Str(plain_bin);
            }

            plain = strStrip(plain);
            cout << plain << endl;
            //cout << "'" << plain << "'" << endl;
        }
    }
}

string num2bin(int num, int precision)
{
    string bin = "";
    
    while(num > 0)
    {
        bin = ((num % 2 == 0)? '0': '1') + bin;
        num /= 2;
    }

    while(bin.length() < precision)
    {
        bin = "0" + bin;
    }

    return bin;
}

string hex2bin(char hex)
{
    string bin = "";

    short decimal = hex - '0';
    short alphabet = hex - 'a';

    if(decimal >= 0 && decimal <= 9)
    {
        bin = num2bin(decimal, 4);
    }
    else if(alphabet >= 0 && alphabet <= 5)
    {
        bin = num2bin(10+alphabet, 4);
    }
    
    return bin;
}

char bin2ASCII(string bin)
{
    char c = 0;
    for(char b: bin)
    {
        c = (c << 1) + ((b == '0')? 0: 1);
    }
    return c;
}

string bin2Str(string bins)
{
    const short BITS_PER_BYTE = 8;
    string str = "";

    string bin_temp = "";
    for(char b: bins)
    {
        bin_temp += b;
        if(bin_temp.length() >= BITS_PER_BYTE)
        {
            str += bin2ASCII(bin_temp);
            bin_temp.clear();
        }
    }
    return str;
}

string bin2hexstr(string bins)
{
    #define bin2Num bin2ASCII

    string hexStr = "";

    string binbuffer = "";
    for(char b: bins)
    {
        binbuffer += b;

        if(binbuffer.length() >= 4)
        {
            hexStr.push_back(num2hex(bin2Num(binbuffer)));
            binbuffer.clear();
        }
    }
    return hexStr;
}

char num2hex(short num)
{
    char hex = '\0';
    if(num >= 0 && num <= 9)
    {
        hex = '0'+num;
    }
    else if(num >= 10 && num <= 15)
    {
        hex = 'a'+(num - 10);
    }
    return hex;
}

string strStrip(string str)
{
    string newStr = "";
    for(int i=str.length()-1; i >= 0; i--)
    {
        if(str[i] != ' ')
        {
            newStr = str.substr(0, i+1);
            break;
        }
    }
    return newStr;
}

string Xor(string bin1, string bin2)
{
    string result = "";

    int maxLen = max(bin1.length(), bin2.length());
    for(int i=0; i<maxLen; i++)
    {
        char b1 = '0';
        char b2 = '0';
        if(i < bin1.length())
        {
            b1 = bin1[bin1.length() - i - 1];
        }
        if(i < bin2.length())
        {
            b2 = bin2[bin2.length() - i - 1];
        }

        if(b1 == b2)
        {
            result = '0' + result;
        }
        else
        {
            result = '1' + result;
        }
    }

    return result;
}

string DES::encrypt(string plain_64bit, string key_64bit)
{
    vector<string> subkeys = DES::subKey_gen(key_64bit);

    string plain_temp = DES::InitialPermutation(plain_64bit);

    /*for(int i=16; i>=1; i--)
    {
        cout << i << ": " << subkeys[i-1] << endl;
    }*/

    for(int round=1; round <= 16; round++)
    {
        plain_temp = DES::Round(plain_temp, subkeys[round-1]);

        //cout << round << " " << plain_temp << endl;
    }

    string plain_roundEnd = plain_temp.substr(32, 32) + plain_temp.substr(0, 32);

    string cipher = DES::FinalPermutation(plain_roundEnd);

    return cipher;
}
string DES::decrypt(string cipher_64bit, string key_64bit)
{
    vector<string> subkeys = DES::subKey_gen(key_64bit);

    string cipher_temp = DES::InitialPermutation(cipher_64bit);

    /*for(int i=16; i>=1; i--)
    {
        cout << i << ": " << subkeys[i-1] << endl;
    }*/

    for(int round=16; round >= 1; round--)
    {
        cipher_temp = DES::Round(cipher_temp, subkeys[round-1]);

        //cout << round << " " << cipher_temp << endl;
    }

    string cipher_roundEnd = cipher_temp.substr(32, 32) + cipher_temp.substr(0, 32);

    string plain = DES::FinalPermutation(cipher_roundEnd);

    return plain;
}
string DES::InitialPermutation(string cipher_64bit)
{
    const short IP_TABLE[] = 
    {
        58, 50, 42, 34, 26, 18, 10, 2, 
        60, 52, 44, 36, 28, 20, 12, 4, 
        62, 54, 46, 38, 30, 22, 14, 6, 
        64, 56, 48, 40, 32, 24, 16, 8, 
        57, 49, 41, 33, 25, 17,  9, 1, 
        59, 51, 43, 35, 27, 19, 11, 3, 
        61, 53, 45, 37, 29, 21, 13, 5, 
        63, 55, 47, 39, 31, 23, 15, 7
    };
    
    string cipher_permuted = "";
    for(int i=0; i<64; i++)
    {
        cipher_permuted += cipher_64bit[IP_TABLE[i] - 1];
    }
    return cipher_permuted;
}
string DES::Round(string cipher_64bit, string key_48bit)
{
    string cipher_32bit_left = cipher_64bit.substr(0, 32);
    string cipher_32bit_right = cipher_64bit.substr(32, 32);

    string f_out = DES::F_func(cipher_32bit_right, key_48bit);
    string xor_out = Xor(cipher_32bit_left, f_out);

    return cipher_32bit_right + xor_out;
}
string DES::FinalPermutation(string cipher_64bit)
{
    const short IP_INV_TABLE[] = 
    {
        40,  8, 48, 16, 56, 24, 64, 32, 
        39,  7, 47, 15, 55, 23, 63, 31, 
        38,  6, 46, 14, 54, 22, 62, 30, 
        37,  5, 45, 13, 53, 21, 61, 29, 
        36,  4, 44, 12, 52, 20, 60, 28, 
        35,  3, 43, 11, 51, 19, 59, 27, 
        34,  2, 42, 10, 50, 18, 58, 26, 
        33,  1, 41,  9, 49, 17, 57, 25
    };
    
    string cipher_permuted = "";
    for(int i=0; i<64; i++)
    {
        cipher_permuted += cipher_64bit[IP_INV_TABLE[i] - 1];
    }
    return cipher_permuted;
}


string DES::F_func(string cipher_32bit, string subkey_48bit)
{
    string cipher_48bit = DES::expansion(cipher_32bit);
    string xor_out = Xor(cipher_48bit, subkey_48bit);

    string sbox_out = "";
    for(int id=1; id<=8; id++)
    {
        sbox_out += DES::s_box(xor_out.substr((id-1)*6, 6), id);
    }

    string f_out =  DES::permuatation(sbox_out);
    return f_out;
}
string DES::expansion(string cipher_32bit)
{
    const short EXPEND_TABLE[] = 
    {
        32,  1,  2,  3,  4,  5, 
         4,  5,  6,  7,  8,  9, 
         8,  9, 10, 11, 12, 13, 
        12, 13, 14, 15, 16, 17, 
        16, 17, 18, 19, 20, 21, 
        20, 21, 22, 23, 24, 25, 
        24, 25, 26, 27, 28, 29, 
        28, 29, 30, 31, 32,  1
    };
    string cipher_expanded = "";
    for(int i=0; i<48; i++)
    {
        cipher_expanded += cipher_32bit[EXPEND_TABLE[i] - 1];
    }
    return cipher_expanded;
}
string DES::s_box(string bin_6bit, short id)
{
    #define bin2Num bin2ASCII

    const short S_BOXS[][64] = 
    {
        {
            14,  0,  4, 15, 13,  7,  1,  4, 
             2, 14, 15,  2, 11, 13,  8,  1, 
             3, 10, 10,  6,  6, 12, 12, 11, 
             5,  9,  9,  5,  0,  3,  7,  8, 
             4, 15,  1, 12, 14,  8,  8,  2, 
            13,  4,  6,  9,  2,  1, 11,  7, 
            15,  5, 12, 11,  9,  3,  7, 14, 
             3, 10, 10,  0,  5,  6,  0, 13
        }, 
        {
            15,  3,  1, 13,  8,  4, 14,  7, 
             6, 15, 11,  2,  3,  8,  4, 14, 
             9, 12,  7,  0,  2,  1, 13, 10, 
            12,  6,  0,  9,  5, 11, 10,  5,
             0, 13, 14,  8,  7, 10, 11,  1, 
            10,  3,  4, 15, 13,  4,  1,  2, 
             5, 11,  8,  6, 12,  7,  6, 12, 
             9,  0,  3,  5,  2, 14, 15,  9
        }, 
        {
            10, 13,  0,  7,  9,  0, 14,  9, 
             6,  3,  3,  4, 15,  6,  5, 10, 
             1,  2, 13,  8, 12,  5,  7, 14, 
            11, 12,  4, 11,  2, 15,  8,  1, 
            13,  1,  6, 10,  4, 13,  9,  0, 
             8,  6, 15,  9,  3,  8,  0,  7, 
            11,  4,  1, 15,  2, 14, 12,  3, 
             5, 11, 10,  5, 14,  2,  7, 12
        }, 
        {
             7, 13, 13,  8, 14, 11,  3,  5, 
             0,  6,  6, 15,  9,  0, 10,  3, 
             1,  4,  2,  7,  8,  2,  5, 12, 
            11,  1, 12, 10,  4, 14, 15,  9, 
            10,  3,  6, 15,  9,  0,  0,  6, 
            12, 10, 11,  1,  7, 13, 13,  8, 
            15,  9,  1,  4,  3,  5, 14, 11, 
             5, 12,  2,  7,  8,  2,  4, 14
        }, 
        {
             2, 14, 12, 11,  4,  2,  1, 12, 
             7,  4, 10,  7, 11, 13,  6,  1, 
             8,  5,  5,  0,  3, 15, 15, 10, 
            13,  3,  0,  9, 14,  8,  9,  6, 
             4, 11,  2,  8,  1, 12, 11,  7, 
            10,  1, 13, 14,  7,  2,  8, 13, 
            15,  6,  9, 15, 12,  0,  5,  9, 
             6, 10,  3,  4,  0,  5, 14,  3
        }, 
        {
            12, 10,  1, 15, 10,  4, 15,  2, 
             9,  7,  2, 12,  6,  9,  8,  5, 
             0,  6, 13,  1,  3, 13,  4, 14, 
            14,  0,  7, 11,  5,  3, 11,  8, 
             9,  4, 14,  3, 15,  2,  5, 12, 
             2,  9,  8,  5, 12, 15,  3, 10, 
             7, 11,  0, 14,  4,  1, 10,  7, 
             1,  6, 13,  0, 11,  8,  6, 13
        }, 
        {
             4, 13, 11,  0,  2, 11, 14,  7, 
            15,  4,  0,  9,  8,  1, 13, 10, 
             3, 14, 12,  3,  9,  5,  7, 12, 
             5,  2, 10, 15,  6,  8,  1,  6, 
             1,  6,  4, 11, 11, 13, 13,  8, 
            12,  1,  3,  4,  7, 10, 14,  7, 
            10,  9, 15,  5,  6,  0,  8, 15, 
             0, 14,  5,  2,  9,  3,  2, 12
        }, 
        {
            13,  1,  2, 15,  8, 13,  4,  8, 
             6, 10, 15,  3, 11,  7,  1,  4, 
            10, 12,  9,  5,  3,  6, 14, 11, 
             5,  0,  0, 14, 12,  9,  7,  2, 
             7,  2, 11,  1,  4, 14,  1,  7, 
             9,  4, 12, 10, 14,  8,  2, 13, 
             0, 15,  6, 12, 10,  9, 13,  0, 
            15,  3,  3,  5,  5,  6,  8, 11
        }
    };

    short oriNum = bin2Num(bin_6bit);
    short newNum = S_BOXS[id-1][oriNum];
    string bin_4bit = num2bin(newNum, 4);
    return bin_4bit;
}
string DES::permuatation(string bin_32bit)
{
    const short PERMUTE_TABLE[] = 
    {
        16,  7, 20, 21, 
        29, 12, 28, 17, 
         1, 15, 23, 26, 
         5, 18, 31, 10, 
         2,  8, 24, 14, 
        32, 27,  3,  9, 
        19, 13, 30,  6, 
        22, 11,  4, 25
    };
    string bin_permuted = "";
    for(int i=0; i<32; i++)
    {
        bin_permuted += bin_32bit[PERMUTE_TABLE[i] - 1];
    }
    return bin_permuted;
}


vector<string> DES::subKey_gen(string key_64bit)
{
    vector<string> subkeys;

    string key_56bit = DES::PC_1(key_64bit);
    for(int round=1; round<=16; round++)
    {
        string subkey = DES::transform(key_56bit, round);
        subkeys.push_back(subkey);
    }
    return subkeys;
}
string DES::PC_1(string key_64bit)
{
    const short PC1_TABLE[] = 
    {
        57, 49, 41, 33, 25, 17,  9, 
         1, 58, 50, 42, 34, 26, 18, 
        10,  2, 59, 51, 43, 35, 27, 
        19, 11,  3, 60, 52, 44, 36, 
        63, 55, 47, 39, 31, 23, 15, 
         7, 62, 54, 46, 38, 30, 22, 
        14,  6, 61, 53, 45, 37, 29, 
        21, 13,  5, 28, 20, 12,  4
    };
    string key_permuted = "";
    for(int i=0; i<56; i++)
    {
        key_permuted += key_64bit[PC1_TABLE[i] - 1];
    }
    return key_permuted;
}
string DES::transform(string& key_56bit, short round)
{
    string key_28bit_left = key_56bit.substr(0, 28);
    string key_28bit_right = key_56bit.substr(28, 28);

    string key_28bit_left_shifted = DES::leftShift(key_28bit_left, round);
    string key_28bit_right_shifted = DES::leftShift(key_28bit_right, round);
    key_56bit = key_28bit_left_shifted+key_28bit_right_shifted;

    string PC2_out = DES::PC_2(key_56bit);
    return PC2_out;
}
string DES::leftShift(string key_28bit, short round)
{
    const short MOVE_TABLE[] = 
    {
        1, 1, 2, 2, 
        2, 2, 2, 2, 
        1, 2, 2, 2, 
        2, 2, 2, 1
    };

    string key_shifted = "";
    key_shifted = key_28bit.substr(MOVE_TABLE[round-1]);
    key_shifted += key_28bit.substr(0, MOVE_TABLE[round-1]);

    return key_shifted;
}
string DES::PC_2(string key_56bit)
{
    const short PC2_TABLE[] = 
    {
        14, 17, 11, 24,  1,  5, 
         3, 28, 15,  6, 21, 10, 
        23, 19, 12,  4, 26,  8, 
        16,  7, 27, 20, 13,  2, 
        41, 52, 31, 37, 47, 55, 
        30, 40, 51, 45, 33, 48, 
        44, 49, 39, 56, 34, 53, 
        46, 42, 50, 36, 29, 32
    };
    string key_permuted = "";
    for(int i=0; i<48; i++)
    {
        key_permuted += key_56bit[PC2_TABLE[i] - 1];
    }
    return key_permuted;
}
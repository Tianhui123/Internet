#include<iostream>
#include<vector>
#include<regex>
#include<fstream>
#include"total.h"
#include"interNet.h"

using namespace std;

/*

#define url "https://51cg7.com/archives/28848/"
#define urll "https://51cg7.com/archives/28383/"

std::string to_utf8(const char* ansi_chars)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring utf16_str = conv.from_bytes(yuan);
	std::string utf8_str = std::string(reinterpret_cast<const char*>(utf16_str.c_str()), utf16_str.size() * sizeof(wchar_t));
	std::string(reinterpret_cast<const char*>(utf16_str.c_str()), utf16_str.size() * sizeof(wchar_t));

	char ansi_chars[] = "Hello, world!";

	 将ANSI编码的字符数组转换为UTF-8编码的字符串
	std::string utf8_str;
	for (int i = 0; i < strlen(ansi_chars); i++) {
		unsigned char c = static_cast<unsigned char>(ansi_chars[i]);
		if (c < 0x80) {
			utf8_str += c;
		}
		else if (c < 0xC0) {
			// 非法UTF-8编码
			break;
		}
		else if (c < 0xE0) {
			utf8_str += ((c & 0x1F) << 6) | (static_cast<unsigned char>(ansi_chars[++i]) & 0x3F);
		}
		else if (c < 0xF0) {
			utf8_str += ((c & 0x0F) << 12) | ((static_cast<unsigned char>(ansi_chars[++i]) & 0x3F) << 6) | (static_cast<unsigned char>(ansi_chars[++i]) & 0x3F);
		}
		else {
			// 非法UTF-8编码
			break;
		}
	}


	return utf8_str;
}


std::string AnsiToUtf8(const std::string& ansi)
{
	int len = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, NULL, 0);
	wchar_t* wide = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, wide, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
	char* utf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, wide, -1, utf8, len, NULL, NULL);

	std::string ret(utf8);
	delete[] wide;
	delete[] utf8;
	return ret;
}


char * to_utf8(const char*input_utf8)
{
	int input_utf8_len = strlen(input_utf8);

	int output_utf16_len = MultiByteToWideChar(CP_UTF8, 0, input_utf8, input_utf8_len, NULL, 0);
	wchar_t* output_utf16 = new wchar_t[output_utf16_len + 1];
	memset(output_utf16, 0, sizeof(wchar_t) * (output_utf16_len + 1));

	MultiByteToWideChar(CP_UTF8, 0, input_utf8, input_utf8_len, output_utf16, output_utf16_len);

	int output_acp_len = WideCharToMultiByte(CP_ACP, 0, output_utf16, output_utf16_len, NULL, 0, NULL, NULL);
	char* output_acp = new char[output_acp_len + 1];
	memset(output_acp, 0, sizeof(char) * (output_acp_len + 1));

	WideCharToMultiByte(CP_ACP, 0, output_utf16, output_utf16_len, output_acp, output_acp_len, NULL, NULL);



	delete[] output_utf16;
	//delete[] output_acp;

	return output_acp;
}


*/

const char* paath = ".\\aNet";

fstream inout{};

int main()
{
	inout.open(paath, fstream::binary | fstream::out);

	Net net{};

	threadpool pool;
	pool.start(3);
	
	auto rr = net.request("https://www.qqkw.com/touxiang/");

	std::smatch need{};

	vector< future<results>>resArr{};
	vector<string>strArr{};

	auto start = rr.text_.cbegin();

	while (std::regex_search(start, rr.text_.cend(), need, std::regex("<img alt=\".*?\" src=\"(.*?)\"")))
	{
		strArr.push_back(need.str(1));
		start = need.suffix().first;
		if (strArr.size() >= 10)
			break;
	}

	for (auto& i : strArr)
	{
		resArr.emplace_back(pool.submit([&net](const string& p)->results {return net.request(p.c_str()); }, i));
	}
	
	

	for (int i = 0; i <resArr.size(); ++i)
	{
		std::string temp(paath);
		temp = temp + "\\" + to_string(i) + ".jpg";
		inout.open(temp.c_str(), fstream::binary | fstream::out);

		auto text = resArr[i].get();
		
		inout << text.text_;
		
		inout.close();
	}





	return 0;
}


#pragma once


#ifdef _WIN32

#include<io.h>
#include <winsock2.h>
#include<WS2tcpip.h>
#include<direct.h>
#pragma comment(lib, "ws2_32.lib")


#elif __linux

#include <cstring>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#endif

#include<fstream>
#include <iostream>
//#include <locale>
//#include <codecvt>
#include <openssl/ssl.h>
#include"total.h"

#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")


enum class AGREEMENT
{
	Ipv4,
	Ipv6,
	NoAgreement
};

enum class IP_AGREEMENT
{
	Ipv4,
	Ipv6,
	TCP,
	UDP
};

struct results final :TianHui::noCopy
{
	explicit results(std::string&& str,std::string&&hea, int sta) :
		text_(std::forward<std::string>(str)),
		header_(std::forward<std::string>(hea)),
		statusCode_(sta)
	{}

	results(results&& res) noexcept:
		text_(std::move(res.text_)),
		header_(std::move(res.header_)),
		statusCode_(res.statusCode_)
	{
		res.~results();
	}

	~results() = default;

public:

	std::string text_;
	std::string header_;
	int statusCode_;

};



class Net final :TianHui::noCopy
{
public:
	//Ĭ�Ͽͻ���
	Net();

	// ����Э��
	explicit Net(const IP_AGREEMENT& , const AGREEMENT& );

	~Net();



	void clear();


	// ��Ŀ���������������
	bool sockConnect(const char* , const char* post);

	// ʹ��HTTP��������
	std::string GET(const char *, const char* );

	// ���ӷ���������ȡ��������
	results request(const char* , const char* port = "https");

	// ���Ӽ��ܷ�����
	bool sslConnect();

	std::string Send(const std::string & reques)
	{
		if (SSL_write(sockSsl_, reques.c_str(), reques.size()) <= 0)
		{
			std::cerr << "Error sending request" << std::endl;
			throw std::runtime_error("Error sending request");
		}

		std::string response{};
		char buffer[1024] = { 0 };

		int bytes = SSL_read(sockSsl_, buffer, 1024);

		int status = 100 * (*(buffer + 9) - '0') + 10 * (*(buffer + 10) - '0') + *(buffer + 11) - '0';

		do
		{
			response += buffer;
		} while ((bytes = SSL_read(sockSsl_, buffer, sizeof(buffer))) > 0);

		return std::move(response);
	}


private:
	// �׽���
	int sock_;

	// Э����Ϣ
	addrinfo hint_;

	// ����Э��
	AGREEMENT agree_;

	// IPЭ��
	IP_AGREEMENT ipAgree_;

	// �����׽���
	SSL* sockSsl_;

	// �ر�������Դ
	void close();

	// ƥ��Э�鲢�����׽���
	void useAgree(bool client = true);

	std::vector<int> split(const char* host, const char& c)noexcept;

	std::string read_write(const mString& reques, bool isPort_443,int& status, int& position);

};






















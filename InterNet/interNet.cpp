#include"interNet.h"



const char* path = "./aNet";


Net::Net():
	sock_(-1),
	hint_({0}),
	agree_(AGREEMENT::Ipv4),
	ipAgree_(IP_AGREEMENT::TCP),
	sockSsl_{nullptr}
{
	
	int flag = -1;
#ifdef _WIN32
	if (_access(path, 0) == -1)
	{
		flag = _mkdir(path);
	}
	// 打开windows动态链接
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return;
	}



#elif __linux
	if (_access(path, 0) == -1)
	{
		flag = mkdir("./aNet", 4 | 2 | 1);
	}
#endif 

	// 打断点查看hint是否置空
	SSL_library_init();
	SSL_load_error_strings();
	// 默认状态下Ipv4和TCP协议
	useAgree();
	
	



}


Net::Net(const IP_AGREEMENT& ipAg, const AGREEMENT& ag):
	sock_(-1),
	hint_({ 0 }),
	agree_(ag),
	ipAgree_(ipAg),
	sockSsl_{ nullptr }
{
	int flag = -1;

#ifdef _WIN32

	if (_access(path, 0) == -1)
	{
		flag = _mkdir(path);
	}

	// 打开windows动态链接
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return;
	}



#elif __linux

	if (_access(path, 0) == -1)
	{
		flag = mkdir("./aNet", 4 | 2 | 1);
	}
	

#endif 

	SSL_library_init();
	SSL_load_error_strings();

	useAgree();


}


Net::~Net()
{
	close();
}

void Net::close()
{

#ifdef WIN32

	closesocket(sock_);
	WSACleanup();

#elif __linux

	close(sock_);

#endif

	EVP_cleanup();

	if (sockSsl_) {
		SSL_shutdown(sockSsl_);
		SSL_free(sockSsl_);
	}

}

void Net::clear()
{
	
}

bool Net::sockConnect(const char* hostname, const char* post)
{

	sock_ = socket(hint_.ai_family, hint_.ai_socktype, hint_.ai_protocol);
	if (sock_ == -1)
	{
		std::cerr << "failed creat socket: " << std::endl;
		return false;
	}

	addrinfo* addrs_ = nullptr;
	const int sta = getaddrinfo(hostname, post, &hint_, &addrs_);
	if (sta != 0)
	{
		std::cerr << "failed translate host 域名解析失败" << std::endl;
		return false;
	}

	for (auto addr = addrs_; addr != nullptr; addr = addr->ai_next)
	{

		if ((connect(sock_, addr->ai_addr, addr->ai_addrlen)) == -1)
		{
			continue;
		}
		
	}



	freeaddrinfo(addrs_);

	return true;

}

std::string Net::GET(const char*url, const char *port)
{
	sockConnect(url, port);

	// 设置请求头
	mString reques = "GET / HTTP/1.1\r\n";
	reques += "Host: ";
	reques += url;
	reques += "\r\nConnection: close\r\n\r\n";

	// 发送数据
	int resultCode = send(sock_, reques.c_str(), reques.size(), 0);
	if (resultCode == -1)
	{
		std::cerr << "error send" << std::endl;
		throw std::runtime_error("error send");
	}

	std::string temp{};
	// 接收数据
	char buffer[1024] = { 0 };
	resultCode = 0;
	resultCode = recv(sock_, buffer, 1024, 0);

	char* ptr = buffer + 9;

	int status = 100 * (*(buffer + 9) - '0') + 10 * (*(buffer + 10) - '0') + *(buffer + 11) - '0';

	while (resultCode > 0)
	{
		temp = temp + buffer;
		resultCode = recv(sock_, buffer, 1024, 0);
	}
		


	//return  results(std::move(temp), status);
	return std::move(temp);
}

std::vector<int> Net::split(const char* host,const char& c)noexcept
{

	std::vector<int> temp{};

	int n = 0;
	for (auto i = host; *i != '\0'; ++i,++n)
	{
		if (*i == '/')
			temp.push_back(n + 1);
	}
	

	return temp;
}

std::string Net::read_write(const mString& reques, bool isPort_443, int& status,int &position)
{

	std::string response{};
	char buffer[1024] = { 0 };
	int bytes = 0;
	if (isPort_443) 
	{

		if (SSL_write(sockSsl_, reques.c_str(), reques.size()) <= 0)
		{
			std::cerr << "Error sending request" << std::endl;
			throw std::runtime_error("Error sending request");
		}

		bytes = SSL_read(sockSsl_, buffer, 1024);

		status = 100 * (*(buffer + 9) - '0') + 10 * (*(buffer + 10) - '0') + *(buffer + 11) - '0';

		do
		{
			response.append(buffer, 1024);
		} while ((bytes = SSL_read(sockSsl_, buffer, sizeof(buffer))) > 0);

		position = 4;
		for (int i = 4; i <= response.size(); ++i, ++position)
			if (response[i - 4] == '\r' && response[i - 2] == '\r' && response[i - 1] == '\n' && response[i - 3] == '\n')
			{
				break;
			}

	}
	else
	{

		// 发送数据
		int resultCode = send(sock_, reques.c_str(), reques.size(), 0);
		if (resultCode == -1)
		{
			std::cerr << "error send" << std::endl;
			throw std::runtime_error("error send");
		}

		// 接收数据
		bytes = recv(sock_, buffer, 1024, 0);

		char* ptr = buffer + 9;

		status = 100 * (*(buffer + 9) - '0') + 10 * (*(buffer + 10) - '0') + *(buffer + 11) - '0';

		while (bytes > 0)
		{
			response += buffer;
			bytes = recv(sock_, buffer, 1024, 0);
		}


		position = 4;
		for (int i = 4; i <= response.size(); ++i, ++position)
			if (response[i - 4] == '\r' && response[i - 2] == '\r' && response[i - 1] == '\n' && response[i - 3] == '\n')
			{
				break;
			}

	}

	return std::move(response);

}

results Net::request(const char* hostname, const char* port)
{

	size_t size= 0;
	for (auto i = hostname; *i != '\0'; ++i)
		++size;

	char* ptr = new char[size + 1]{};
	mPtr<char> host{ ptr,2 };

	for (auto i = hostname; *i != '\0'; ++i)
		*ptr++ = *i;
	*ptr = '\0';
	ptr = nullptr;


	const char* getrequest = nullptr;

	auto arr = split(hostname, '/');
	if (arr.size() > 1)
	{	

		if (arr.size() >= 3)
		{
			getrequest = hostname + arr[2];

			host = mPtr<char>{ new char[arr[2] - arr[1]] {} };

			char* j = host.getPtr();
			for (const char* i = hostname + arr[1]; i != hostname + arr[2] - 1; ++i, ++j)
			{
				*j = *i;
			}
			*j = '\0';
			j = nullptr;
		}


		if (arr[1] <= 7)
			port = "80";

	}

	// 设置请求头
	mString reques = "GET /";
	if (getrequest && *getrequest != '\0')
		reques += getrequest;

	reques += " HTTP/1.1\r\n";
	reques += "Host: ";
	reques += host.getPtr();
	reques += "\r\nConnection: close\r\n\r\n";

	std::string response{};

	if (!sockConnect(host.getPtr(), port))
	{
		std::cerr << "Error connect socket" << std::endl;
		throw std::runtime_error("Error connect socket");
	}

	int status = -1, pos_ = -1;


	if (*port != '8' && *(port + 1) != '0') {
		if (!sslConnect())
		{
			std::cerr << "Error connect SSL socket" << std::endl;
			throw std::runtime_error("Error connect socket");
		}

		response = read_write(reques, true, status, pos_);
	}
	else
	{
		response = read_write(reques, false, status, pos_);
	}



	return results(std::string(response.substr(pos_)), std::string(response.substr(0, pos_)), status);
}

bool Net::sslConnect()
{

	SSL_CTX* ssl_ctx = SSL_CTX_new(TLS_client_method());
	if (!ssl_ctx)
	{
		std::cerr << "Error creating SSL context" << std::endl;
		return false;
	}

	// 创建 SSL 套接字并连接到服务器
	if (sockSsl_!=nullptr)
	{
		SSL_shutdown(sockSsl_);
		SSL_free(sockSsl_);
		sockSsl_ = nullptr;
	}

	sockSsl_ = SSL_new(ssl_ctx);

	if (!sockSsl_)
	{
		std::cerr << "Error creating SSL socket" << std::endl;
		return false;
	}

	if (SSL_set_fd(sockSsl_, sock_) == 0)
	{
		std::cerr << "Error setting SSL file descriptor" << std::endl;
		return false;
	}

	if (SSL_connect(sockSsl_) <= 0)
	{
		std::cerr << "Error connecting SSL" << std::endl;
		return false;
	}

	SSL_CTX_free(ssl_ctx);

	return true;

}


void Net::useAgree(bool client)
{


	switch (agree_)
	{
	case AGREEMENT::Ipv4:
		hint_.ai_family = AF_INET;
		break;
	case AGREEMENT::Ipv6:
		hint_.ai_family = AF_INET6;
		break;
	case AGREEMENT::NoAgreement:
		hint_.ai_family = AF_UNSPEC;
		break;
	default:
		break;
	}


	switch (ipAgree_)
	{
	case IP_AGREEMENT::Ipv4:
		hint_.ai_protocol = IPPROTO_IPV4;
		break;
	case IP_AGREEMENT::Ipv6:
		hint_.ai_protocol = IPPROTO_IPV6;
		break;
	case IP_AGREEMENT::TCP:
		hint_.ai_protocol = IPPROTO_TCP;
		hint_.ai_socktype = SOCK_STREAM;
		break;
	case IP_AGREEMENT::UDP:
		hint_.ai_protocol = IPPROTO_UDP;
		hint_.ai_socktype = SOCK_DGRAM;
		break;
	default:
		break;
	}


}

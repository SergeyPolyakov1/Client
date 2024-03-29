#include "stdafx.h"
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<iostream>
#include<string>
#include<fstream>
#pragma warning(disable: 4996)
SOCKET Connection;

std::string readStringFromFile(std::string fileName)
{
	std::ifstream file;
	file.open(fileName);
	std::string result_string;

	if (!file.is_open())
	{
		std::cout << "Error open file!\n";
	}
	else
	{
		while (!file.eof())
		{
			std::string temp_string;
			file >> temp_string;
			result_string += temp_string + " ";
		}
	}
	file.close();
	return result_string;
}

void clientHandler()
{
	int size_answer_from_server;

	while (true)
	{
		recv(Connection, (char*)&size_answer_from_server, sizeof(int), NULL);
		char*answer_from_server = new char[size_answer_from_server + 1];
		recv(Connection, answer_from_server, size_answer_from_server, NULL);
		std::cout << answer_from_server <<std::endl;
	}
}

int main(int argc, char*argv[])
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		std::cout << "error " << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int	sizeOfAddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	 Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection,(SOCKADDR*)&addr,sizeof(addr)) != 0)
	{
		std::cout << "Failed connect to server!\n";
		return 1;
	}
	std::cout << "connected!\n";


	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandler, NULL, NULL, NULL);

	std::string msg,fname;
	while (true)
	{
		std::cout << "Enter file name\n";
		std::cin >> fname;
		msg = readStringFromFile(fname);
		int msg_size = msg.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg.c_str(), msg_size, NULL);
		Sleep(10);
	}

	system("pause");
	return 0;
}
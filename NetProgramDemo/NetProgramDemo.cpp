// NetProgramDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib,"iphlpapi")
#pragma comment(lib,"Ws2_32")

void prtIP(ULONG ulHostIP) {
	ULONG ip1 = ulHostIP, ip2 = ulHostIP, ip3 = ulHostIP, ip4 = ulHostIP;


	ip1 = ip1 >> 24;
	ip2 = (ip2 << 8) >> 24;
	ip3 = (ip3 << 16) >> 24;
	ip4 = (ip4 << 24) >> 24;

	std::cout << ip1 << "." << ip2 << "." << ip3 << "." << ip4 << std::endl;
}
void getIPv4(ULONG ulHostIP, ULONG* retIP1, ULONG* retIP2, ULONG* retIP3, ULONG* retIP4) {
	ULONG ip1 = ulHostIP, ip2 = ulHostIP, ip3 = ulHostIP, ip4 = ulHostIP;


	*retIP1 = ip1 >> 24;
	*retIP2 = (ip2 << 8) >> 24;
	*retIP3 = (ip3 << 16) >> 24;
	*retIP4 = (ip4 << 24) >> 24;

	std::cout << *retIP1 << "." << *retIP2 << "." << *retIP3 << "." << *retIP4 << std::endl;
}
void searchIPTable(ULONG dwSize, MIB_IPADDRTABLE* pIPAddrTable) {
	for (int i = 0; i < dwSize; i++)
	{
		ULONG ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
		ULONG ulHostIP = ntohl(pIPAddrTable->table[i].dwAddr);
		MIB_IPADDRROW CorrentIpTable = pIPAddrTable->table[i];
		std::cout << "\n第" << i << "个IP地址为：";
		getIPv4(ulHostIP, &ip1, &ip2, &ip3, &ip4);
		std::cout << ulHostIP << std::endl;
	}
}

int main()
{
	MIB_IPADDRTABLE* pIPAddrTable = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE));
	ULONG dwSize = 0, dwRetVel = 0;
	if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
		free(pIPAddrTable);
		//重新申请返回条数的IP表
		pIPAddrTable = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE)*dwSize);
	}
	if ((dwRetVel = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) == NO_ERROR) {
		//ULONG ulHostIP = ntohl(pIPAddrTable->table[2].dwAddr);
		ULONG ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
		DWORD d = pIPAddrTable->dwNumEntries;
		std::cout << "\nIPTable中dwNumEntries为：" << d << "\n";
		std::cout << "\nIPTable中共有" << dwSize << "条数据\n";
		
		//std::cout << ulHostIP << std::endl;
		//printf();
		//prtIP(ulHostIP);
		
		
		//std::cout << ip1 << "-" << ip2 << "-" << ip3 << "-" << ip4 << std::endl;
	}
	
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

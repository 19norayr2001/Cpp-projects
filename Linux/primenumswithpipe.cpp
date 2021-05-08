#include <iostream>
#include <cstring>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
void failure() { std::cout << strerror(errno) << std::endl; }
int min(int a, int b) { return (a < b ? a: b); }
void close_desc(const std::vector<int>& fd)
{
	int size = fd.size();
	for(int i = size - 1; i >= 0; --i)
	{
		close(fd[i]);
	}
}
bool CheckPrime(int p)
{
	for(int i = 2; i * i <= p; ++i)
	{
		if(p % i == 0)
		{
			return false;
		}
	}
	return p != 1;
}
void CalculateWrite(int left, int right, int fd)
{
	int cnt = 0;
	for(int i = left; i <= right; ++i)
	{
		cnt += CheckPrime(i);
	}
	sleep(1);
	const int size = 10;
	char buffer[size];
	sprintf(buffer, "%d", cnt);
	if(write(fd, buffer, size) == -1)
	{
		failure();
		return;
	}
}
int ReadValue(int fd){

	const int size = 10;
	char buffer[size];
	int r = -1, pos = 0;
	while(r != 0)
	{
		std::cout << r << std::endl;
		r = read(fd, buffer + pos, size);
		if(r == -1)
		{
			failure();
			return -1;
		}
		pos += r;
	}
	return atoi(buffer);
}
			
int main()
{
	int left = 0, right = 0, m = 0;
	std::cin >> left >> right >> m;
	std::vector<int> fd;
	for(int i = 0; i < m; ++i)
	{
		const int size = 15;
		char buffer[size] = "prime";
		sprintf(buffer + 5, "%d", i);
		int cr = mkfifo(buffer, 0700);
		if(cr == -1 && errno != EEXIST)
		{
			failure();
			return 0;
		}
	}
	int lf = 0, rg = 0;
	int sectorsize = (right - left + 1) / m;
	int count = 0;
	for(int i = 0; i < m; ++i)
	{
		lf = left + sectorsize * i;
		rg = lf + sectorsize - 1;
		if(i == m - 1)
			rg = right;
		int id = fork();
		const int size = 15;
		char buffer[size] = "prime";
		sprintf(buffer + 5, "%d", i);
		if(id == 0)
		{
			int pd = open(buffer, O_WRONLY);
			std::cout << buffer <<" " << pd << " " << i << std::endl;
			CalculateWrite(lf, rg, pd);
			std::cout << i << "process" << pd<<" \n";
			close(pd);
			return 0;
		}
	}
	for(int i = 0; i < m; ++i)
	{
		const int size = 15;
		char buffer[size] = "prime";
		sprintf(buffer + 5, "%d", i);
		int fdi = open(buffer, O_RDONLY);
		if(fdi == -1)
		{
			failure();
			close_desc(fd);
			if(wait(nullptr) == -1) { failure();}
			return 0;
		}
		fd.push_back(fdi);
	}
	std::cout << 1 << std::endl;
	for(int i = 0; i < m; ++i)
	{
		int cnt = ReadValue(fd[i]);
		if(cnt == -1)
		{
			failure();
			close_desc(fd);
			if(wait(nullptr) == -1)
			{
				failure();
			}
			return 0;
		}
		count += cnt;
	}
	if(wait(nullptr) == -1)
	{
		failure();
		return 0;
	}
	std::cout << count << std::endl << std::flush;
	close_desc(fd);
	return 0;
}

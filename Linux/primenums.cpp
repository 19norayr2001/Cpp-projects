#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cstring>
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
	const int size = 10;
	char buffer[size];
	sprintf(buffer, "%d", cnt);
	if(write(fd, buffer, size) == -1)
	{
		failure();
		return;
	}
	std::cout << cnt << "precess\n";
}
int ReadValue(int fd)
{
	const int size = 10;
	char buffer[size];
	if(read(fd, buffer, size) == -1)
	{
		failure();
		return -1;
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
		int fdi = open(buffer, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
		if(fdi == -1)
		{
			failure();
			close_desc(fd);
			return 0;
		}
		fd.push_back(fdi);
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
		if(id == 0)
		{
			CalculateWrite(lf, rg, fd[i]);			
			return 0;
		}
	}
	wait(nullptr);
	for(int i = 0; i < m; ++i)
	{
		lseek(fd[i], 0, SEEK_SET);
		int cnt = ReadValue(fd[i]);
		if(cnt == -1)
		{
			failure();
			close_desc(fd);
			return 0;
		}
		count += cnt;
	}
	std::cout << count << std::endl;
	close_desc(fd);
	return 0;
}

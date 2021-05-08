#include <iostream>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int Factorial(int n)
{
       	if(n <= 1)
	       	return 1;
       	return n*Factorial(n - 1);
}
void failure() { std::cout << strerror(errno) << std::endl;}
void closefd(std::vector<int>& fd, int except = -1)
{
	int size = fd.size();
	for(int i =  size - 1; i >= 0; --i)
	{
		if(except != fd[i])
		{
			close(fd[i]);
		}
	}
}
bool MakePipe(int mypipe[2], std::vector<int>& fd)
{
	if(pipe(mypipe) == -1)
	{
		failure();
		closefd(fd);
		return false;
	}
	fd.push_back(mypipe[0]);
	fd.push_back(mypipe[1]);
	return true;
}
void CalculateWrite(int n, int fd)
{
	const int size = 10;
	char buffer[size];
	int value = Factorial(n);
	sprintf(buffer, "%d", value);
	int w = write(fd, buffer, size);
	if(w == -1)
	{
		failure();
	}
	close(fd);
}	
int ReadValue(int fd)
{
	const int size = 10;
	char buffer[size];
	int r = -1;
	int readsize = 0;
	while(r != 0)
	{
		r = read(fd, buffer + readsize, size);
		readsize += r;
		if(r == -1)
		{
			failure();
			return -1;
		}
	}
	return std::stoi(buffer);
}	
int main()
{
	int n = 0, k = 0, nk = 0;
	std::vector<int> fd;
	std::cin >> n >> k;
	nk = n - k;
	int npipe[2], kpipe[2], nkpipe[2];
	if(!MakePipe(npipe, fd))
	{
		return 0;
	}	
	if(!MakePipe(kpipe, fd))
	{
		return 0;
	}
	if(!MakePipe(nkpipe, fd))
	{
		return 0;
	}
	int child_id = fork();
	if(child_id == 0)
	{	
		closefd(fd, npipe[1]);
		CalculateWrite(n, npipe[1]);
		return 0;
	}
	child_id = fork();
	if(child_id == 0)
	{
		closefd(fd, kpipe[1]);
		CalculateWrite(k, kpipe[1]);
		return 0;
	}
	child_id = fork();
	if(child_id == 0)
	{	
		closefd(fd, nkpipe[1]);
		CalculateWrite(nk, nkpipe[1]);
		return 0;
	}
	close(npipe[1]);
	close(kpipe[1]);
	close(nkpipe[1]);
	int nvalue = ReadValue(npipe[0]);
	int kvalue = ReadValue(kpipe[0]);
	int nkvalue = ReadValue(nkpipe[0]);
	if(nvalue == -1 || kvalue == -1 || nkvalue == -1)
	{
		std::cout << -1 << std::endl;
	}
	else
	{
		std::cout << nvalue / kvalue / nkvalue << std::endl;
	}
	wait(nullptr);
	close(npipe[0]);
	close(kpipe[0]);
	close(nkpipe[0]);
	return 0;
}

#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <vector> 

int max(int a, int b) { return (a > b ? a: b); }
int min(int a, int b) { return (a < b ? a: b); }
void failure() { std::cout << strerror(errno) << std:: endl; }
void close_desc(const std::vector<int>& dfs)
{
	int size = dfs.size();
	for(int i =  size - 1; i >= 0; --i)
	{
		close(dfs[i]);
	}
}
bool copy(const char* name1, const char* name2, int n)
{
	std::vector<int> dfs;
	int fd1 = open(name1, O_RDONLY);
	if(fd1 == -1)
	{
		failure();
		close_desc(dfs);
		return false;
	}
	dfs.push_back(fd1);
	int fd2 = open(name2, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if(fd2 == -1)
	{
		failure();
		close_desc(dfs);
		return false;
	}
	dfs.push_back(fd2);
	int size = 10;
	char buffer[size];
	off_t filesize = lseek(fd1, -1, SEEK_END);
	if(filesize == -1)
	{
		failure();
		close_desc(dfs);
		return false;
	}
	int count = 0, r = 0, w = 0;
	off_t cur = 0;
	while(filesize != 0 && count != n)
	{
		cur = lseek(fd1, max(0, filesize - size), SEEK_SET);
		if(cur == -1)
		{
			failure();
			close_desc(dfs);
			return false;
		}
		r = read(fd1, buffer, min(size, filesize - cur));
		if(r == -1)
		{
			failure();
			close_desc(dfs);
			return false;
		}
		int i = r - 1;
		for(; i >= 0; --i)
		{
			if(buffer[i] == '\n')
				count++;
			if(count == n)
				break;
		}
		r = r - 1 - i;
		w = write(fd2, buffer + i + 1, r);
		if(w != r)
		{
			failure();
			close_desc(dfs);
			return false;
		}
		filesize = cur;
	}
	close_desc(dfs);
	return true;
}



int main(int argc, char* argv[])
{
	int n = 0;
	std::cin >> n;
	if(argc >= 3 && copy(argv[1], argv[2], n))
	{
		std::cout << "Success" << std::endl;
	}
	else
	{
		std::cout << "Failure" << std::endl;
	}
	return 0;
}

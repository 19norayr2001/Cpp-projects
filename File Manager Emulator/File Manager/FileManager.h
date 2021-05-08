#pragma once
#include <iostream>
#include <vector>
#include <string>

class File;
class Document;
class Link;
class DynamicLink;
class HardLink;
class Directory;

class File
{
public:
	File(const std::string&, Directory* const);
	File(const File&) = delete;
	File& operator=(const File&) = delete;
	const std::string& GetName() const { return m_name; }
	virtual int HardLinkCount() const = 0;
	virtual ~File() {}
protected:
	Directory* const GetRoot();
	const Directory* const GetRoot() const;
protected:
	std::string m_name;
	Directory* const m_root;
};

class Directory :public File
{
public:
	Directory(const std::string&, Directory* const = nullptr);
	Directory(const Directory&) = delete;
	Directory& operator=(const Directory&) = delete;
	~Directory();
private:
	void Deallocate();
public:
	void print(std::ostream&, int = 0) const;
public:
	void AddFile(File*);
	void AddFolder(const std::string&);
	void AddDocument(const std::string&);
	void AddHardLink(const std::string&, Document*, Directory*);
	void AddDynamicLink(const std::string&, Document*, Directory*);
public:
	void DeleteFolder(const std::string&, const Directory* const);
	void DeleteTree(const std::string&, const Directory* const);
	void DeleteFile(const std::string&);
	void DeleteDynamicLink(const Document* const);
	void RemoveFile(const File* const);
public:
	void IncrementHardLinkCount(const std::string&);
	void DecrementHardLinkCount(const std::string&);
public:
	void Copy(const File* const);
	void Move(File* const, std::string);
public:
	File* SearchFile(const std::string&);
	const File* SearchFile(const std::string&) const;
	Directory* SearchFolder(const std::string&);
	Document* SearchDocument(const std::string&);
	DynamicLink* SearchDynamicLink(const Document* const);
	bool SearchDynamicLink(const DynamicLink* const);
public:
	bool empty() const { return m_Files.size() == 0; }
	virtual int HardLinkCount() const { return m_HardLinkCount; }
private:
	void DeleteObject(int);
	bool HasFileWithSameName(const std::string&) const;
	void ModifyDuringMove(File* const, std::string);
	bool CheckTree(const Directory*, const Directory* const);
	void IncrementHardLinkCount(const std::vector<std::string>&, int = 1);
	void DecrementHardLinkCount(const std::vector<std::string>&, int = 1);
	Directory* SearchFolder(const std::vector<std::string>&, int = 0);
private:
	std::vector<File*> m_Files;
	int m_HardLinkCount;
};

class Document :public File
{
public:
	Document(const std::string&, Directory* const);
	~Document();
public:
	void ChangePath(const std::string&);
public:
	void AddHardLink() { ++m_HardLinkCount; }
	void RemoveHardLink() { --m_HardLinkCount; }
	bool CheckHardLinkCount() const { return m_HardLinkCount == 0; }
	virtual int HardLinkCount() const { return m_HardLinkCount; }
public:
	void AddDynamicLink(Directory* parent) { m_DynamicLinks.push_back(parent); }
	Directory* FindParentDynamicLink(const DynamicLink* const) const;
	void RemoveDynamicLink(Directory*);
private:
	void Deallocate();
private:
	std::vector<Directory*> m_DynamicLinks;
	int m_HardLinkCount;
};

class Link : public File
{
public:
	Link(const std::string&, Document*, Directory* const);
	std::string GetPath() const;
	Document* GetFile() const { return m_file; }
	virtual int HardLinkCount() const { return 0; }
private:
	Document* const m_file;
};

class DynamicLink :public Link 
{
public:
	DynamicLink(const std::string&, Document*, Directory* const);
	~DynamicLink();
	void ChangePath(const std::string& path) { m_name = "dlink[" + path + "]"; }
private:
	void Deallocate();
};

class HardLink :public Link
{
public:
	HardLink(const std::string&, Document*, Directory* const);
	~HardLink();
private:
	void Deallocate();
};

class FileManager
{
	typedef std::vector<std::string> StringVector;
	typedef void(FileManager::* OneArgCommandFunction)(const std::string&);
	typedef void(FileManager::* TwoArgCommandFunction)(const std::string&, const std::string&);
	friend std::ostream& operator<<(std::ostream&, const FileManager&);
	friend std::istream& operator>>(std::istream&, FileManager&);
public:
	FileManager();
	FileManager(const FileManager&) = delete;
	FileManager& operator=(const FileManager&) = delete;
	~FileManager();
private:
	static OneArgCommandFunction CheckOneArgumentCommand(const std::string&, int);
	static TwoArgCommandFunction CheckTwoArgumentCommand(const std::string&, int);
private:
	void Command(const std::string&);
	void ChangeDirectory(const std::string&);
	void MakeDirectory(const std::string&);
	void RemoveDirectory(const std::string&);
	void DeleteTree(const std::string&);
	void MakeFile(const std::string&);
	void MakeHardLink(const std::string&, const std::string&);
	void MakeDynamicLink(const std::string&, const std::string&);
	void Delete(const std::string&);
	void Copy(const std::string&, const std::string&);
	void Move(const std::string&, const std::string&);
private:
	void LinkCase(const std::string&, const std::string&, Directory*&, Document*&);
	std::string GetFullPath(const std::string&) const;
	Directory* GetParent(const std::string&, std::string&);
	Directory* GetParent(const std::string&);
private:
	Directory*const m_root;
	Directory* m_CurrentDirectory;
	std::string m_CurrentDirectoryPath;
};
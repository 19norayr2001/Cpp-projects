#include "FileManager.h"
#include <exception>
#include <cctype>

namespace
{
	typedef std::vector<std::string> VectorString;

	void ToLower(std::string& s)
	{
		for (char& elem : s)
		{
			elem = tolower(elem);
		}
	}

	int Drop(int x)
	{
		return x == '/';
	}

	void Concatenate(std::vector<std::string>& vec1, const std::vector<std::string>& vec2)
	{
		for (const std::string& s: vec2)
		{
			vec1.push_back(s);
		}
	}

	bool IsFullPath(const std::string& path)
	{
		return path.size() >= 2 && path[0] == 'C' && path[1] == ':';
	}

	bool IsPrefix(const std::string& first, const std::string& second)
	{
		if (first.size() > second.size())
			return false;
		for (size_t i = 0; i < first.size(); ++i)
		{
			if (first[i] != second[i])
			{
				return false;
			}
		}
		return true;
	}

	std::string GetLastComponent(const std::string& path)
	{
		int len = path.size();
		std::string ret;
		int i = len - 1;
		bool bracket = false;
		for (; i >= 0; --i)
		{
			if (path[i] == ']')
				bracket = true;
			if (path[i] == '[')
				bracket = false;
			if (bracket)
				continue;
			if (path[i] == '/')
				break;
		}
		for (i = i + 1; i < len; ++i)
			ret += path[i];
		return ret;
	}

	std::string SubString(const std::string& str, int left, int right)
	{
		return str.substr(left, str.size() - left - right);
	}

	void ParsingCommands(VectorString& args, const std::string& str, int(*predicat)(int))
	{
		std::string next_command;
		int len = str.size();
		bool flag = false;
		for (int i = 0; i < len; ++i)
		{
			if (str[i] == ']')
				flag = true;
			if(str[i] == '[')
				flag = false;
			if (!predicat(str[i]) || flag)
			{
				next_command += str[i];
			}
			else
			{
				if (!next_command.empty())
				{
					args.push_back(next_command);
					next_command = "";
				}
				while (i < len && predicat(str[i]))
				{
					i++;
				}
				i--;
			}
		}
		if (!next_command.empty())
		{
			args.push_back(next_command);
		}
	}

	void GetArgs(VectorString& args, const std::string& command)
	{
		ParsingCommands(args, command, isspace);
		if (!args.empty())
		{
			ToLower(args[0]);
		}
	}

	void GetPathArgs(VectorString& files, const std::string& path)
	{
		ParsingCommands(files, path, Drop);
	}

	void PrintNode(const File* file, std::ostream& out, int layer = 0)
	{
		if (layer != 0)
			out << '|';
		for (int i = 0; i < layer - 1; ++i)
			out << "\t|";
		if (layer != 0)
			out << '_';
		out << file->GetName();
		const Directory* folder = dynamic_cast<const Directory*>(file);
		if (folder == nullptr)
		{
			out << std::endl;
			return;
		}
		out << std::endl;
		folder->print(out, layer + 1);
	}
}

std::ostream& operator<<(std::ostream& out, const FileManager& filemanager)
{
	const Directory* root = filemanager.m_root;
	PrintNode(root, out);
	return out;
}

std::istream& operator>>(std::istream& in, FileManager& filemanager)
{
	std::string s;
	getline(in, s);
	filemanager.Command(s);
	return in;
}

FileManager::FileManager()
	:m_root(new Directory("C:"))
	, m_CurrentDirectory(m_root)
	, m_CurrentDirectoryPath("C:")
{}

FileManager::~FileManager()
{
	delete m_root;
}

FileManager::OneArgCommandFunction FileManager::CheckOneArgumentCommand(const std::string& command, int count)
{
	const static StringVector command_names{ "md", "cd", "rd", "deltree", "mf", "del", };
	const static std::vector<OneArgCommandFunction> Functions
	{
		&FileManager::MakeDirectory,
		&FileManager::ChangeDirectory,
		&FileManager::RemoveDirectory,
		&FileManager::DeleteTree,
		&FileManager::MakeFile,
		&FileManager::Delete,
	};
	int len = command_names.size();
	for (int i = 0; i < len; ++i)
	{
		if (command_names[i] == command)
		{
			if (count == 1)
				return Functions[i];
			else if (count > 1)
				throw std::exception("Too many arguments");
			else
				throw std::exception("Mising argument");
		}
	}
	return nullptr;
}

FileManager::TwoArgCommandFunction FileManager::CheckTwoArgumentCommand(const std::string& command, int count)
{
	const static StringVector command_names{ "mhl", "mdl", "copy", "move" };
	const static std::vector<TwoArgCommandFunction> Functions
	{
		&FileManager::MakeHardLink,
		&FileManager::MakeDynamicLink,
		&FileManager::Copy,
		&FileManager::Move,
	};
	int len = command_names.size();
	for (int i = 0; i < len; ++i)
	{
		if (command_names[i] == command)
		{
			if (count == 2)
				return Functions[i];
			else if (count > 2)
				throw std::exception("Too many arguments");
			else
				throw std::exception("Mising argument");
		}
	}
	return nullptr;
}

void FileManager::Command(const std::string& command)
{
	StringVector args;
	GetArgs(args, command);
	if (args.empty())
		return;
	const std::string& first = args[0];
	int count = args.size() - 1;
	OneArgCommandFunction func1 = CheckOneArgumentCommand(first, count);
	if (func1 != nullptr)
	{
		(this->*func1)(args[1]);
		return;
	}
	TwoArgCommandFunction func2 = CheckTwoArgumentCommand(first, count);
	if (func2 != nullptr)
	{
		(this->*func2)(args[1], args[2]);
		return;
	}
	throw std::exception("Command not found");
}

void FileManager::ChangeDirectory(const std::string& path)
{
	bool pathflag = IsFullPath(path);
	const std::string& my_path = (pathflag ? SubString(path, 2, 0) : path);
	Directory* cur = (pathflag ? m_root : m_CurrentDirectory);
	m_CurrentDirectory = cur->SearchFolder(my_path);
	if (pathflag)
		m_CurrentDirectoryPath = path;
	else
		m_CurrentDirectoryPath += "/" + path;
}

void FileManager::MakeDirectory(const std::string& path)
{
	std::string FileName;
	Directory* parent = GetParent(path, FileName);
	parent->AddFolder(FileName);
}

void FileManager::RemoveDirectory(const std::string& path)
{
	if (path == "C:")
		throw std::exception("Not allowed remove root");
	std::string FileName;
	Directory* parent = GetParent(path, FileName);
	parent->DeleteFolder(FileName, m_CurrentDirectory);
}

void FileManager::DeleteTree(const std::string& path)
{
	if (path == "C:")
		throw std::exception("Not allowed remove root");
	std::string FileName;
	Directory* parent = GetParent(path, FileName);
	parent->DeleteTree(FileName, m_CurrentDirectory);
}

void FileManager::MakeFile(const std::string& path)
{
	std::string FileName;
	Directory* parent = GetParent(path, FileName);
	parent->AddDocument(FileName);
}

void FileManager::MakeHardLink(const std::string& document_path, const std::string& link_path)
{
	Document* document{};
	Directory* link_location{};
	const std::string& full_path = GetFullPath(document_path);
	LinkCase(document_path, link_path, link_location, document);
	link_location->AddHardLink(full_path, document, m_root);
}

void FileManager::MakeDynamicLink(const std::string& document_path, const std::string& link_path)
{
	Document* document{};
	Directory* link_location{};
	const std::string& full_path = GetFullPath(document_path);
	LinkCase(document_path, link_path, link_location, document);
	link_location->AddDynamicLink(full_path, document, m_root);
}

void FileManager::Delete(const std::string& path)
{
	if (path == "C:")
		throw std::exception("Is a directory");
	std::string FileName;
	Directory* parent = GetParent(path, FileName);
	parent->DeleteFile(FileName);
}

void FileManager::Copy(const std::string& file_path, const std::string& loc_path)
{
	if (file_path == "C:")
		throw std::exception("Not allowed make directories with name C:");
	std::string FileName;
	Directory* parent = GetParent(file_path, FileName);
	const File* const file = parent->SearchFile(FileName);
	Directory* location = GetParent(loc_path);
	location->Copy(file);
}
void FileManager::Move(const std::string& file_path, const std::string& loc_path)
{
	if (file_path == "C:")
		throw std::exception("Not allowed move root directory");
	if (file_path == loc_path)
		return;
	if (IsPrefix(GetFullPath(file_path), GetFullPath(loc_path)))
		throw std::exception("Command not allowed");
	std::string FileName;
	Directory* parent = GetParent(file_path, FileName);
	File* file = parent->SearchFile(FileName);
	Directory* location = GetParent(loc_path);
	location->Move(file, GetFullPath(loc_path));
	parent->RemoveFile(file);
}

void FileManager::LinkCase(
	const std::string& document_path,
	const std::string& link_path,
	Directory*& link_location,
	Document*& document)
{
	std::string FileName;
	Directory* parent = GetParent(document_path, FileName);
	document = parent->SearchDocument(FileName);
	link_location = GetParent(link_path);
}

std::string FileManager::GetFullPath(const std::string& path) const
{
	return (IsFullPath(path) ? "" : m_CurrentDirectoryPath) + "/" + path;
}

Directory* FileManager::GetParent(const std::string& path, std::string& FileName)
{
	bool pathflag = IsFullPath(path);
	Directory* cur = (pathflag ? m_root : m_CurrentDirectory);
	FileName = GetLastComponent(path);
	return cur->SearchFolder(SubString(path, 2 * pathflag, FileName.size()));
}

Directory* FileManager::GetParent(const std::string& path)
{
	bool pathflag = IsFullPath(path);
	Directory* cur = (pathflag ? m_root : m_CurrentDirectory);
	return cur->SearchFolder(SubString(path, 2 * pathflag, 0));
}

File::File(const std::string& filename, Directory* const root)
	:m_name(filename)
	, m_root(root)
{}

Directory* const File::GetRoot() 
{ 
	return (m_root == nullptr ? dynamic_cast<Directory* const>(this) : m_root); 
}

const Directory* const File::GetRoot() const 
{ 
	return (m_root == nullptr ? dynamic_cast<const Directory* const>(this) : m_root); 
}

Directory::Directory(const std::string& filename, Directory* const root)
	:File(filename, root)
	, m_HardLinkCount(0)
{}

Directory::~Directory() { Deallocate(); }

void Directory::Deallocate()
{
	int len = m_Files.size();
	for (int i = 0; i < len; ++i)
	{
		delete m_Files[i];
		m_Files[i] = nullptr;
	}
}

void Directory::print(std::ostream& out, int layer) const
{
	int len = m_Files.size();
	for (int i = 0; i < len; ++i)
	{
		PrintNode(m_Files[i], out, layer);
	}
}

void Directory::AddFile(File* file)
{
	if (!HasFileWithSameName(file->GetName()))
	{
		m_Files.push_back(file);
	}
}

void Directory::AddFolder(const std::string& name)
{
	if (name == "C:")
		throw std::exception("Not allowed make directories with name C:");
	if (name.empty())
		throw std::exception("Not allowed make directories with empty names");
	if (!HasFileWithSameName(name))
	{
		m_Files.push_back(new Directory(name, GetRoot()));
	}
}

void Directory::AddDocument(const std::string& name)
{
	if (!HasFileWithSameName(name))
	{
		m_Files.push_back(new Document(name, GetRoot()));
	}
}

void Directory::AddHardLink(const std::string& path, Document* file, Directory* root)
{
	m_Files.push_back(new HardLink(path, file, root));
}

void Directory::AddDynamicLink(const std::string& path, Document* file, Directory* const root)
{
	m_Files.push_back(new DynamicLink(path, file, root));
	file->AddDynamicLink(this);
}

void Directory::DeleteFolder(const std::string& name, const Directory*const cur)
{
	int len = m_Files.size();
	for (int i = 0; i < len; ++i)
	{
		File* file = m_Files[i];
		if (file->GetName() == name)
		{
			Directory* myfolder = dynamic_cast<Directory*>(file);
			if (myfolder == nullptr)
				throw std::exception("Not a directory");
			else if (!myfolder->empty())
				throw std::exception("Directory not empty");
			else if (myfolder == cur)
				throw std::exception("Not allowed remove current directory");
			else
				DeleteObject(i);
			return;
		}
	}
	throw std::exception("No such file or directory");
}

void Directory::DeleteTree(const std::string& name, const Directory* const cur)
{
	int len = m_Files.size();
	for (int i = 0; i < len; ++i)
	{
		File* file = m_Files[i];
		if (file->GetName() == name)
		{
			Directory* myfolder = dynamic_cast<Directory*>(file);
			if (myfolder == nullptr)
				throw std::exception("Not a directory");
			else if (!CheckTree(myfolder, cur))
				throw std::exception("Not allowed remove current directory");
			else if (m_HardLinkCount > 0)
				throw std::exception("Not allowed remove folder containing hard links");
			else
				DeleteObject(i);
			return;
		}
	}
	throw std::exception("No such file or directory");
}

void Directory::DeleteFile(const std::string& name)
{
	int len = m_Files.size();
	for (int i = 0; i < len; ++i)
	{
		if (m_Files[i]->GetName() == name)
		{
			Directory* folderptr = dynamic_cast<Directory*>(m_Files[i]);
			if (folderptr != nullptr)
			{
				throw std::exception("Is a directory");
			}
			Document* doc = dynamic_cast<Document*>(m_Files[i]);
			if (doc != nullptr && !doc->CheckHardLinkCount()) 
			{
				throw std::exception("File contains hard links");
			}
			DeleteObject(i);
			return;
		}
	}
}

void Directory::DeleteDynamicLink(const Document* const file)
{
	int len = m_Files.size();
	for (int i = 0; i < len; ++i)
	{
		DynamicLink* dlink = dynamic_cast<DynamicLink*>(m_Files[i]);
		if (dlink != nullptr && dlink->GetFile() == file)
		{
			DeleteObject(i);
			return;
		}
	}
}

void Directory::RemoveFile(const File* const file)
{
	for (size_t i = 0; i < m_Files.size(); ++i)
	{
		if (m_Files[i] == file)
		{
			m_Files.erase(m_Files.begin() + i);
			return;
		}
	}
}

void Directory::IncrementHardLinkCount(const std::string& path)
{
	VectorString pathvector;
	GetPathArgs(pathvector, path);
	IncrementHardLinkCount(pathvector);
}

void Directory::DecrementHardLinkCount(const std::string& path)
{
	std::vector<std::string> pathvector;
	GetPathArgs(pathvector, path);
	DecrementHardLinkCount(pathvector);
}

void Directory::Copy(const File* const file)
{
	if (HasFileWithSameName(file->GetName())) 
	{
		return;
	}
	const Document* const doc = dynamic_cast<const Document* const>(file);
	if (doc != nullptr)
	{
		this->AddDocument(doc->GetName());
		return;
	}
	const DynamicLink* const dlink = dynamic_cast<const DynamicLink* const>(file);
	if (dlink != nullptr)
	{
		this->AddDynamicLink(dlink->GetPath(), dlink->GetFile(), m_root);
		return;
	}
	const HardLink* const hlink = dynamic_cast<const HardLink* const>(file);
	if (hlink != nullptr)
	{
		this->AddHardLink(hlink->GetPath(), hlink->GetFile(), m_root);
		return;
	}
	const Directory* const folder = dynamic_cast<const Directory* const>(file);
	if (folder != nullptr)
	{
		if (!HasFileWithSameName(folder->GetName()))
		{
			this->AddFolder(folder->GetName());
			Directory* loc = this->SearchFolder(folder->GetName());
			for (const File* const myfile : folder->m_Files)
			{
				loc->Copy(myfile);
			}
		}
		return;
	}
	throw std::exception("Something went wrong");
}

void Directory::Move(File* const myfile, std::string path)
{
	if (myfile->HardLinkCount() > 0)
		throw std::exception("Not allowed move files containing hard links");
	if (HasFileWithSameName(myfile->GetName()))
		return;
	this->AddFile(myfile);
	ModifyDuringMove(myfile, path);
}

Directory* Directory::SearchFolder(const std::string& path)
{
	VectorString path_vector;
	GetPathArgs(path_vector, path);
	return SearchFolder(path_vector);
}

void Directory::IncrementHardLinkCount(const std::vector<std::string>& pathvector, int ind)
{
	m_HardLinkCount++;
	if (ind == pathvector.size() - 1)
	{
		Document* doc = this->SearchDocument(pathvector.back());
		doc->AddHardLink();
		return;
	}
	for (File* file : m_Files)
	{
		Directory* folder = dynamic_cast<Directory*>(file);
		if (folder != nullptr && pathvector[ind] == folder->GetName())
		{
			folder->IncrementHardLinkCount(pathvector, ind + 1);
			return;
		}
	}
}

void Directory::DecrementHardLinkCount(const std::vector<std::string>& pathvector, int ind)
{
	m_HardLinkCount--;
	if (ind == pathvector.size() - 1)
	{
		Document* doc = this->SearchDocument(pathvector.back());
		doc->RemoveHardLink();
		return;
	}
	for (File* file : m_Files)
	{
		Directory* folder = dynamic_cast<Directory*>(file);
		if (folder != nullptr && pathvector[ind] == folder->GetName())
		{
			folder->DecrementHardLinkCount(pathvector, ind + 1);
			return;
		}
	}
}

File* Directory::SearchFile(const std::string& name)
{
	for (File* file : m_Files)
	{
		if (file->GetName() == name)
		{
			return file;
		}
	}
	throw std::exception("No such file or directory");
}

const File* Directory::SearchFile(const std::string& name) const
{
	for (File* file : m_Files)
	{
		if (file->GetName() == name)
		{
			return file;
		}
	}
	throw std::exception("No such file or directory");
}

Directory* Directory::SearchFolder(const std::vector<std::string>& pathvector, int ind)
{
	if (ind == pathvector.size())
		return this;
	for (File* file : m_Files)
	{
		if (file->GetName() == pathvector[ind])
		{
			Directory* folder = dynamic_cast<Directory*>(file);
			if (folder == nullptr)
				throw std::exception("Not a directory");
			else
				return folder->SearchFolder(pathvector, ind + 1);
		}
	}
	throw std::exception("No such file or directory");
}

Document* Directory::SearchDocument(const std::string& name)
{
	for (File* file : m_Files)
	{
		if (file->GetName() == name)
		{
			Document* ptr = dynamic_cast<Document*>(file);
			if (ptr == nullptr)
				throw std::exception("Operation not permitted");
			return ptr;
		}
	}
	throw std::exception("No such file or directory");
}

DynamicLink* Directory::SearchDynamicLink(const Document* const doc)
{
	for (File* file : m_Files)
	{
		DynamicLink* link = dynamic_cast<DynamicLink*>(file);
		if (link != nullptr && link->GetFile() == doc)
		{
			return link;
		}
	}
	throw std::exception("No such file or directory");
}

bool Directory::SearchDynamicLink(const DynamicLink* const link)
{
	for (File* file : m_Files)
	{
		if (file == link)
		{
			return true;
		}
	}
	return false;
}

void Directory::DeleteObject(int i)
{
	delete m_Files[i];
	m_Files.erase(m_Files.begin() + i);
}

bool Directory::HasFileWithSameName(const std::string& name) const
{
	for (const File* const file : m_Files)
	{
		if (file->GetName() == name)
		{
			return true;
		}
	}
	return false;
}

void Directory::ModifyDuringMove(File* const myfile, std::string path)
{
	DynamicLink* dlink = dynamic_cast<DynamicLink*>(myfile);
	if (dlink != nullptr)
	{
		Document* file = dlink->GetFile();
		Directory* parent = file->FindParentDynamicLink(dlink);
		if (parent != this)
		{
			file->RemoveDynamicLink(parent);
			file->AddDynamicLink(this);
		}
		return;
	}
	Document* doc = dynamic_cast<Document*>(myfile);
	if (doc != nullptr)
	{
		doc->ChangePath(path + "/" + doc->GetName());
		return;
	}
	Directory* folder = dynamic_cast<Directory*>(myfile);
	if (folder != nullptr)
	{
		for (File* file : folder->m_Files)
		{
			folder->ModifyDuringMove(file, path + "/" + myfile->GetName());
		}
		return;
	}
}

bool Directory::CheckTree(const Directory* folder, const Directory* const cur)
{
	if (folder == cur)
		return false;
	int len = folder->m_Files.size();
	for (int i = 0; i < len; ++i)
	{
		Directory* folderptr = dynamic_cast<Directory*>(folder->m_Files[i]);
		if (folderptr != nullptr)
		{
			bool flag = CheckTree(folderptr, cur);
			if (!flag)
				return false;
		}
	}
	return true;
}

Document::Document(const std::string& name, Directory* const root)
	:File(name, root)
	, m_HardLinkCount(0)
{}

Document::~Document() { Deallocate(); }

void Document::ChangePath(const std::string& path)
{
	for (Directory* parent : m_DynamicLinks)
	{
		DynamicLink* link = parent->SearchDynamicLink(this);
		link->ChangePath(path);
	}
}

Directory* Document::FindParentDynamicLink(const DynamicLink* const link) const
{
	for (size_t i = 0; i < m_DynamicLinks.size(); ++i)
	{
		if (m_DynamicLinks[i]->SearchDynamicLink(link))
		{
			return m_DynamicLinks[i];
		}
	}
	throw std::exception("Something went wrong");
}

void Document::RemoveDynamicLink(Directory* parent)
{
	int len = m_DynamicLinks.size();
	for (int i = 0; i < len; ++i)
	{
		if (m_DynamicLinks[i] == parent)
		{
			m_DynamicLinks.erase(m_DynamicLinks.begin() + i);
			return;
		}
	}
}

void Document::Deallocate()
{
	int len = m_DynamicLinks.size();
	for (int i = 0; i < len; ++i)
	{
		Directory* parent = m_DynamicLinks[0];
		parent->DeleteDynamicLink(this);
	}
}
Link::Link(const std::string& name, Document* file, Directory* const root)
	:File(name, root)
	, m_file(file)
{}

std::string Link::GetPath() const
{
	const std::string& name = this->GetName();
	int len = name.size();
	return name.substr(6, len - 7);
}

DynamicLink::DynamicLink(const std::string& path, Document* file, Directory* const root)
	:Link("dlink[" + path + "]", file, root)
{}

DynamicLink::~DynamicLink() { Deallocate(); }

void DynamicLink::Deallocate()
{
	Directory* parent = GetFile()->FindParentDynamicLink(this);
	GetFile()->RemoveDynamicLink(parent);
}

HardLink::HardLink(const std::string& path, Document* file, Directory* const root)
	:Link("hlink[" + path + "]", file, root)
{
	root->IncrementHardLinkCount(path);
}

HardLink::~HardLink() { Deallocate(); }

void HardLink::Deallocate()
{
	m_root->DecrementHardLinkCount(GetPath());
}
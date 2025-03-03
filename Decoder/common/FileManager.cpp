#include "FileManager.h"

std::once_flag FileManager::once_flag;
std::unique_ptr<FileManager> FileManager::instance;

FileManager& FileManager::Instance() {
    std::call_once(once_flag, [&]() {
        instance.reset(new FileManager());
    });
    return *instance;
}

FileManager::~FileManager()
{
	if (hZipMod) {
		FreeLibrary(hZipMod);
	}
	m_minizip = NULL;
}

FileManager::FileManager()
	: hZipMod(NULL)
	, m_minizip(NULL)
{
	init();
	hZipMod = LoadLibrary("MINIZIPDLL.dll");
	if (hZipMod == NULL) {
		printf("DLL不存在或加载失败.\n");
	}
	if (hZipMod)
	{
		m_minizip = (minizip)GetProcAddress(hZipMod, "?minizip@@YAHHQEAPEAD@Z");
	}
}

void FileManager::init()
{
	memset(base_file_name, 0, 1024);
	close_all_files();
}

void FileManager::set_base_file_name(const char* file_name)
{
	strcpy(base_file_name, file_name);
}

void FileManager::close_all_files()
{
	FilesMap::iterator it;
	for (it = output_file_map.begin(); it != output_file_map.end(); it++) {
		if (it->second) fclose(it->second); it->second = NULL;
	}
	output_file_map.clear();
}

void FileManager::create_file(FILE*& file, const char* suffix, const char* title)
{
	if (strlen(base_file_name) == 0) return;
	if (file == NULL) {
		char file_name[1024] = { 0 };
		sprintf(file_name, "%s_%s", base_file_name, suffix);
		file = fopen(file_name, "wb");
		if (file && title) {
			fprintf(file, title);
		}
	}
}

FILE* FileManager::get_file(std::string suffix, std::string title)
{
	FILE* f_ptr = NULL;
	FilesMap::iterator it = output_file_map.find(suffix);
	if (it == output_file_map.end()) {
		create_file(f_ptr, suffix.c_str(), title.c_str());
		output_file_map[suffix] = f_ptr;
	}
	f_ptr = output_file_map[suffix];
	return f_ptr;
}

bool FileManager::has_file(std::string suffix)
{
	FilesMap::iterator it = output_file_map.find(suffix);
	return (it != output_file_map.end());
}

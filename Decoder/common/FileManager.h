#pragma once
#include <map>
#include <string>
#include <mutex>
#include <windows.h>

typedef std::map<std::string, FILE*> FilesMap;

typedef int (*minizip)(int, char*[]);

class FileManager {
public:
    static FileManager& Instance();
    ~FileManager();
private:
    FileManager();    // 构造函数私有化
    FileManager(FileManager const&); // 禁止拷贝构造
    FileManager& operator=(FileManager const&);

    static std::once_flag once_flag;
    static std::unique_ptr<FileManager> instance;
public:
    void init();
    void set_base_file_name(const char* file_name);
    void close_all_files();
    void create_file(FILE*& file, const char* suffix, const char* title);
    FILE* get_file(std::string suffix, std::string title);

private:
    char base_file_name[1024];
    FilesMap output_file_map;			//把文件指针都保存到map中
    HMODULE hZipMod;
    minizip m_minizip;
};


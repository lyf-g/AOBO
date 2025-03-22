#ifndef VTABLE_PARSER_HPP
#define VTABLE_PARSER_HPP

#include <libelf.h>
#include <gelf.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>

// 数据结构声明
struct LibraryInfo {
    std::string path;
    uint64_t base_address;
};

struct Symbol_info {
    std::string lib_name;
    uint64_t offset;
    uint64_t size;
};

struct Symbol {
    std::string name;
    uint64_t address;
    uint64_t type;  // relocation type
    int64_t addend;
};

struct SymbolInfo {
    std::string name;
    GElf_Addr address;
    GElf_Xword size;
    std::vector<uint8_t> content;
};

struct VTableEntry {
    GElf_Addr content;
    bool is_dynamic;
    std::string symbol_name;
    std::string reloc_type;
    GElf_Addr base_address;
};

struct VTableInfo {
    std::string name;
    GElf_Addr address;
    GElf_Xword size;
    std::vector<uint8_t> raw_data;
    std::vector<VTableEntry> entries;
};

// 全局变量声明
extern std::unordered_map<std::string, LibraryInfo> lib_info;
extern std::unordered_map<std::string, Symbol_info> symbol_map;
extern std::unordered_map<uint64_t, Symbol> reloc_map;
extern std::vector<VTableInfo> vtables;

// 函数声明
std::string extractFileName(const std::string& path);
void parse_proc_maps(std::string pid);
void get_symbols_from_libs();
int extract_dynamic_symbols(std::string file_path);
void parse_vtable_entries(Elf *elf, VTableInfo &vtable);
int get_vtables(std::string file_path);
void save_vtables_to_binary(const std::vector<VTableInfo>& vtables, 
                           const char* filename);
void process_vtables(const std::string& bolted_binary_path,
                    const std::string& target_pid,
                    const std::string& v_table_bin_path);

#endif // VTABLE_PARSER_HPP

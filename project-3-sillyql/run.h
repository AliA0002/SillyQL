#pragma once
// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "TableEntry.h"
#include <getopt.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <map>
#include <iterator>
using namespace std;

enum class OP { Equal, LessThan, GreaterThan };

struct Table
{
	bool indexExists = false;
	uint32_t indexColumn = 0;
	vector<string> columnNames;
	vector<vector<TableEntry>> info;
	unordered_map<string, uint32_t> nameToIndex;
	unordered_map<string, EntryType> nameToType;
	unordered_map<TableEntry, vector<uint32_t>> hash;
	map<TableEntry, vector<uint32_t>,less<TableEntry>> bst;
};


class Silly
{
public:
	Silly();
	void readOptions(int argc, char** argv);
	void run();
	void CreateTable();
	void InsertInto();
	void GenerateIndex();
	void DeleteFrom();
	void Print();
	void Join();
	void Remove();
	void printHelp();
	void updateIndex(string& tableName);
	TableEntry ReturnTableEntry(EntryType& type, string& value);

private:
	bool quiet;
	unordered_map<string, Table> Database;
	vector<EntryType> types;
};

class comp
{
private:
	char OP;
	TableEntry value;
	uint32_t idx;

public:
	comp(const char& OP_in, const TableEntry& entry_in, uint32_t& colIdx_in)
		: OP(OP_in), value(entry_in), idx(colIdx_in) {}

	bool operator()(vector<TableEntry>const& in)
	{
		switch (OP)
		{
		case '=':
			return value == in.at(idx);
		case '>':
			return in.at(idx) > value;
		case '<':
			return in.at(idx) < value;
		default:
			break;
		}
		return false;
	}
};

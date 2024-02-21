// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "run.h"
//TODO: test if string cmd can be changed into a char
//TODO: you can probably get rid of idx in create_table
//TODO: make sure that you update indices when editing table
//TODO: try to use reserve for the container functions to limit them
//TODO: make sure that reserving the database container doesnt delete stuff
//TODO: to save space, you can define string name and junk in the class
//TODO: make sure to updated index type when using generate index
//TODO: try to put the if (!quiet) outside the for loop to save time
//TODO: bools may be capital True and False instead of lower case
//TODO: can use resize on some vectors to save memory
//TODO: keep track of numrows with printergreaterwithbst and lessthanwithbst

using namespace std;
Silly::Silly()
{
	quiet = false;
}

void Silly::readOptions(int argc, char** argv)
{
	int option_index = 0, choice = 0;
	string line = "";

	opterr = false;

	option longOpts[] = { {"help",no_argument,nullptr,'h'},
							{"quiet",no_argument,nullptr,'q'} };


	while ((choice = getopt_long(argc, argv, "hq", longOpts, &option_index)) != -1)
	{
		switch (choice) {
		case 'h':
			printHelp();
			break;

		case 'q':
			quiet = true;
			break;
		default:
			exit(1);
		}// switch
	}// while
}// readOptions

void Silly::run()
{
	string cmd;
	do
	{
		cout << "% ";
		cin >> cmd;

		if (cin.fail()) {
			cerr << "Error: Reading from cin has failed" << endl;
			exit(1);
		} // if

		switch (cmd[0])
		{
		case 'C':
			CreateTable();
			break;
		case 'I':
			InsertInto();
			break;
		case 'D':
			DeleteFrom();
			break;
		case 'P':
			Print();
			break;
		case 'J':
			Join();
			break;
		case 'G':
			GenerateIndex();
			break;
		case 'R':
			Remove();
			break;
		case 'Q':
			return;
			break;
		case '#':
			getline(cin, cmd);
			break;
		default:
			cout << "Error: unrecognized command\n";
			getline(cin, cmd);
		}

	} while (cmd != "QUIT");


}

void Silly::CreateTable()
{
	Table newTable;
	string name, junk;
	uint32_t numColumns = 0;
	vector<EntryType> type;

	cin >> name >> numColumns;

	if (Database.find(name) != Database.end())
	{
		cout << "Error during CREATE: Cannot create already existing table" << name << "\n";
		getline(cin, junk);
		return;
	}

	Database.reserve(Database.size() + 1);

	for (uint32_t i = 0; i < numColumns; ++i)
	{
		cin >> junk;
		switch (junk[0])
		{
		case 'd':
			type.push_back(EntryType::Double);
			break;
		case 'i':
			type.push_back(EntryType::Int);
			break;
		case 'b':
			type.push_back(EntryType::Bool);
			break;
		case 's':
			type.push_back(EntryType::String);
			break;
		default:
			exit(1);
		}
	}

	cout << "New table " << name << " with column(s) ";

	for (uint32_t i = 0; i < numColumns; ++i)
	{
		cin >> junk;
		cout << junk << " ";

		newTable.columnNames.push_back(junk);
		newTable.nameToIndex[junk] = i;
		newTable.nameToType[junk] = type[i];
	}

	Database.emplace(name, newTable);
	cout << "created\n";
}

void Silly::InsertInto()
{
	string name, junk;
	uint32_t num_rows = 0;
	cin >> junk >> name;
	if (Database.find(name) == Database.end() || junk != "INTO")
	{
		cout << "Error during INSERT: " << name << " does not name a table in the database\n";
		getline(cin, junk);
		return;
	}

	cin >> num_rows >> junk;

	string Value;
	vector<TableEntry> newRow;
	size_t idx_start = Database[name].info.size();

	for (uint32_t i = 0; i < num_rows; ++i)
	{
		for (vector<string>::iterator it = Database[name].columnNames.begin();
			it != Database[name].columnNames.end(); ++it)
		{
			cin >> Value;

			switch (Database[name].nameToType[*it])
			{
			case EntryType::Bool:
				if (Value == "true")
				{
					newRow.emplace_back(true);
				}
				else newRow.emplace_back(false);
				break;

			case EntryType::Double:
				newRow.emplace_back(stod(Value));
				break;

			case EntryType::Int:
				newRow.emplace_back(stoi(Value));
				break;

			case EntryType::String:
				newRow.emplace_back(Value);
				break;

			default:
				break;
			}
		}
		Database[name].info.emplace_back(newRow);
		newRow.clear();
	}

	if (Database[name].indexExists)
	{
		updateIndex(name);
	}

	cout << "Added " << num_rows << " rows to " << name << " from position "
		<< idx_start << " to " << idx_start + num_rows - 1 << "\n";

}

void Silly::updateIndex(string& tableName)
{
	uint32_t idx = Database[tableName].indexColumn;

	if (!Database[tableName].hash.empty())
	{
		Database[tableName].hash.clear();
		for (size_t i = 0; i < Database[tableName].info.size(); ++i)
		{
			Database[tableName].hash[Database[tableName].info[i][idx]].emplace_back(i);
		}
	}
	else if (!Database[tableName].bst.empty())
	{
		Database[tableName].bst.clear();
		for (size_t i = 0; i < Database[tableName].info.size(); ++i)
		{
			Database[tableName].bst[Database[tableName].info[i][idx]].emplace_back(i);
		}
	}
}

void Silly::GenerateIndex()
{
	string tableName, junk, indexType, colName;

	cin >> junk >> tableName;

	if (Database.find(tableName) == Database.end())	//check if name and colname exist
	{
		cout << "Error during GENERATE: " << tableName << " does not name a table in the database\n";
		getline(cin, junk);
		return;
	}

	cin >> indexType >> junk >> junk >> colName;

	if (Database[tableName].nameToIndex.find(colName)
		== Database[tableName].nameToIndex.end())
	{
		cout << "Error during GENERATE: " << colName << " does not name a column in " << tableName << "\n";
		getline(cin, junk);
		return;
	}

	if (!Database[tableName].hash.empty())
		Database[tableName].hash.clear();

	if (!Database[tableName].bst.empty())
		Database[tableName].bst.clear();

	uint32_t idxCol = Database[tableName].nameToIndex[colName];

	switch (indexType[0])
	{
	case 'b':
	{
		for (size_t i = 0; i < Database[tableName].info.size(); ++i)
		{
			Database[tableName].bst[Database[tableName].info[i][idxCol]].emplace_back(i);
		}
		break;
	}

	case 'h':
	{
		for (size_t i = 0; i < Database[tableName].info.size(); ++i)
		{
			Database[tableName].hash[Database[tableName].info[i][idxCol]].emplace_back(i);
		}
		break;
	}
	default:
		break;
	}

	Database[tableName].indexColumn = Database[tableName].nameToIndex[colName];

	Database[tableName].indexExists = true;

	cout << "Created " << indexType << " index for table " << tableName
		<< " on column " << colName << "\n";
}

void Silly::DeleteFrom()
{
	char OP;
	uint32_t colIndex = 0;
	string name, junk, colname, value;
	cin >> junk >> name;

	if (Database.find(name) == Database.end())
	{
		cout << "Error during DELETE: " << name << " does not name a table in the database\n";
		getline(cin, junk);
		return;
	}
	cin >> junk >> colname;

	if (Database[name].nameToIndex.find(colname)
		== Database[name].nameToIndex.end())
	{
		cout << "Error during DELETE: " << colname << " does not name a column in " << name << "\n";
		getline(cin, junk);
		return;
	}
	cin >> OP >> value;

	uint32_t oldSize = (uint32_t)Database[name].info.size();

	colIndex = Database[name].nameToIndex[colname];

	Database[name].info.erase(remove_if(Database[name].info.begin(), Database[name].info.end(),
		comp(OP, ReturnTableEntry(Database[name].nameToType[colname], value),
			colIndex)),
		Database[name].info.end());

	if (Database[name].indexExists)
	{
		updateIndex(name);
	}

	cout << "Deleted " << int(oldSize) - Database[name].info.size()
		<< " rows from " << name << "\n";
}

void Silly::Print()
{
	string tableName, colName, junk;
	uint32_t numColumns = 0;
	uint32_t numRowsPrinted = 0;
	vector<string> Columns;
	vector<uint32_t> ColumnIndexes;

	cin >> junk >> tableName;

	if (Database.find(tableName) == Database.end())
	{
		cout << "Error during PRINT: " << tableName << " does not name a table in the database\n";
		getline(cin, junk);
		return;
	}

	cin >> numColumns;

	Columns.reserve(numColumns);

	for (uint32_t i = 0; i < numColumns; ++i)
	{
		cin >> colName;

		if (Database[tableName].nameToIndex.find(colName)
			== Database[tableName].nameToIndex.end())
		{
			cout << "Error during PRINT: " << colName << " does not name a column in " << tableName << "\n";
			getline(cin, junk);
			return;
		}
		Columns.push_back(colName);
		ColumnIndexes.push_back(Database[tableName].nameToIndex[colName]);
	}

	if (!quiet)
	{
		for (uint32_t i = 0; i < numColumns; ++i)
		{
			cout << Columns[i] << " ";
		}
		cout << "\n";
	}

	cin >> junk;

	if (junk[0] == 'A')
	{
		if (!quiet)
		{
			for (uint32_t i = 0; i < (uint32_t)Database[tableName].info.size(); ++i)
			{
				for (uint32_t j = 0; j < numColumns; ++j)
				{
					cout << Database[tableName].info[i][Database[tableName]
						.nameToIndex[Columns[j]]] << " ";
				}
				cout << "\n";
			}
		}
		numRowsPrinted = (uint32_t)Database[tableName].info.size();
	}

	else
	{
		char OP;
		string in_value;
		cin >> colName;

		if (Database[tableName].nameToIndex.find(colName)
			== Database[tableName].nameToIndex.end())
		{
			cout << "Error during PRINT: " << colName << " does not name a column in " << tableName << "\n";
			getline(cin, junk);
			return;
		}

		cin >> OP >> in_value;

		uint32_t colIdx = Database[tableName].nameToIndex[colName];

		TableEntry entry(ReturnTableEntry(Database[tableName].nameToType[colName], in_value));

		if (!Database[tableName].hash.empty() && Database[tableName].indexColumn
			== Database[tableName].nameToIndex[colName] && OP == '=' && quiet)
		{
			auto val = Database[tableName].hash.find(entry);

			if (val != Database[tableName].hash.end())
				numRowsPrinted = (uint32_t)val->second.size();
		}

		else if (!Database[tableName].bst.empty() && Database[tableName].indexColumn
			== Database[tableName].nameToIndex[colName])
		{
			if (OP == '=')
			{
				auto elements = Database[tableName].bst.find(entry);

				if (elements != Database[tableName].bst.end())
				{
					numRowsPrinted = (uint32_t)elements->second.size();
					if (!quiet)
					{
						for (uint32_t i = 0; i < (uint32_t)elements->second.size(); ++i)
						{
							for (uint32_t j = 0; j < numColumns; ++j)
							{
								cout << Database[tableName].info[elements->second[i]][ColumnIndexes[j]]
									<< " ";
							}
							cout << "\n";
						}
					}
				}
			}

			else if (OP == '>')
			{
				auto upperBound = Database[tableName].bst.upper_bound(entry);

				for (auto& it = upperBound; it != Database[tableName].bst.end(); ++it)
				{
					numRowsPrinted += (uint32_t)it->second.size();
					if (!quiet)
					{
						for (auto i : it->second)
						{
							for (auto j : ColumnIndexes)
							{
								cout << Database[tableName].info.at(i).at(j) << " ";
							}
							cout << "\n";
						}
					}
				}
			}

			else if (OP == '<')
			{
				auto lowerBound = Database[tableName].bst.lower_bound(entry);

				for (auto it = Database[tableName].bst.begin(); it != lowerBound; ++it)
				{
					numRowsPrinted += (uint32_t)it->second.size();
					if (!quiet)
					{
						for (auto i : it->second)
						{
							for (auto j : ColumnIndexes)
							{
								cout << Database[tableName].info.at(i).at(j) << " ";
							}
							cout << "\n";
						}
					}
				}
			}
		}

		else
		{
			vector<uint32_t> printRow;

			for (uint32_t i = 0; i < Database[tableName].info.size(); ++i)
			{
				switch (OP)
				{
				case '=':
					if (Database[tableName].info[i][colIdx] == entry)
						printRow.emplace_back(i);
					break;

				case '>':
					if (Database[tableName].info[i][colIdx] > entry)
						printRow.emplace_back(i);
					break;

				case '<':
					if (Database[tableName].info[i][colIdx] < entry)
						printRow.emplace_back(i);
					break;

				default:
					break;
				}
			}
			numRowsPrinted = (uint32_t)printRow.size();

			if (!quiet)
			{
				for (uint32_t i = 0; i < numRowsPrinted; ++i)
				{
					for (uint32_t j = 0; j < (uint32_t)ColumnIndexes.size(); ++j)
					{
						cout << Database[tableName].info[printRow[i]][ColumnIndexes[j]]
							<< " ";
					}
					cout << "\n";
				}
			}

		}
	}

	cout << "Printed " << numRowsPrinted << " matching rows from "
		<< tableName << "\n";
}

TableEntry Silly::ReturnTableEntry(EntryType& type, string& value)
{
	switch (type)
	{
	case EntryType::Int:
	{
		int num = stoi(value);
		return TableEntry(num);
		break;
	}
	case EntryType::Double:
	{
		double num = stod(value);
		return TableEntry(num);
		break;
	}
	case EntryType::Bool:
	{
		bool boolVal;
		if (value[0] == 't') boolVal = true;
		else boolVal = false;
		return TableEntry(boolVal);
		break;
	}
	case EntryType::String:
	{
		return TableEntry(value);
		break;
	}
	default:
		break;
	}
	return TableEntry(0);
}

void Silly::Join()
{
	string tableName1, tableName2, colName1, colName2
		, junk;

	cin >> tableName1;

	if (Database.find(tableName1) == Database.end())		//check if tables exist
	{
		cout << "Error during JOIN: " << tableName1 << " does not name a table in the database\n";
		getline(cin, junk);
		return;
	}

	cin >> junk >> tableName2;

	if (Database.find(tableName2) == Database.end())
	{
		cout << "Error during JOIN: " << tableName2 << " does not name a table in the database\n";
		getline(cin, junk);
		return;
	}

	uint32_t numColumns, numRowsPrinted;
	numColumns = numRowsPrinted = 0;

	cin >> junk >> colName1;

	if (Database[tableName1].nameToIndex.find(colName1)
		== Database[tableName1].nameToIndex.end())
	{
		cout << "Error during JOIN: " << colName1 << " does not name a column in " << tableName1 << "\n";
		getline(cin, junk);
		return;
	}

	cin >> junk >> colName2;

	if (Database[tableName2].nameToIndex.find(colName2)
		== Database[tableName2].nameToIndex.end())
	{
		cout << "Error during JOIN: " << colName2 << " does not name a column in " << tableName2 << "\n";
		getline(cin, junk);
		return;
	}

	cin >> junk >> junk >> numColumns;

	vector<uint32_t> tableNum;
	vector<string> colNames;
	tableNum.reserve(numColumns);
	colNames.reserve(numColumns);
	uint32_t num = 0;
	string name;

	for (uint32_t i = 0; i < numColumns; ++i)
	{
		cin >> name >> num;

		if (num == 1)
		{
			if (Database[tableName1].nameToIndex.find(name)
				== Database[tableName1].nameToIndex.end())
			{
				cout << "Error during JOIN: " << colName1 << " does not name a column in " << tableName1 << "\n";
				getline(cin, junk);
				return;
			}
		}
		else
		{
			if (Database[tableName2].nameToIndex.find(name)
				== Database[tableName2].nameToIndex.end())
			{
				cout << "Error during JOIN: " << colName2 << " does not name a column in " << tableName2 << "\n";
				getline(cin, junk);
				return;
			}
		}
		colNames.push_back(name);
		tableNum.push_back(num);
	}

	if (!quiet)
	{
		for (uint32_t i = 0; i < numColumns; ++i)
		{
			cout << colNames[i] << " ";
		}
		cout << "\n";
	}

	uint32_t col2Index = Database[tableName2].nameToIndex[colName2];

	unordered_map<TableEntry, vector<uint32_t>> HashTable2;
	for (uint32_t i = 0; i < Database[tableName2].info.size(); ++i)
	{
		HashTable2[Database[tableName2].info[i][col2Index]].emplace_back(i);
	}

	for (uint32_t i = 0; i < Database[tableName1].info.size(); ++i)
	{
		auto val = HashTable2.find(Database[tableName1]
			.info[i][Database[tableName1].nameToIndex[colName1]]);

		if (val != HashTable2.end())
		{
			for (auto it : val->second)
			{
				if (!quiet)
				{
					for (uint32_t j = 0; j < numColumns; ++j)
					{
						if (tableNum[j] == 1)
						{
							int idx = Database[tableName1].nameToIndex[colNames[j]];
							cout << Database[tableName1].info[i][idx] << " ";
						}
						else
						{
							int idx = Database[tableName2].nameToIndex[colNames[j]];
							cout << Database[tableName2].info.at(it).at(idx) << " ";
						}
					}
					cout << "\n";
				}
				++numRowsPrinted;
			}
		}
	}

	cout << "Printed " << numRowsPrinted << " rows from joining " << tableName1
		<< " to " << tableName2 << "\n";
}

void Silly::Remove()
{
	string name;
	cin >> name;
	if (Database.find(name) == Database.end())
	{
		cout << "Error during REMOVE: " << name << " does not name a table in the database\n";
		getline(cin, name);
		return;
	}
	Database.erase(name);

	cout << "Table " << name << " deleted\n";
}

void Silly::printHelp()
{
	cout << "Helpful message!!!\n";
	exit(0);
}
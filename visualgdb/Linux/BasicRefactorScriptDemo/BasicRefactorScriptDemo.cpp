#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<int> ReadCSVLine(istream& str)
{
    vector<int> result;
    string line;
    getline(str, line);

    stringstream lineStream(line);
    string tmp;

    while (getline(lineStream, tmp, ';'))
        result.push_back(atoi(tmp.c_str()));
    
    return move(result);
}

struct DataRow
{
    int Col1, Col2, Col3;
    
    DataRow(int col1, int col2, int col3)
    		: Col1(col1)
    		, Col2(col2)
    		, Col3(col3)
    {
    }
    
    bool operator<(const DataRow &right) const
    {
    	if (Col1 != right.Col1)
    		return Col1 < right.Col1;
    
    	if (Col2 != right.Col2)
    		return Col2 < right.Col2;
    
    	if (Col3 != right.Col3)
    		return Col3 < right.Col3;
    
    	return false;
    }
};

template<class _Traits> inline basic_ostream<char, _Traits>& 
    operator<<(basic_ostream<char, _Traits>& stream, const DataRow &row)
{
    stream << "[";
    stream << "Col1 = " << row.Col1 << ", ";
    stream << "Col2 = " << row.Col2 << ", ";
    stream << "Col3 = " << row.Col3;
    stream << "]" << endl;
    return stream;
}

int main(int argc, char *argv[])
{
    ifstream stream("data.csv");
    std::vector<DataRow> rows;
    while (!stream.eof())
    {
        auto line = ReadCSVLine(stream);
        if (line.size() >= 3)
            rows.push_back(DataRow(line[0], line[1], line[2]));
    }
    
    std::sort(rows.begin(), rows.end());
    
    for (const auto &row : rows)
        cout << row;
    return 0;
}
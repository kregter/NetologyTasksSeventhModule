#pragma once
#include <vector>
#include <map>
#include <string>

class SqlSelectQueryBuilder {
private:
	std::string column;
	std::string from;
	std::string where_val;
	bool AddColumn_was_called = false;
	std::string query;

	bool query_is_correct();
public:
	SqlSelectQueryBuilder& AddColumn(std::string _column);
	SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns);

	SqlSelectQueryBuilder& AddFrom(std::string _from);

	SqlSelectQueryBuilder& AddWhere(std::string _where, std::string _equal);
	SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv);

	bool BuildQuery();

	std::string get_query();
};
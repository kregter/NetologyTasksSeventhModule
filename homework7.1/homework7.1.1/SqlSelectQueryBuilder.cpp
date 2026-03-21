#include "SqlSelectQueryBuilder.hpp"

bool SqlSelectQueryBuilder::query_is_correct() {
	if (from.empty() || where_val.empty() || column.empty()) return false;
	else return true;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumn(std::string _column) {
	if (column.empty()) {
		column = _column;
	}
	else {
		column += ", " + _column;
	}
	AddColumn_was_called = true;
	return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddFrom(std::string _from) {
	from = _from;
	return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(std::string _where, std::string _equal) {
	if (where_val.empty()) {
		where_val = _where + "=" + _equal;
	}
	else {
		where_val += " AND " + _where + "=" + _equal;
	}
	return *this;
}

bool SqlSelectQueryBuilder::BuildQuery() {
	if (!AddColumn_was_called) {
		column = "*";
	}
	if (!query_is_correct()) {
		return false;
	}
	query = "SELECT " + column + " FROM " + from + " WHERE " + where_val + ";";
	return true;
}

std::string SqlSelectQueryBuilder::get_query() {
	return query;
}
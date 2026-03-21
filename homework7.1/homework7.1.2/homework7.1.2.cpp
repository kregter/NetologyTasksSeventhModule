#include <iostream>
#include "SqlSelectQueryBuilder.hpp"

int main()
{
	setlocale(LC_ALL, "rus");

	SqlSelectQueryBuilder query_builder;
	query_builder.AddColumn("name").AddColumn("phone");
	query_builder.AddFrom("students");
	query_builder.AddWhere("id", "42").AddWhere("name", "John");
	if (query_builder.BuildQuery()) {
		std::cout << query_builder.get_query() << std::endl;
	}
	else {
		std::cout << "Query wasnt formed" << std::endl;
	}

	SqlSelectQueryBuilder query_builder2;
	std::vector<std::string> columns = {"name", "address"};
	query_builder2.AddColumns(columns);
	query_builder2.AddFrom("students");
	std::map<std::string, std::string> Wheres = { {"id", "53"}, {"name", "Nikita"} };
	query_builder2.AddWhere(Wheres);

	if (query_builder2.BuildQuery()) {
		std::cout << query_builder2.get_query() << std::endl;
	}
	else {
		std::cout << "Query wasnt formed" << std::endl;
	}

	return 0;
}
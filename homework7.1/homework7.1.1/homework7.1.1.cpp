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

	return 0;
}
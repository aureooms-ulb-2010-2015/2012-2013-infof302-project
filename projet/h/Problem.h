#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <string>

typedef struct Problem{

	size_t size = 0;
	size_t tapes = 0;
	std::vector<std::vector<int>> rows;
	std::vector<std::vector<int>> cols;
	std::vector<std::vector<int>> grid;

} Problem;

#endif // PROBLEM_H
#ifndef SOLUTION_H
#define SOLUTION_H


#include <vector>
#include <string>

typedef struct Solution{

	size_t size = 0;
	size_t tapes = 0;
	std::vector<std::vector<int>> rows;
	std::vector<std::vector<int>> cols;
	std::vector<std::vector<int>> grid;

} Solution;


#endif // SOLUTION_H
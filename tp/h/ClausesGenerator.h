#ifndef _CLAUSESGENERATOR_H
#define _CLAUSESGENERATOR_H

#include <vector>
#include <cstring>


class ClausesGenerator{
	
	private:
		void generateClausesRecursively(std::vector<std::vector<int>>& clauses, std::vector<int> neighbours, int mines, size_t offset);
		std::vector<std::vector<int>> generateClauses(std::vector<int> neighbours, int mines);
		std::vector<std::vector<int>> translate(std::vector<std::vector<int>> XORclauses);
		int f(int n);

	public:
		std::vector<std::vector<int>> run(std::vector<std::vector<char>> table);
};


#endif // _CLAUSESGENERATOR_H

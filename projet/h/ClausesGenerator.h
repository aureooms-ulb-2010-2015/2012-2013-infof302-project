#ifndef _CLAUSESGENERATOR_H
#define _CLAUSESGENERATOR_H

#include <vector>
#include <string>
#include "Problem.h"


class ClausesGenerator{

public:
	typedef std::vector<int> Clause;
	typedef std::vector<Clause> FNC;
	
private:
	Problem* _problem;

	virtual FNC generateClauses();
	virtual int BOX(size_t i, size_t j);
	virtual int LBande(size_t pos, size_t ID, size_t start);
	virtual int CBande(size_t pos, size_t ID, size_t start);
	virtual int L(size_t i, size_t ID);
	virtual int C(size_t j, size_t ID);
	virtual FNC generateHorizontalClauseAtMostOne(size_t pos, size_t ID, size_t start);
	virtual FNC generateHorizontalClauseBoxes(size_t pos, size_t ID, size_t start);
	virtual FNC generateHorizontalClauseOrder(size_t pos, size_t ID, size_t start);
	virtual FNC generateHorizontalClauseOrder(size_t pos, size_t ID);
	virtual FNC generateHorizontalClause(size_t pos, size_t ID);
	virtual FNC generateHorizontalClause(size_t pos);
	virtual FNC generateAllHorizontalClauses();
	virtual FNC generateVerticalClause(size_t pos, size_t ID, size_t start);
	virtual FNC generateVerticalClause(size_t pos, size_t ID);
	virtual FNC implies(int condition, std::vector<int> lits);
	virtual void load(const Problem&);


public:
	virtual FNC run(const Problem&);
};


#endif // _CLAUSESGENERATOR_H

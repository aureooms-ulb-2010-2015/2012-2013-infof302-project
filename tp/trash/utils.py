class Utils:
	MINES_SWEEPER_1 = [
		['0','0','0','1','1'],
		['0','0','0','2','?'],
		['0','0','0','2','?'],
		['1','1','0','1','1'],
		['?','1','0','0','0']
	]
	
	MINES_SWEEPER_2 = [
		['?','?','?','?','?','?'],
		['?','2','2','2','2','?'],
		['?','2','0','0','2','?'],
		['?','2','0','0','2','?'],
		['?','2','2','2','2','?'],
		['?','?','?','?','?','?']
	]
	
	MINES_SWEEPER_3 = [
		['0','0','0','0','0','0','0','0'],
		['0','1','1','1','1','1','1','0'],
		['0','1','M','2','2','M','1','0'],
		['0','1','2','?','?','2','1','0'],
		['0','1','2','?','?','2','1','0'],
		['0','1','M','2','2','M','1','0'],
		['0','1','1','1','1','1','1','0'],
		['0','0','0','0','0','0','0','0']
	]
	@staticmethod
	def countVariables(clauses):
		res = 0
		for clause in clauses:
			for var in clause:
				res = max(res,abs(var))
		return res
	@staticmethod
	def clauseToString(clause):
		res = ""
		for var in clause:
			res = res + str(var) + " "
		return res + "0"
	@staticmethod
	def toDIMACS(clauses):
		varCount = Utils.countVariables(clauses)
		res = "p cnf "+str(varCount)+" "+str(len(clauses))+"\n"
		for clause in clauses:
			res = res + Utils.clauseToString(clause) + "\n"
		return res


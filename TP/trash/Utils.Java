import static java.lang.Math.abs;
import static java.lang.Math.max;

public final class Utils{
	public static final char[][]
		MINES_SWEEPER_1 = {
			{'0','0','0','1','1'},
			{'0','0','0','2','?'},
			{'0','0','0','2','?'},
			{'1','1','0','1','1'},
			{'?','1','0','0','0'}
		},
		MINES_SWEEPER_2 = {
			{'?','?','?','?','?','?'},
			{'?','2','2','2','2','?'},
			{'?','2','0','0','2','?'},
			{'?','2','0','0','2','?'},
			{'?','2','2','2','2','?'},
			{'?','?','?','?','?','?'}
		},
		MINES_SWEEPER_3 = {
			{'0','0','0','0','0','0','0','0'},
			{'0','1','1','1','1','1','1','0'},
			{'0','1','M','2','2','M','1','0'},
			{'0','1','2','?','?','2','1','0'},
			{'0','1','2','?','?','2','1','0'},
			{'0','1','M','2','2','M','1','0'},
			{'0','1','1','1','1','1','1','0'},
			{'0','0','0','0','0','0','0','0'}
		}
	;
	private static final int countVariables(int[][] clauses){
		int res = 0;
		for(int[] clause: clauses){
			for(int var:clause)
				res = std::max<int>( res , std::abs<int>(var) );
		}
		return res;
	}
	private static final String clauseToString(int[] clause){
		StringBuffer res = new StringBuffer(clause.length);
		for(int i=0;i<clause.length;i++){
			res.append(clause[i]);
			res.append(" ");
		}
		res.append("0");
		return res.toString();
	}
	public static String finaltoDIMACS(int[][] clauses){
		StringBuffer res = new StringBuffer("p cnf " + countVariables(clauses)+ " " +clauses.length+"\n");
		for(int[] clause : clauses){
			res.append(clauseToString(clause));
			res.append("\n");
		}
		return res.toString();
	}
}

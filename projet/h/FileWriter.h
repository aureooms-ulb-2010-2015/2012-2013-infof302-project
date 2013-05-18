#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "Solution.h"
#include <string>
#include <fstream>
#include "Exception.h"

class FileWriter{

public:
	class Exception : public ::Exception {
	public:
		Exception(const std::string& what): ::Exception(what) {}
	};

public:
	virtual void save(const Solution& solution, const std::string& filename){
		std::ofstream fs;
		fs.open(filename);
		if(!fs.good()){
			throw FileWriter::Exception("Could not open file : '" + filename + "'");
		}

		fs << solution.size << " " << solution.tapes << std::endl;
		for(const std::vector<int>& col : solution.cols){
			std::copy(col.begin(), col.end(),
			std::ostream_iterator<int>(fs, " "));
			fs <<  std::endl;
		}
		for(const std::vector<int>& row : solution.rows){
			std::copy(row.begin(), row.end(),
			std::ostream_iterator<int>(fs, " "));
			fs <<  std::endl;
		}
		for(const std::vector<int>& line : solution.grid){
			std::copy(line.begin(), line.end(),
			std::ostream_iterator<int>(fs, " "));
			fs <<  std::endl;
		}

		fs.close();
	}

};


#endif //FILEWRITER_H
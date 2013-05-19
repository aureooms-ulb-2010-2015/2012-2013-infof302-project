#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "Problem.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "Exception.h"

class FileWriter{

public:
	class Exception : public ::Exception {
	public:
		Exception(const std::string& what): ::Exception(what) {}
	};

public:
	virtual void save(const Problem& problem, const std::string& filename){
		this->save(std::vector<Problem>({problem}), filename);
	}

	virtual void save(const std::vector<Problem>& problems, const std::string& filename){
		std::ofstream fs;
		fs.open(filename);
		if(!fs.good()){
			throw FileWriter::Exception("Could not open file : '" + filename + "'");
		}
		for(const Problem& problem : problems){
			fs << problem.size << " " << problem.tapes << std::endl;
			for(const std::vector<int>& col : problem.cols){
				std::copy(col.begin(), col.end(),
				std::ostream_iterator<int>(fs, " "));
				fs <<  std::endl;
			}
			for(const std::vector<int>& row : problem.rows){
				std::copy(row.begin(), row.end(),
				std::ostream_iterator<int>(fs, " "));
				fs <<  std::endl;
			}
			for(const std::vector<int>& line : problem.grid){
				std::copy(line.begin(), line.end(),
				std::ostream_iterator<int>(fs, " "));
				fs <<  std::endl;
			}
			fs <<  std::endl;
		}
		fs.close();
	}

	virtual void empty(const std::string& filename){
		std::ofstream fs;
		fs.open(filename);
		if(!fs.good()){
			throw FileWriter::Exception("Could not open file : '" + filename + "'");
		}
		fs.close();
	}

};


#endif //FILEWRITER_H
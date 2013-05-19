#include "ProblemParser.h"
#include <iostream>
#include <locale>
#include <string>

Problem ProblemParser::parse(const std::string& filename) const{

	std::ifstream fs;
	fs.open(filename);
	if(!fs.good()){
		throw ProblemParser::Exception("Could not open file : '" + filename + "'");
	}

	Problem problem;

	this->fill(fs, problem.size, ' ');
	this->fill(fs, problem.tapes, '\n');

	if(problem.size > 0){
		this->fill(fs, problem.cols, problem.size);
		this->fill(fs, problem.rows, problem.size);
		this->fill(fs, problem.grid, problem.size);
	}

	fs.close();

	this->validate(problem);

	return problem;
}

void ProblemParser::validate(const Problem& problem) const{
	if(problem.cols.size() < problem.size){
		throw ProblemParser::Exception("Early eof : could not find all column constraints");
	}
	if(problem.rows.size() < problem.size){
		throw ProblemParser::Exception("Early eof : could not find all row constraints");
	}
	if(problem.grid.size() < problem.size){
		throw ProblemParser::Exception("Early eof : could not fill all the grid");
	}
	this->validate(problem.cols, problem.tapes, "Cols", std::greater<size_t>());
	this->validate(problem.rows, problem.tapes, "Rows", std::greater<size_t>());
	this->validate(problem.grid, problem.size, "Grid", std::not_equal_to<size_t>());
		
}

void ProblemParser::validate(const std::vector<std::vector<int>>& values, size_t size, const std::string& type, std::function<bool(size_t,size_t)> comp) const{
	for(size_t i = 0; i < values.size(); ++i){
		if(comp(values[i].size(),size)){
			throw ProblemParser::Exception(type + " line #" + std::to_string(i) + " : expected size : " + std::to_string(size) + ", actual size : " + std::to_string(values[i].size()));
		}
	}	
}

void ProblemParser::fill(std::ifstream& fs, size_t& val, char stop) const{
	std::string token;
	while(fs.good()){
		char c = fs.get();
		if(fs.good()){
			if(std::isdigit(c)){
				token.push_back(c);
			}
			else if(c == stop && !token.empty()){
				val = std::stoi(token);
				token.clear();
				break;
			}
		}
	}
}


void ProblemParser::fill(std::ifstream& fs, std::vector<std::vector<int>>& mat, size_t size) const{

	std::string token;
	std::vector<int> temp;
	while(fs.good() && mat.size() < size){
		char c = fs.get();
		if(fs.good()){
			if(std::isdigit(c) || c == '-'){
				token.push_back(c);
			}
			else if(c == ' ' && !token.empty()){
				temp.push_back(std::stoi(token));
				token.clear();
			}
			else if(c == '\n'){
				if(!token.empty()){
					temp.push_back(std::stoi(token));
					token.clear();
				}
				mat.push_back(temp);
				temp.clear();
			}
		}
	}
}
#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

#define FLAG(x) (FLAGS.count(#x) > 0)

std::unordered_map<std::string, std::vector<std::string>> OPTIONAL;
std::unordered_set<std::string> FLAGS;
std::vector<std::string> ARGV;

void PARSEINPUT(int argc, char* argv[], const std::unordered_set<std::string>& flags){

    bool isOptionalParameter = false;
    std::string optionalParameterKey = "";

    for (int n = 1; n < argc; ++n){
        std::string p = argv[n];
        if (p[0] == '-'){
            isOptionalParameter = false;
            if(p.size() > 1){
                if(p[1] == '-' || p.size() == 2){
                    if(flags.count(p) > 0){
                        FLAGS.insert(p);
                    }
                    else{
                        OPTIONAL[p] = std::vector<std::string>();
                        optionalParameterKey = p;
                        isOptionalParameter = true;
                    }
                }
                else{
                    for(size_t i = 1; i < p.size(); ++i){
                        std::stringstream option;
                        option << "-" << p[i];
                        FLAGS.insert(option.str());
                    }
                }
            }
        }
        else if(isOptionalParameter){
            OPTIONAL[optionalParameterKey].push_back(p);
        }
        else{
            ARGV.push_back(p);
        }
    }
}

#endif // INPUTPARSER_H
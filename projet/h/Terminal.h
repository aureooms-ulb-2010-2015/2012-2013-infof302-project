#ifndef WTERMINAL_H
#define WTERMINAL_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>


class Terminal{

public:

	const char* BEG = "\033[";
	const char* END = "m";
	const char* RESET = "0";

	bool colors = true;
	bool inhibited = false;
	
	const std::vector<const char*> COLOR = {"30","30;47","30;40","30;41","30;42","30;43","30;44","30;45","30;46",
	"1;30","1;30;47","1;30;40","1;30;41","1;30;42","1;30;43","1;30;44","1;30;45","1;30;46","37","37;47","37;40",
	"37;41","37;42","37;43","37;44","37;45","37;46","1;37","1;37;47","1;37;40","1;37;41","1;37;42","1;37;43","1;37;44",
	"1;37;45","1;37;46","31","31;47","31;40","31;41","31;42","31;43","31;44","31;45","31;46","1;31","1;31;47","1;31;40",
	"1;31;41","1;31;42","1;31;43","1;31;44","1;31;45","1;31;46","32","32;47","32;40","32;41","32;42","32;43","32;44","32;45",
	"32;46","1;32","1;32;47","1;32;40","1;32;41","1;32;42","1;32;43","1;32;44","1;32;45","1;32;46","33","33;47","33;40",
	"33;41","33;42","33;43","33;44","33;45","33;46","1;33","1;33;47","1;33;40","1;33;41","1;33;42","1;33;43","1;33;44","1;33;45",
	"1;33;46","34","34;47","34;40","34;41","34;42","34;43","34;44","34;45","34;46","1;34","1;34;47","1;34;40","1;34;41","1;34;42",
	"1;34;43","1;34;44","1;34;45","1;34;46","35","35;47","35;40","35;41","35;42","35;43","35;44","35;45","35;46","1;35","1;35;47",
	"1;35;40","1;35;41","1;35;42","1;35;43","1;35;44","1;35;45","1;35;46","36","36;47","36;40","36;41","36;42","36;43","36;44","36;45",
	"36;46","1;36","1;36;47","1;36;40","1;36;41","1;36;42","1;36;43","1;36;44","1;36;45","1;36;46"};
	

	const char* HEADER = "95";
    const char* OKBLUE = "94";
    const char* OKGREEN = "92";
    const char* WARNING = "93";
    const char* FAIL = "91";

	virtual void print(const std::string& s) const{
		if(inhibited) return;
		std::cout << s;
	}

	virtual void print(const std::string& s, const char* color) const{
		if(inhibited) return;
		this->setColor(color);
		this->print(s);
		this->resetColor();
	}
	
	virtual void println(const std::string& s) const{
		if(inhibited) return;
		this->print(s);
		std::cout << std::endl;
	}
	
	virtual void println(const std::string& s, const char* color) const{
		if(inhibited) return;
		this->print(s, color);
		std::cout << std::endl;
	}

	virtual void setColor(const char* color) const{
		if(inhibited) return;
		if(colors) std::cout << this->BEG << color << this->END;
	}
	
	virtual void resetColor() const{
		if(inhibited) return;
		this->setColor(this->RESET);
	}

};
#endif // WTERMINAL_H

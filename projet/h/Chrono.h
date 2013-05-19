#ifndef WCHRONO_H
#define WCHRONO_H

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <sstream>
#include <sys/time.h>

class Chrono {

private:
	unsigned long long _start;
	unsigned long long _lastStop;
	unsigned long long _stop;
	unsigned long long _elapsed;
	bool _running;

public:
	Chrono():_start(0),_lastStop(0),_stop(0),_elapsed(0),_running(false){
	}
	virtual ~Chrono() throw(){}
	
	void start(){
		this->_running = true;
		this->_start = this->timestamp();
		this->_lastStop = this->_start;
		this->_elapsed = 0;
	}
	void resume(){
		this->_running = true;
		this->_lastStop = this->timestamp();
	}
	void pause(){
		this->stop();
	}
	void stop(){
		this->_stop = this->timestamp();
		this->_elapsed += this->_stop - this->_lastStop;
		this->_lastStop = this->_stop;
		this->_running = false;
	}
	bool running(){
		return this->_running;
	}
	unsigned long long sec(){
		return this->msec()/1000;
	}

	unsigned long long msec(){
		return this->usec()/1000;
	}

	unsigned long long usec(){
		if (!this->running())
			return (this->_elapsed);
		else
			return (this->timestamp() - this->_lastStop + this->_elapsed);
	}

	unsigned long long timestamp(){
		timeval now;
		gettimeofday(&now,NULL);
	    unsigned long long res = now.tv_sec;
	    res *= 1000000;
	    res += now.tv_usec;
	    return res;
	}
	
};

#endif // WCHRONO_H

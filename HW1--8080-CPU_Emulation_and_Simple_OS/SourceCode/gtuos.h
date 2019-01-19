#ifndef H_GTUOS
#define H_GTUOS
#include <cstdlib>
#include <ctime>
#include "8080emuCPP.h"


class GTUOS{
    public:
    	GTUOS(){
			srand(time(NULL));
		}
        uint64_t handleCall(const CPU8080 &);
		void WriteToFileFromMemory(const CPU8080 &);         
};

#endif

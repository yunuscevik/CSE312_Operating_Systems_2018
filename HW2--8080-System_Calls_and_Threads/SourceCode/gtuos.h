#ifndef H_GTUOS
#define H_GTUOS
#include <cstdlib>
#include <ctime>
#include "8080emuCPP.h"
#include <vector>
typedef enum { READY,BLOCKED,RUNNING } status;
typedef struct Thread{
    State8080 regesters;
    uint64_t threadID;
    uint64_t usedCycle;
    uint64_t time;
    uint8_t state;
    uint16_t startingAddress;
    bool stackSpace;
}THREAD;

class GTUOS{
    public:
        
    	GTUOS(){
            srand(time(NULL));
        }
        uint64_t handleCall(const CPU8080 &, uint64_t);
        void WriteToFileFromMemory(const CPU8080 &);
        void Debug3_Print();
        void switchThread();
        void robin(int & cycle, CPU8080 & cpu);
        
        std::vector<Thread> getThreadTable() const {
            return threadTable;
        }
        
    
         
    private:
        std::vector<Thread> threadTable;
        std::vector<uint64_t> deleteTid;
        static const uint64_t quantumTime = 100;
        static uint64_t tid;
        static uint64_t tidCount;
                
};

#endif

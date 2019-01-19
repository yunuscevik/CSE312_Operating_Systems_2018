#ifndef H_GTUOS
#define H_GTUOS
#include <cstdlib>
#include <ctime>
#include "8080emuCPP.h"
#include <vector>
#include <string>

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

typedef struct FileInfo{
    std::string fileName;
    std::string fileCreation;
    std::string lastModification;
    std::string lastAccess;
    int fileSize;
    std::string content;
    bool openOrClose = false;
     
}FILES;

class GTUOS{
    public:
        
    	GTUOS();
        GTUOS(std::string FileSystemDat);

        uint64_t handleCall(const CPU8080 &, uint64_t);
        void WriteToFileFromMemory(const CPU8080 &);
        void Debug3_Print();
        void switchThread();
        void robin(int & cycle, CPU8080 & cpu);
        void readFileSystemDat(std::string fileSystemDat);
        void writeFileSystemDat(std::string fileSystemDat);
        void Debug1Print();
        void endOfSimulation(); 
        std::string parse(std::string& str);
        std::string getTime();
        
        std::vector<Thread> getThreadTable() const { return threadTable; }
        std::vector<FILES> getFileTable() const { return fileTable; }
        
    private:
        std::vector<Thread> threadTable;
        std::vector<uint64_t> deleteTid;
        std::vector<FILES> fileTable;
        static const uint64_t quantumTime = 100;
        static uint64_t tid;
        static uint64_t tidCount;
        int handleID;
        uint8_t seekValue;
        bool exception;
           
};

#endif

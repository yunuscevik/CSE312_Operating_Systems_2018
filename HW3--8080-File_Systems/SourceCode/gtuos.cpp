#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <math.h>
#include "8080emuCPP.h"
#include "gtuos.h"
# include<fstream>

const uint8_t PRINT_B = 1;
const uint8_t PRINT_MEM = 2;
const uint8_t READ_B = 3;
const uint8_t READ_MEM = 4;
const uint8_t PRINT_STR = 5;
const uint8_t READ_STR = 6;
const uint8_t GET_RND = 7;
const uint8_t TExit = 8;
const uint8_t TJoin = 9;
const uint8_t TYield = 10;
const uint8_t TCreate = 11;
const uint8_t FileCreate = 12;
const uint8_t FileClose = 13;
const uint8_t FileOpen = 14;
const uint8_t FileRead = 15;
const uint8_t FileWrite = 16;
const uint8_t FileSeek = 17;
const uint8_t DirRead = 18;
const uint64_t CYCLES_FIVE = 5;
const uint64_t CYCLES_TEN = 10;
const uint64_t CYCLES_HUNDRED = 100;
const uint64_t CYCLES_EIGHTY = 80;
const uint64_t CYCLES_FIFTY = 50;
const uint64_t CYCLES_FOURTY = 40;
uint64_t GTUOS::tid = 1000;
uint64_t GTUOS::tidCount = 1;

GTUOS::GTUOS(std::string FileSystemDat) {
    exception = false;
    srand(time(NULL));
    readFileSystemDat(FileSystemDat);
}
GTUOS::GTUOS() {
    srand(time(NULL));
}

uint64_t GTUOS::handleCall(const CPU8080 & cpu, uint64_t cycle){

    if(exception == false){ 
	uint16_t address = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

	if(cpu.state->a == PRINT_B){
		std::cout << "System Call: PRINT_B" << std::endl;
		std::cout << "Value of Register B: " << (int)cpu.state->b << std::endl << std::endl;
		return CYCLES_TEN;	
	}

	else if(cpu.state->a == PRINT_MEM){
		std::cout << "System Call: PRINT_MEM" << std::endl;
		std::cout << "Content value of Memory(BC): " << (int)cpu.memory->at(address) << std::endl << std::endl;
		return CYCLES_TEN;	
	}

	else if(cpu.state->a == READ_B){
		std::cout << "System Call: READ_B" << std::endl;
		int valueB;
		std::cout << "Enter a value between 0 and 255 for Register B: ";
		std::cin >> valueB;
		std::cout << std::endl;
		if(valueB<0 || valueB>255){
	        std::cout << "uint8_t error !!! Zero is assigned to Register B" << std::endl;
	        cpu.state->b = (uint8_t)0;
	    }else
        	cpu.state->b = (uint8_t)valueB;
		std::cin.clear();
		return CYCLES_TEN;
	}

	else if(cpu.state->a == READ_MEM){
		std::cout << "System Call: READ_MEM" << std::endl;
		uint8_t num;
		std::cout<<"Enter a value between 0 and 255 for Memory(BC): ";
		std::cin >> num;

		if(num<0 || num>255){
	        std::cout << "uint8_t error !!! Zero is assigned to Memory B" << std::endl;
	        cpu.memory->at(address) = (uint8_t)0;
	    }else
	        cpu.memory->at(address) = (uint8_t)num;

	    std::cout << "Address value of Memory(BC): "<<unsigned(address) << std::endl << std::endl;
		std::cin.clear();
		return CYCLES_TEN;
	}

	else if(cpu.state->a == PRINT_STR){
            std::cout << "System Call: PRINT_STR" << std::endl;
            uint16_t i = address;
            while(true){	
                if(cpu.memory->at(i) != (uint8_t)0)
                    std::cout << (char)cpu.memory->at(i++);
                else
                    break;
            }
            std::cout << std::endl;
            return CYCLES_HUNDRED;
	}

	else if(cpu.state->a == READ_STR){
		std::cout << "System Call: READ_STR" << std::endl;
		std::string str = "";
		std::cin >> str;
		
		uint16_t i;
		for (int j = 0 , i = address ; j < str.length(); i++, j++)
		{	
                    cpu.memory->at(i) = str[j];
		}
		cpu.memory->at(i) = (uint8_t)'\0';
		std::cout<<"The string is written on the address: "<<unsigned(address)<<std::endl;
		std::cin.clear();
		return CYCLES_HUNDRED;
	}

	else if(cpu.state->a == GET_RND){
		std::cout << "System Call: GET_RND" << std::endl;
		int randomNumber = (rand() % 255 + 0);
		cpu.state->b = ((uint8_t)randomNumber);
		return CYCLES_FIVE;
	}


	////////////////////////////////////////////
	else if(cpu.state->a == TCreate){
            std::cout << "System Call: TCreate" << std::endl;
            do{
                THREAD thread;
                
                if(deleteTid.size() != 0){
                    thread.threadID = deleteTid.at(0);
                    deleteTid.erase(deleteTid.begin());
                }
                else
                    thread.threadID = (uint64_t) GTUOS::tid++;
                
                thread.regesters = *cpu.state;
                thread.state = READY;
                thread.time = cycle;
                thread.usedCycle = (uint64_t) GTUOS::tidCount++;
                thread.startingAddress = address;
                thread.regesters.pc = address;
                thread.stackSpace = true;

                threadTable.push_back(thread);
                cpu.state->b = (uint8_t) thread.threadID;
            }while(threadTable.size() < 2);
                      
            return CYCLES_EIGHTY;
	}
	else if(cpu.state->a == TExit){
            std::cout << "System Call: TExit" << std::endl;
            if(threadTable.size() > 1 && threadTable.at(0).threadID != 1000){
                deleteTid.push_back(threadTable.at(0).threadID);
                threadTable.erase(threadTable.begin());
                for(int i = 0; i < threadTable.size(); i++){
                    if(threadTable.at(i).threadID == 1000){
                        threadTable.at(i).state = RUNNING;
                    }
                }
            }
            
            return CYCLES_FIFTY;
	}
	else if(cpu.state->a == TJoin){
            std::cout << "System Call: TJoin" << std::endl;
            
            if(threadTable.at(0).threadID == 1000){
                threadTable.at(0).regesters = *cpu.state;
                threadTable.at(0).state = BLOCKED;
            }
            
                
            return CYCLES_FOURTY;
	}
	else if(cpu.state->a == TYield){
            std::cout << "System Call: TYield" << std::endl;
            
            if(threadTable.size() > 1 && cycle >= GTUOS::quantumTime){

                do{ 
                    if(threadTable.at(0).state != BLOCKED)
                        threadTable.at(0).regesters = *cpu.state;
                    switchThread();
                }while(threadTable.at(0).state == BLOCKED);
                threadTable.at(0).state = RUNNING;
                *cpu.state = threadTable.at(0).regesters;
                cycle = 0;
            }
            if(threadTable.size() == 1){
                *cpu.state = threadTable.at(0).regesters;
                threadTable.at(0).state = BLOCKED;
                threadTable.erase(threadTable.begin());
                cycle = 0;
            }
            return CYCLES_FOURTY;
	}
        
        //////////////////////// HW 3 //////////////////////////////////////////
        else if(cpu.state->a == FileCreate){
            
            std::cout << "System Call: FileCreate" << std::endl;
            FILES newFile;
            std::string fileName = "";
            uint16_t i = address;
            while(true){	
                if(cpu.memory->at(i) != (uint8_t)0)
                    fileName += (char)cpu.memory->at(i++);
                else
                    break;
            }
            
            for(int i = 0; i < fileTable.size(); i++){
                if(fileName.compare((fileTable.at(i)).fileName) == 0){
                    std::cout<< "This file already exists." << std::endl;
                    exception = true;
                    return 0;
                }     
            }
            newFile.fileName = fileName;
            newFile.fileCreation = getTime();
            newFile.lastModification = getTime();
            newFile.lastAccess = getTime();
            newFile.fileSize = 0;
            newFile.content = "";
            
            fileTable.push_back(newFile);
            
            return 1;
        }
        else if(cpu.state->a == FileClose){
            std::cout << "System Call: FileClose" << std::endl;
            if(cpu.state->b == handleID){
                fileTable.at(handleID).openOrClose = false;
                cpu.state->b = 1;
                handleID = -1;
                return 1;
            }
            cpu.state->b = 0;
            
            return 0;
            
        }
        else if(cpu.state->a == FileOpen){
            std::cout << "System Call: FileOpen" << std::endl;
            
            std::string fileName = "";
            uint16_t i = address;
            while(true){	
                if(cpu.memory->at(i) != (uint8_t)0)
                    fileName += (char)cpu.memory->at(i++);
                else
                    break;
            }
            
            for(int i = 0; i < fileTable.size(); i++){
                if(fileName.compare((fileTable.at(i)).fileName) == 0){
                    std::cout<< "This file already exists." << std::endl;
                    handleID = i;
                    cpu.state->b = (uint8_t) i;
                    fileTable.at(handleID).openOrClose = true;
                    return 1;
                    
                }     
            }
            std::cout<< "This file already not exists." << std::endl;
            handleID = -1;
            cpu.state->b = (uint8_t) -1;
            exception = true;
            return 0;
        }
        else if(cpu.state->a == FileRead){
            std::cout << "System Call: FileRead" << std::endl;
            
            if(handleID == cpu.state->e){
                (fileTable.at(handleID)).lastAccess = getTime();
                uint16_t i = address;
                int j=0,k=0;
                while(k < cpu.state->d){
                    
                    if((fileTable.at(handleID).content)[j] == ' '){
                        cpu.memory->at(i) = ' ';
                        i++;
                        j++;
                    }
                    else
                        k++;
                    
                    
                    cpu.memory->at(i) = (fileTable.at(handleID).content)[j++];
                    seekValue++;
                    i++;
                }
                cpu.memory->at(i) = '\0';
                cpu.state->b = i+seekValue;
            }
            return 0;
        }
        else if(cpu.state->a == FileWrite){
            std::cout << "System Call: FileWrite" << std::endl;
            
            std::string content = "";
            uint16_t i = address;
            for(int j =0; j < cpu.state->d; j++){
                
                
                content += std::to_string((int)cpu.memory->physicalAt(i++)) + " ";
                seekValue++;
            }
            content += '\0';
            (fileTable.at(cpu.state->e)).lastModification = getTime();
            (fileTable.at(cpu.state->e)).lastAccess = getTime();
            (fileTable.at(cpu.state->e)).fileSize = content.size();
            (fileTable.at(cpu.state->e)).content = content;
            cpu.state->b = seekValue;
            return seekValue;
            
        }
        else if(cpu.state->a == FileSeek){
            std::cout << "System Call: FileSeek" << std::endl;
            int i;
            int sizeOfContent;
            sizeOfContent = fileTable.at(handleID).content.size();

            for(i =address ; unsigned(cpu.memory->at(i)) != '\0'; i++){
                if(i > sizeOfContent)
                    cpu.state->b = 0;
                    return 0;
            }
            sizeOfContent -= i;
            seekValue = sizeOfContent;
            std::cout<<"fileSeek position: "<< seekValue << std::endl;
            cpu.state->b = 1;
            return 1;
        }
        else if(cpu.state->a == DirRead){
            std::cout << "System Call: DirRead" << std::endl;
            
            char tilda = '~';
            uint16_t k = address;
            for (int i = 0; i < fileTable.size(); i++) {
                for(int j = 0; j < fileTable.at(i).fileName.size(); j++)
                    cpu.memory->at(k++) = (fileTable.at(i).fileName)[j];
                cpu.memory->at(k++) = tilda;
                for(int j = 0; j < fileTable.at(i).fileCreation.size(); j++)
                    cpu.memory->at(k++) = (fileTable.at(i).fileCreation)[j];
                cpu.memory->at(k++) = tilda;
                for(int j = 0; j < fileTable.at(i).lastModification.size(); j++)
                    cpu.memory->at(k++) = (fileTable.at(i).lastModification)[j];
                cpu.memory->at(k++) = tilda;
                for(int j = 0; j < fileTable.at(i).lastAccess.size(); j++)
                    cpu.memory->at(k++) = (fileTable.at(i).lastAccess)[j];
                cpu.memory->at(k++) = tilda;
                std::string fileSize = std::to_string(fileTable.at(i).fileSize);
                for(int j = 0; j < fileSize.size(); j++)
                    cpu.memory->at(k++) = fileSize[j];
                cpu.memory->at(k++) = ';';
                cpu.memory->at(k++) = '\n';
                
            }
            cpu.memory->at(k++) = '\0';
            return 0;

        }

	std::cout <<  "Unimplemented OS call";
	throw -1;
    }
    return 0;
}

void GTUOS::WriteToFileFromMemory(const CPU8080 & cpu){

	FILE *filePtr;

	filePtr = fopen("exe.mem" , "w");	

	if(filePtr == NULL){ 
		fprintf(stderr, "File did not open in write mode.\n");
		exit(1);
	}	

	for(uint32_t i=0x0; i < 0x10000; i += 0x00010){
		fprintf(filePtr, "%.4x:		", i);
		for(uint16_t j = 0x00000; j < 0x00010; j++){
                    fprintf(filePtr, "%.2x ", cpu.memory->at(i+j));
		}
		fprintf(filePtr, "\n");
	}


	fclose(filePtr);
	return;
}

void GTUOS::Debug3_Print(){
    std::cout<<"(TID)\t(REGISTER)\t(TIME OF THREAD)\t(THREAD HAS USED)\t(STATE)\t(ADDRESS OF MEMEORY)\t(EMPTY STACK SPACE)"<<std::endl;
    for(int i = 0; i < threadTable.size(); i++){
        std::cout << threadTable.at(i).threadID <<"\t\t"
                  << (int)threadTable.at(i).regesters. b <<"\t\t"
                  << threadTable.at(i).time <<"\t\t"
                  << threadTable.at(i).usedCycle <<"\t\t\t"; 
        if(threadTable.at(i).state == RUNNING)
            std::cout << "RUNNING\t\t\t";
        else if(threadTable.at(i).state == READY)
            std::cout << "READY\t\t\t";
        else if(threadTable.at(i).state == BLOCKED)
            std::cout << "BLOCKED\t\t\t";
        std::cout << threadTable.at(i).startingAddress <<"\t\t\t"; 
        if(threadTable.at(i).stackSpace == true)
            std::cout <<"TRUE"<<std::endl;
        else
            std::cout <<"FALSE"<<std::endl;
    }
}

void GTUOS::switchThread(){
    if(threadTable.size() > 1){
        if(threadTable.at(0).state != BLOCKED)
            threadTable.at(0).state = READY;
        threadTable.push_back(threadTable.at(0));
        threadTable.erase(threadTable.begin());
    }
    
}

void GTUOS::robin(int & cycle, CPU8080 & cpu){

    if(threadTable.size() > 1 && cycle >= GTUOS::quantumTime){

        do{ 
            if(threadTable.at(0).state != BLOCKED)
                threadTable.at(0).regesters = *cpu.state;
            switchThread();
        }while(threadTable.at(0).state == BLOCKED);
        threadTable.at(0).state = RUNNING;
        *cpu.state = threadTable.at(0).regesters;
        cycle = 0;
    }
    if(threadTable.size() == 1){
        *cpu.state = threadTable.at(0).regesters;
        threadTable.at(0).state = BLOCKED;
        threadTable.erase(threadTable.begin());
        cycle = 0;
    }

    
}


void GTUOS::writeFileSystemDat(std::string fileSystemDat) {
    remove(fileSystemDat.c_str());
    
    std::string tilda = "~";
    std::string tempStr ="";
    std::string writeStr = "";
    std::ofstream filewriteDat;
    filewriteDat.open(fileSystemDat, std::ios::out);
    for(int i = 0; i < fileTable.size(); i++){
        tempStr = fileTable.at(i).fileName + tilda + fileTable.at(i).fileCreation + tilda + 
                fileTable.at(i).lastModification + tilda + fileTable.at(i).lastAccess + tilda + 
                std::to_string(fileTable.at(i).fileSize) + tilda + fileTable.at(i).content + ";";
        

            filewriteDat << tempStr;
            if(tempStr.size() < 256){
                for(int j = 0; j < (256 - tempStr.size()); j++){
                    filewriteDat << '\0';
                }
            }
            tempStr = "";

        writeStr += tempStr;
    }
    
    if(writeStr.size() < (1024*1024)){
        for(int j = 0; j < ((1024*1024) - writeStr.size()); j++)
            filewriteDat << '\0';
    }
    
    
    filewriteDat.close();
}

void GTUOS::readFileSystemDat(std::string fileSystemDat){
    int start = 0;
    int end = 256;
    int i = 1;
    std::ifstream filereadDat(fileSystemDat, std::ios::in);
    if(!filereadDat.is_open()){
        std::cerr<<"Unable to open "<<fileSystemDat<<" file!"<<std::endl;
        exit(EXIT_FAILURE);
    }else{
        std::string str ="";
        std::string tempStr="";
        FILES newFile;
        while (!filereadDat.eof()){
            getline(filereadDat, tempStr);
        }
        filereadDat.close();

        while(tempStr.size() != 0){
            if(tempStr.find(";") == std::string::npos){
                tempStr.erase(start, tempStr.size());
                break;
            }
            if(tempStr[0] == '\0'){
                int count = 0;
                for(int i = 0; i < tempStr.size();i++){
                    if(tempStr[i] == '\0')
                        count++;
                    else
                        break;
                }
                tempStr.erase(0,count);
            }
            str = tempStr.substr(start,tempStr.find(';')+1);
            tempStr.erase(start, tempStr.find(';')+1);

            bool flag = true;
            std::string fileName = parse(str);
            for(int i = 0; i < fileTable.size(); i++){
                if(fileName.compare((fileTable.at(i)).fileName) == 0){
                    std::cout<< "This file already exists." << std::endl;
                    flag = false;
                }  
            }
            if(flag == true){
                newFile.fileName = fileName;
                newFile.fileCreation = parse(str);
                newFile.lastModification = parse(str);
                newFile.lastAccess = parse(str);
                newFile.fileSize = atoi((parse(str).c_str()));
                newFile.content = str.substr(0,str.length()-1);
                str = "";
                fileTable.push_back(newFile);
            }

        }
    }

}

std::string GTUOS::parse(std::string& str){
    size_t pos = 0;
    int cammaPos = 0;
    std::string retString = "";

    if((pos = str.find("~")) != std::string::npos){
       retString = str.substr(0,pos);
       str.erase(0,pos+1);
    }
    return retString;
}


std::string GTUOS::getTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);

    return str;
}
void GTUOS::Debug1Print(){
    std::string minus = " - ";
    std::string printStr = "";
    for(int i = 0; i < fileTable.size(); i++){
        if(fileTable.at(i).openOrClose == true){
            std::cout << "DEBUG Mode 1" << std::endl;
            printStr += fileTable.at(i).fileName + minus + fileTable.at(i).fileCreation + minus + 
                fileTable.at(i).lastModification + minus + fileTable.at(i).lastAccess + minus + 
                std::to_string(fileTable.at(i).fileSize) + " bytes" + ";\n";
            std::cout<< printStr << std::endl;
        }
    }
}
void GTUOS::endOfSimulation(){
    
    std::string minus = " - ";
    std::string printStr = "";
    for(int i = 0; i < fileTable.size(); i++){
        printStr += fileTable.at(i).fileName + minus + fileTable.at(i).fileCreation + minus + 
                fileTable.at(i).lastModification + minus + fileTable.at(i).lastAccess + minus + 
                std::to_string(fileTable.at(i).fileSize) + " bytes" + ";\n";
    }
    std::cout<< printStr << std::endl;
    
}

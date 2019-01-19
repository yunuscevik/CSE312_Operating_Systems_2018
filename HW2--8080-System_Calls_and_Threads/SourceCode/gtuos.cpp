#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <math.h>
#include "8080emuCPP.h"
#include "gtuos.h"

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
const uint64_t CYCLES_FIVE = 5;
const uint64_t CYCLES_TEN = 10;
const uint64_t CYCLES_HUNDRED = 100;
const uint64_t CYCLES_EIGHTY = 80;
const uint64_t CYCLES_FIFTY = 50;
const uint64_t CYCLES_FOURTY = 40;
uint64_t GTUOS::tid = 1000;
uint64_t GTUOS::tidCount = 1;
uint64_t GTUOS::handleCall(const CPU8080 & cpu, uint64_t cycle){


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
            while(true)
            {	
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

	std::cout <<  "Unimplemented OS call";
	throw -1;
	
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
    /// bak

    
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


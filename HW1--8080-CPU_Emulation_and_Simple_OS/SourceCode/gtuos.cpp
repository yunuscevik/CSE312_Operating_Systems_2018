#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "8080emuCPP.h"
#include "gtuos.h"

const uint8_t PRINT_B = 1;
const uint8_t PRINT_MEM = 2;
const uint8_t READ_B = 3;
const uint8_t READ_MEM = 4;
const uint8_t PRINT_STR = 5;
const uint8_t READ_STR = 6;
const uint8_t GET_RND = 7;
const uint64_t CYCLES_FIVE = 5;
const uint64_t CYCLES_TEN = 10;
const uint64_t CYCLES_HUNDRED = 100;

uint64_t GTUOS::handleCall(const CPU8080 & cpu){

	// Use for address. Almost all of the calls requirement that 
	// generate address from b and c registers.
	uint16_t address = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
	// Print num
	if(cpu.state->a == PRINT_B){
		std::cout << "System Call: PRINT_B" << std::endl;
		std::cout << "Value of Register B: " << (int)cpu.state->b << std::endl << std::endl;
		return CYCLES_TEN;	
	}
	// Print memory content
	else if(cpu.state->a == PRINT_MEM){
		std::cout << "System Call: PRINT_MEM" << std::endl;
		std::cout << "Content value of Memory(BC): " << (int)cpu.memory->at(address) << std::endl << std::endl;
		return CYCLES_TEN;	
	}
	// Read an integer. Integer will be casted to 8 bit integer.
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
	// Read memory content
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
	// Print a string
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
	// Read a string. String will be read until there will be a whitespace char.
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
	//
	else if(cpu.state->a == GET_RND){
		std::cout << "System Call: GET_RND" << std::endl;
		int randomNumber = (rand() % 255 + 0);
		cpu.state->b = ((uint8_t)randomNumber);
		return CYCLES_FIVE;
	}


	// Case of unimplemented call. 
	std::cout <<  "Unimplemented OS call";
	throw -1;
	
	return 0;
}
// Writes memory to file "exe.mem" 
void GTUOS::WriteToFileFromMemory(const CPU8080 & cpu){

	FILE *filePtr;

	filePtr = fopen("exe.mem" , "w");	 // open

	if(filePtr == NULL){ // control
		fprintf(stderr, "File did not open in write mode.\n");
		exit(1);
	}	
	// Write part
	for(uint32_t i=0x0; i < 0x10000; i += 0x00010){
		fprintf(filePtr, "%.4x:		", i);
		for(uint16_t j = 0x00000; j < 0x00010; j++){
			fprintf(filePtr, "%.2x ", cpu.memory->at(i+j));
		}
		fprintf(filePtr, "\n");
	}


	fclose(filePtr); // close
	return;
}


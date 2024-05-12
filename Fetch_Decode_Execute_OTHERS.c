#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/memory.h"
#include "header/parse.h"
int registers[32];
int programCounter=0;
int memoryDataRegister;
int memoryAdressRegister;
int temporayRegister;
char instructionRegister[33];
char executeInstruction[33];
int numberofinstructions;
int cycle=0;
int opcodeInt;
char flag;
int reg1;
int reg2;
int reg3;
int imm_value;
int shamt;
int address;
int states[]={-1,-1,-1,-1,-1,-1,-1};
int decodecycle=1;
int executeCycle=1;
int result;
int memoryAccessOn=0;
int writeBackOn=0;
int opcodetemp;
int dispatch=0;
int cycletoggle=0;
int writeBacktemp;


int binaryToDecimal(char *binary) {
    int decimal = 0;
    int size = strlen(binary);
    int base = 1;

    for(int i=size-1; i>=0; i--) {
        if(binary[i] == '1') {
            decimal += base;
        }
        base = base * 2;
    }

    // If the number is negative (starts with 1 in 2's complement)
    if(binary[0] == '1') {
        decimal = decimal - base;
    }

    return decimal;
}


 void intToBinary(int n, char *str, int str_size) {
    str[str_size-1] = '\0';
    for(int i = str_size - 2; i >= 0; --i) {
        str[i] = (n & 1) ? '1' : '0';
        n >>= 1;
    }
 }


    

    void fetch(){
        // printf("programCounter: %d\n",programCounter);
        // printf("memory[programCounter] %s\n",memory[programCounter]);
        // printf("instructionRegister: %s\n",instructionRegister);
        strcpy(instructionRegister,memory[programCounter]); 
        // printf("instructionRegister: %s\n",instructionRegister);
        states[0]=programCounter;
    }

    void decode(){
        
        if(decodecycle==1){
            decodecycle=0;
        }
        else {
            // printf("entered Decode\n");
        char opcode[5];
        strncpy(opcode, instructionRegister, 4);
        //opcode[5] = '\0'; // Ensure null-termination
        opcodeInt = strtol(opcode,NULL, 2);
        switch (opcodeInt)
        {
        case 0:
            flag = 'R';
            break;
        case 1:
            flag = 'R';
            break;
        case 2:
            flag = 'R';
            break;
        case 3:
            flag = 'I';
            break;
        case 4:
            flag = 'I';
            break;
        case 5:
            flag = 'R';
            break;
        case 6:
            flag = 'I';
            break;
        case 7:
            flag = 'J';
            break;
        case 8:
            flag = 'R';
            break;
        case 9:
            flag = 'R';
            break;
        case 10:
            flag = 'I';
            break;
        case 11:
            flag = 'I';
            break;

        default:
            break;
        }

        //printf("%d\n",opcodeInt);

        if(flag=='R')
     {  
        char temp1[6]="00000";
        char temp2[6]="00000";
        char temp3[6]="00000";
        char temp4[14]="0000000000000";
        
        strncpy(temp1,instructionRegister+4,5);
        strncpy(temp2,instructionRegister+9,5);
        strncpy(temp3,instructionRegister+14,5);
        strncpy(temp4,instructionRegister+19,13);
        
        reg1=binaryToDecimal(temp1);
        reg2=binaryToDecimal(temp2);
        reg3=binaryToDecimal(temp3);
        shamt=binaryToDecimal(temp4);
    


     }

     if(flag=='I')
     {
        char temp1[7]="00000";
        char temp2[6]="00000";
        char imm[19]="000000000000000000";
       
        strncpy(temp1,instructionRegister+4,5);
        char temp[8]; // One extra space for the null terminator
        sprintf(temp, "0%s", temp1);
        strcpy(temp1, temp);
        strncpy(temp2,instructionRegister+9,5);
        
        strncpy(imm,instructionRegister+14,18);
        
        reg1=binaryToDecimal(temp1);
        reg2=binaryToDecimal(temp2);
        imm_value=binaryToDecimal(imm);
            }

            decodecycle=1;
            strcpy(executeInstruction,instructionRegister); 
            
        }
        

    }


void execute(){
    if(executeCycle==1)
        executeCycle=0;
    else {
        if(opcodeInt==0){  //exec of add operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            printf("operand 1 = %d\n",registers[reg2]);
            printf("operand 2 = %d\n",registers[reg3]);
            printf("destination register = %d\n",reg1);
            result=v1+v2;
            temporayRegister=reg1;

            writeBackOn=1;//correct??
           

        }

        if(opcodeInt==1){  //exec of sub operation
            int v1=registers[reg2];
            int v2=registers[reg3];//why immediate not r3
            printf("operand 1 = %d\n",registers[reg2]);
            printf("operand 2 = %d\n",registers[reg3]);
            printf("destination register = %d\n",reg1);
            result=v1-v2;
            temporayRegister=reg1;
            writeBackOn=1;
            

        }

        if(opcodeInt==2){  //exec of mul operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            printf("operand 1 = %d\n",registers[reg2]);
            printf("operand 2 = %d\n",registers[reg3]);
            printf("destination register = %d\n",reg1);
            result=v1*v2;
            temporayRegister=reg1;
            writeBackOn=1;

        }

        if(opcodeInt==3){  //exec of MOVI operation
            result=imm_value;
            temporayRegister=reg1;
            writeBackOn=1;
            
            
            printf("Immediate value = %d\n",imm_value);
            printf("destination register = %d\n",reg1);

        }

        if(opcodeInt==4){  //exec of JEQ operation
            int v1=registers[reg1];
            int v2=registers[reg2];
            printf("operand 1 = %d\n",registers[reg2]);
            printf("operand 2 = %d\n",registers[reg3]);
            printf("Immediate value = %d\n",imm_value);
            if(v1==v2){
                
                 programCounter=programCounter+imm_value-1;
                  //should this be in write back ?
                  dispatch=1;
                  
                }
           


        }

        if(opcodeInt==5){  //exec of and operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            result=v1&v2;
            temporayRegister=reg1; 
            writeBackOn=1;
            printf("operand 1 = %d\n",registers[reg2]);
            printf("operand 2 = %d\n",registers[reg3]);
            printf("destination register = %d\n",reg1);
          
        }
        

        if(opcodeInt==6){  //exec of XORI operation
            int v1=registers[reg2];
            int v2=imm_value;
            result=v1^v2;
           
            temporayRegister=reg1; 
            printf("operand 1 = %d\n",registers[reg2]);
            printf("operand 2 = %d\n",imm_value);
            printf("destination register = %d\n",reg1);
            writeBackOn=1;
           
        }

        if(opcodeInt==7){   //exec of JMP operation
            char newpc[]="00000000000000000000000000000000";
            //print stuff
            intToBinary(programCounter-1,newpc,33);
           // printf("newpc: %s\n",newpc);
            strncpy(newpc+4,executeInstruction+4,28);
            // printf("%s \n",executeInstruction); 
           // printf("newpc: %s\n",newpc);          
            programCounter=binaryToDecimal(newpc);
            printf("address to jump to= %d\n",programCounter);  
            printf("programCounter: %d\n",programCounter);
             // should be in write back stage ?
            dispatch=1;  
            decodecycle=1;
                  
                
                 
            
         }
        if(opcodeInt==8){   //exec of LSL operation
           result=registers[reg2]<<shamt;
           temporayRegister=reg1;  
           writeBackOn=1;
           printf("operand 1 = %d\n",registers[reg2]);
            printf("shift amount = %d\n",shamt);
            printf("destination register = %d\n",reg1);
           
            
        }
        if(opcodeInt==9){   //exec of LSR operation
            result=registers[reg2]>>shamt;
            temporayRegister=reg1;
            writeBackOn=1;
            printf("operand 1 = %d\n",registers[reg2]);
            printf("shift amount = %d\n",shamt);
            printf("destination register = %d",reg1);
            
        }

         if(opcodeInt==10){  //exec of MOVR operation
            int v1=registers[reg2];
            int v2=imm_value;
            memoryAdressRegister=v1+v2;
            temporayRegister=reg1;  
            memoryAccessOn=1;
            writeBackOn=1;
            // printf("operand 1 = %d\n",registers[reg2]);
            // printf("immediate value = %d\n",imm_value);
            printf("memoryAddressRegister = %d\n",memoryAdressRegister);
            printf("destination register = %d",reg1);


        }
         if(opcodeInt==11){  //exec of MOVM operation
            temporayRegister=reg1;
            int v2=registers[reg2];
            int v3=imm_value;
            memoryAdressRegister=v2+v3;
            memoryAccessOn=1;
            // writeBackOn=1;
            printf("operand 1 = %d\n",registers[reg1]);
            printf("memoryAddressRegister = %d\n",memoryAdressRegister);
            
        }

        // printf("result: %d\n",reg1);
            executeCycle=1;
            }
        }
 
        void writeBack(){
            if(states[5]==states[6]){
            if(temporayRegister==0){
                writeBackOn=0;
                return;
            }

            else{
                if(opcodetemp==10){
                    registers[temporayRegister]=writeBacktemp;
                }
                else{
                    registers[temporayRegister]=result;
                }
            writeBackOn=0;
            printf("Register %d value changed to %d \n",temporayRegister,result);
            
        }}
        
        
        }

        void memoryAccess(){
//DECIDE UPON UPCODE TO READ OR WRITE
                if(opcodetemp==11){
                    // printf("reg:%d, value:%d\n",temporayRegister,registers[temporayRegister]);
                    intToBinary(registers[temporayRegister],memory[memoryAdressRegister],33);
                    printf("Value of Location %d in memory changed to %d \n",memoryAdressRegister,registers[temporayRegister]);
                    memoryAccessOn=0;
                }
                
                 else if(opcodetemp==10){

                    writeBacktemp=binaryToDecimal(memory[memoryAdressRegister]);
                    memoryAccessOn=0;
        }
        }

void execProgram(){
    parse();
    for(int i = 0; i < 20; i++) 
        if(memory[i][0] != '\0')
            numberofinstructions++;
    
    while(1){       
        printf("\nClock cycle number %d\n",cycle+1);

        if(states[6]!=-1){
            
            //printf("Write back stage instruction : %d \n",binaryToDecimal(memory[states[4]]));
            
            if(writeBackOn==1)
                writeBack();
        }
            
        if(states[5]!=-1){
           // printf("Memory access stage\n");
            
                memoryAccess();}
        if(states[4]!=-1 || states[3]!=-1){
            execute();
            opcodetemp=opcodeInt;
            //printf("Execute stage\n");
            }
        if(states[2]!=-1 || states[1]!=-1 )
            {decode();
            //printf("Decode stage\n");
            } 
        
        //check if there is more instructions to fetch else set it to -1
        if(cycle%2==0 && numberofinstructions>0){
            if (programCounter==numberofinstructions)
            {
                states[0]=-1;
            }
            else
            {
                //printf("Fetch stage\n");
            fetch();
            // numberofinstructions--;
            //print
            if(dispatch)
            {
                // cycletoggle=!cycletoggle;
                dispatch=0;
                // states[0]=-1;
                states[1]=-1;
                states[2]=-1;
                states[3]=-1;
                programCounter--;
                }
            programCounter++;
            }
            
        }
        
  
       
            


         cycle++;
         
     if(states[0]!=-1 &&states[0]!=states[1])
            // printf("Instruction fetched : %s\n",instructionRegister);
            printf("Instruction fetched : %s\n",instructions[states[0]]);

        if(states[1]!=-1 )
            printf("Instruction decoding : %s\n",instructions[states[1]]);
        
        if(states[3]!=-1)
           printf("Instruction executing : %s\n",instructions[states[3]]);
        if(states[4]!=-1 && states[4]!=states[5] && states[3]==-1)
           printf("Instruction executing : %s\n",instructions[states[4]]);

        
        if(states[5]!=-1 && states[5]!=states[6])
           printf("Instruction in Memory Access : %s\n",instructions[states[5]]);

        if(states[6]!=-1 && states[5]==states[6])
             printf("Instruction in WriteBack stage : %s\n",instructions[states[6]]);
       

       for(int i = 6; i > 0; i--) {
            printf("states[%d] = %d\n", i, states[i]);
        }
    //     printf("states[%d] = %d\n", 0, states[0]);
        // printf("executeCycle : %d\n", executeCycle);
        // printf("decodecycle: %d\n", decodecycle);
         for(int i = 6; i > 0; i--) {
            states[i] = states[i-1];
        }
         if( states[0]==-1 && states[1]==-1 && states[2]==-1 && states[3]==-1 && states[4]==-1 && states[5]==-1 ){
            printf("Breaked\n");
            break;
        }

    //     for(int i = 0; i < 31; i++) {
    //     printf("registers 0= %i\n", i, registers[i]);
    // }
        }

       
         //fetch instruction states print
         //Decode instruction states print
         //EXecute R-format R2 R3 R1 shiftAmount 
         //Execute I-format R2 R3 imm_value [PC]
         //Execute J-format [PC]
         //memory access stage updates & storage
         //write back stage updates & storage Registers[Temporary Registers]
       
    }
       
       




int main(){
    
    execProgram();

    printf("\nRegisters : \n");
    printf("Zero Register = %d\n",registers[0]);
    for(int i = 1; i < 32; i++) {
        printf("Register %d = %i\n", i, registers[i]);
    }
    printf("Program Counter = %d\n\n",programCounter);
    
    // int instruction_size = (sizeof(memory) / sizeof(memory[0]))/2;
    // printf("%d\n",instruction_size);
    // int datasize=(sizeof(memory) / sizeof(memory[0]))/2;

    printf("Memory : \n");
    for(int i = 0; i < 2048; i++) {
        // If the first character of the current string is not '\0', print it
        if(memory[i][0] != '\0') {
            printf("%d : %s, %s\n",i, instructions[i],memory[i]);
            
        }
    }
   



}
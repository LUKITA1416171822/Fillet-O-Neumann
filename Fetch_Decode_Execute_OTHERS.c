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
        else{
            printf("entered Decode\n");
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
        char temp1[6]="00000";
        char temp2[6]="00000";
        char imm[19]="000000000000000000";
       
       strncpy(temp1,instructionRegister+4,5);
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
        if(executeCycle==1){
        executeCycle=0;}
            else {
                printf("entered Execute\n");
                printf("opcodeInt: %d\n",opcodeInt);
            if(opcodeInt==0){  //exec of add operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            result=v1+v2;
            temporayRegister=reg1;

            writeBackOn=1;//correct??
           

        }

        if(opcodeInt==1){  //exec of sub operation
            int v1=registers[reg2];
            int v2=registers[reg3];//why immediate not r3
            result=v1-v2;
            temporayRegister=reg1;
            writeBackOn=1;
            

        }

        if(opcodeInt==2){  //exec of mul operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            result=v1*v2;
            temporayRegister=reg1;
            writeBackOn=1;
           


        }

        if(opcodeInt==3){  //exec of MOVI operation
            result=imm_value;
            temporayRegister=reg1;
            writeBackOn=1;
            
            
            printf("register: %d\n",temporayRegister);
            printf("result: %d\n",result);

        }

        if(opcodeInt==4){  //exec of JEQ operation
            int v1=registers[reg1];
            int v2=registers[reg2];
            if(v1==v2){
                
                 programCounter=programCounter+imm_value-1;
                  //should this be in write back ?
                  for (int i = 0; i < 4; i++)
                  {
                    states[i]=-1;
                  }
                  
                }
           


        }

        if(opcodeInt==5){  //exec of and operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            result=v1&v2;
            temporayRegister=reg1; 
            writeBackOn=1;
            
          
        }
        

        if(opcodeInt==6){  //exec of XORI operation
            int v1=registers[reg2];
            int v2=imm_value;
            result=v1^v2;
           
            temporayRegister=reg1; 
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
             // should be in write back stage ?
             for (int i = 0; i < 4; i++)
                  {
                    states[i]=-1;
                  }
            
         }
        if(opcodeInt==8){   //exec of LSL operation
            result=reg2<<shamt;
           temporayRegister=reg1;  
           writeBackOn=1;
           
            
        }
        if(opcodeInt==9){   //exec of LSR operation
            result=reg2>>shamt;
            temporayRegister=reg1;
            writeBackOn=1;
           
            
        }

         if(opcodeInt==10){  //exec of MOVR operation
            int v1=registers[reg2];
            int v2=imm_value;
            memoryAdressRegister=v1+v2;
            temporayRegister=reg1;  
            memoryAccessOn=1;
            writeBackOn=1;
           


        }
         if(opcodeInt==11){  //exec of MOVM operation
            temporayRegister=reg1;
            int v2=registers[reg2];
            int v3=imm_value;
            memoryAdressRegister=v2+v3;
            memoryAccessOn=1;
            writeBackOn=1;
           
            
        }

        printf("result: %d\n",reg1);
            executeCycle=1;
            }
        }
 
        void writeBack(){
            registers[temporayRegister]=result;
            printf("registers[%d] write %d on \n",temporayRegister,result);
            writeBackOn=0;
        }

        void memoryAccess(){
//DECIDE UPON UPCODE TO READ OR WRITE
                intToBinary(registers[temporayRegister],memory[memoryAdressRegister],33); // not sure if handles negative numbers 
                memoryAccessOn=0;
        }

void execProgram(){
    parse();
    for(int i = 0; i < 20; i++) 
        if(memory[i][0] != '\0')
            numberofinstructions++;
    
    int opcodetemp;
    while(1){       
        printf("\nClock cycle number %d\n",cycle+1);

        if(states[6]!=-1){
            
            printf("Write back stage instruction : %d \n",binaryToDecimal(memory[states[4]]));
            
            if(writeBackOn==1)
                writeBack();
        }
            
        if(states[5]!=-1){
            printf("Memory access stage\n");
            if(memoryAccessOn==1)
                memoryAccess();}
        if(states[4]!=-1 || states[3]!=-1){
            execute();
            // opcodetemp=opcodeInt;
            printf("Execute stage\n");}
        if(states[2]!=-1 || states[1]!=-1 )
            {decode();
            printf("Decode stage\n");} 
        
        //check if there is more instructions to fetch else set it to -1
        if(cycle%2==0 && numberofinstructions>0){
            printf("Fetch stage\n");
            fetch();
            numberofinstructions--;
            programCounter++;
        }
        else if(numberofinstructions==0){
            states[0]=-1;
        }
  
       
            

       
         

         cycle++;
         
        // if(states[0]!=-1)
        //     printf("Instruction fetched : %s\n",instructionRegister);
        // if(states[1]!=-1) 
        //     printf("Instruction decoded : %s\n",memory[states[1]]);
        // if(states[2]!=-1)
        //    printf("Instruction executed : %s\n",memory[states[2]]);
       

       for(int i = 6; i > 0; i--) {
            printf("states[%d] = %d\n", i, states[i]);
        }
        printf("states[%d] = %d\n", 0, states[0]);
         for(int i = 6; i > 0; i--) {
            states[i] = states[i-1];
        }
         if( states[0]==-1 && states[1]==-1 && states[2]==-1 && states[3]==-1 && states[4]==-1 && states[5]==-1 && states[6]==-1){
            printf("Breaked\n");
            break;
        }
    //     for(int i = 0; i < 4; i++) {
    //     printf("registers[%d] = %i\n", i, registers[i]);
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
    // char line[33];
    //  printf("Enter the number of instructions\n");

    for(int i = 0; i < 4; i++) {
        printf("registers[%d] = %i\n", i, registers[i]);
    }
    printf("memory[%d] = %s\n", 6, memory[6]);
    // for(int i = 0; i < 20; i++) {
    //     printf("memory[%d] = %s\n", i, memory[i]);
    // }
    // FILE *file = fopen("Output.txt", "r");
    // if (file == NULL) {
    //     printf("Failed to open file\n");
    //     return;
    // }

    // while (fgets(line, sizeof(line), file)) 
    //     numberofinstructions++;
    // while (fgets(line, sizeof(line), file)) {
    //     strcpy(instructionRegister, line);
    //     // decode & execute
    // }
        
    
    

// initRegisters();
// strcpy(memory[0], "01111100100110101010011111111111");
// fetch();
// decode();
// execute();
// printf("%s\n",programCounter);

// char address[]="0000000000000000000000000000";
//             strncpy(address,instructionRegister+4,28);
//             strncpy(programCounter+4,address,28);

// while (1)
// {
//    memoryAdressRegister=strtol(programCounter,NULL, 2);
//    incrementProgramCounter(programCounter);
//    strcpy(memoryDataRegister, memory[memoryAdressRegister]);
//    strcpy(instructionRegister,memoryDataRegister);
//    printf("instructionRegister: %s\n",instructionRegister);
   
// // for (int i = 0; i <= 4; i++) {
// //     printf("instruction: %c\n",opcode[i]);
// // }
     
//     // printf("memmoryAdressRegister: %d\n",memoryAdressRegister);
//     // printf("programCounter: %s\n",programCounter);
//     // printf("flag: %c\n",flag); 
//     // printf("Instruction: %s\n",instructionRegister);
   
//     //  int R25 = strtol(registers[25],NULL, 2);
//     // printf("Reg 25 is: %d\n", R25);

  

//     // char a[7]="011010";
//     // char b[7]="010011";
//     // int a1=strtol(a,NULL,2);
//     // int b1=strtol(b,NULL,2);
//     // int c=a1&b1;
//     // char r[7];
//     // intToBinary(c,r,7);
//     // printf("%d\n",a1);
//     // printf("%d\n",b1);
//     // printf("%d\n",c);
//     // printf("res is : %s\n",r);

//      break;
// }



}
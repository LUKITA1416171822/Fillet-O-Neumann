#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int registers[32];
int programCounter=0;
char memory[2048][33];
int memoryDataRegister;
int temporayRegister;
// int memoryAdressRegister;
char instructionRegister[33];
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
int numofcycles;
int states[]={-1,-1,-1,-1,-1};
int decodecycle=1;
int executeCycle=1;
int result;



int binaryToDecimal(char *binary) {
    int decimal = 0;
    int size = strlen(binary);
    int base = 1;

    for(int i=size-1; i>=0; i--) {
        if(binary[i] == '1') {
            decimal += base;
            printf("%d\n",decimal);
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

        strcpy(instructionRegister,memory[programCounter]); 
        states[0]=programCounter;
    }

    void decode(){
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
        
        int reg1=binaryToDecimal(temp1);
        int reg2=binaryToDecimal(temp2);
        int reg3=binaryToDecimal(temp3);
        int shamt=binaryToDecimal(temp4);




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
            decodecycle=0;

    }


        void execute(){
        if(opcodeInt==0){  //exec of add operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            result=v1+v2;
            registers[reg1]=result;

        }

        if(opcodeInt==1){  //exec of sub operation
            int v1=registers[reg1];
            int v2=imm_value;
            result=v1-v2;
            registers[reg1]=result;// should be in write back

        }

        if(opcodeInt==2){  //exec of mul operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            result=v1*v2;
            registers[reg1]=result;// should be in write back

        }

        if(opcodeInt==3){  //exec of MOVI operation
            result=imm_value;
            registers[reg1]=result;// should be in write back

        }

        if(opcodeInt==4){  //exec of JEQ operation
            int v1=registers[reg1];
            int v2=registers[reg2];
            if(v1==v2){
                 programCounter=programCounter+imm_value; //should be in write back
                }

        }

        if(opcodeInt==5){  //exec of and operation
            int v1=registers[reg2];
            int v2=registers[reg3];
            result=v1&v2;
            registers[reg1]=result; // should be in write back

        }
        

        if(opcodeInt==6){  //exec of XORI operation
            int v1=registers[reg2];
            int v2=imm_value;
            result=v1^v2;
           
            registers[reg1]=result; // should be in write back stage

        }

        if(opcodeInt==7){   //exec of JMP operation
            char newpc[]="00000000000000000000000000000000";
            intToBinary(programCounter,newpc,33);
            strncpy(newpc+4,instructionRegister+4,28);
            programCounter=binaryToDecimal(newpc); // should be in write back stage
            
         }
        if(opcodeInt==8){   //exec of LSL operation
            result=reg2<<shamt;
            registers[reg1]=result;  // should be in write back stage
            
        }
        if(opcodeInt==9){   //exec of LSR operation
            result=reg2>>shamt;
            registers[reg1]=result;  // should be in write back stage
            
        }

         if(opcodeInt==10){  //exec of MOVR operation
            int v1=registers[reg2];
            int v2=imm_value;
            int v3=v1+v2;
            result=binaryToDecimal(memory[v3]);    //should be in memory access stage
            registers[reg1]=result;  // should be in write back stage

        }
         if(opcodeInt==11){  //exec of MOVM operation
            int v1=registers[reg1];
            int v2=registers[reg2];
            int v3=imm_value;
            int v4=v2+v3;
            intToBinary(v1,memory[v4],33); // not sure if handles negative numbers  //should be in memory access stage  
        }

        
       executeCycle=0;
        }
void execProgram(){
    printf("Clock cycle number %d\n",cycle);
       while(1){
       if(cycle%2==1 && numberofinstructions>0){
            fetch();
            numberofinstructions--;
             }
              

         if(states[4]!=-1)
            // writeBack();
            printf("Write back stage\n");
         if(states[3]!=-1)
            // memoryAccess();
            printf("Memory access stage\n");
        if(states[2]!=-1 && executeCycle==1)
            {execute();
            printf("Execute stage\n");}
         else{
             executeCycle=1;       
         } 
        if(states[1]!=-1 && decodecycle==1)
            {decode();
            printf("Decode stage\n");} 
            else{
                decodecycle=1;
            }
             
        for(int i = 4; i > 0; i--) {
            states[i] = states[i-1];
                    }
         
         cycle++;
        
       
       }
       
       

        }


int main(){
    char line[33];

    FILE *file = fopen("Output.txt", "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) 
        numberofinstructions++;
    while (fgets(line, sizeof(line), file)) {
        strcpy(instructionRegister, line);
        // decode & execute
    }
        
    
    

// initRegisters();
// strcpy(memory[0], "01111100100110101010011111111111");
// fetch();
// decode();
// execute();
// printf("%s\n",programCounter);

int s= binaryToDecimal("11110110");
printf("%d\n",s);
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


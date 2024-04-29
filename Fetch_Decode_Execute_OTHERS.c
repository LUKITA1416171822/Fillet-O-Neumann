#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char registers[31][33];
const int ZeroRegister = 0;
char programCounter[]="00000000000000000000000000000000";
char memory[2048][33];
char memoryDataRegister[33];
int memoryAdressRegister;
char instructionRegister[33]="00000000000000000000000000000000";
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

void incrementProgramCounter(char *programCounter) {
    long int value = strtol(programCounter, NULL, 2); // Convert binary string to integer
    value++; // Increment the integer
    sprintf(programCounter, "%032ld", value); // Convert the integer back to a binary string
}

void initRegisters(){
for (int i = 0; i < 31; i++) {
   strcpy(registers[i], "0000000000000000000000000000001");
}
}

 void intToBinary(int n, char *str, int str_size) {
    str[str_size-1] = '\0';
    for(int i = str_size - 2; i >= 0; --i) {
        str[i] = (n & 1) ? '1' : '0';
        n >>= 1;
    }
 }


    

    void fetch(){
        int pc=strtol(programCounter,NULL,2);
        strcpy(instructionRegister,memory[pc]); 
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
        
        int reg1=strtol(temp1,NULL, 2);
        int reg2=strtol(temp2,NULL, 2);
        int reg3=strtol(temp3,NULL, 2);
        int shamt=strtol(temp4,NULL, 2);



        



     }

     if(flag=='I')
     {
        char temp1[6]="00000";
        char temp2[6]="00000";
        char imm[19]="000000000000000000";
       
       strncpy(temp1,instructionRegister+4,5);
       strncpy(temp2,instructionRegister+9,5);
       strncpy(imm,instructionRegister+14,18);
        
        reg1=strtol(temp1,NULL, 2);
        reg2=strtol(temp2,NULL, 2);
        imm_value=strtol(imm,NULL, 2);
       
        

            }

    }


        void execute(){
            int result;
        if(opcodeInt==0){  //exec of add operation
            int v1=strtol(registers[reg2],NULL,2);
            int v2=strtol(registers[reg3],NULL,2);
            result=v1+v2;
            intToBinary(result,registers[reg1],33); //should be in write back

        }

        if(opcodeInt==1){  //exec of sub operation
            int v1=strtol(registers[reg1],NULL,2);
            int v2=imm_value;
            result=v1-v2;
            intToBinary(result,registers[reg1],33); // should be in write back

        }

        if(opcodeInt==2){  //exec of mul operation
            int v1=strtol(registers[reg2],NULL,2);
            int v2=strtol(registers[reg3],NULL,2);
            result=v1*v2;
            intToBinary(result,registers[reg1],33); // should be in write back

        }

        if(opcodeInt==3){  //exec of MOVI operation
            result=imm_value;
            intToBinary(result,registers[reg1],33); // should be in write back

        }

        if(opcodeInt==4){  //exec of JEQ operation
            int v1=strtol(registers[reg1],NULL,2);
            int v2=strtol(registers[reg2],NULL,2);
            if(v1==v2){
                 int pc =strtol(programCounter,NULL,2);
                 pc=pc+1+imm_value;
                intToBinary(pc,programCounter,33); // should be in write back
                }

        }

        if(opcodeInt==5){  //exec of and operation
            int v1=strtol(registers[reg2],NULL,2);
            int v2=strtol(registers[reg3],NULL,2);
            result=v1&v2;
            intToBinary(result,registers[reg1],33); // should be in write back

        }
        

        if(opcodeInt==6){  //exec of XORI operation
            int v1=strtol(registers[reg2],NULL,2);
            int v2=imm_value;
            result=v1^v2;
           
            intToBinary(result,registers[reg1],33); // should be in write back stage

        }

        if(opcodeInt==7){   //exec of JMP operation
            char address[]="0000000000000000000000000000";
            strncpy(address,instructionRegister+4,28);
            strncpy(programCounter+4,address,28);   // should be in write back stage
            
        }
        if(opcodeInt==8){   //exec of LSL operation
            result=reg2<<shamt;
            intToBinary(result,registers[reg1],33);  // should be in write back stage
            
        }
        if(opcodeInt==9){   //exec of LSR operation
            result=reg2>>shamt;
            intToBinary(result,registers[reg1],33);  // should be in write back stage
            
        }

         if(opcodeInt==10){  //exec of MOVR operation
            int v1=strtol(registers[reg2],NULL,2);
            int v2=imm_value;
            int v3=v1+v2;
            result=strtol(memory[v3],NULL,2);    //should be in memory access stage
            intToBinary(result,registers[reg1],33);  // should be in write back stage

        }
         if(opcodeInt==11){  //exec of MOVM operation
            int v1=strtol(registers[reg1],NULL,2);
            int v2=strtol(registers[reg2],NULL,2);
            int v3=imm_value;
            int v4=v2+v3;
            intToBinary(v1,memory[v4],33);   //should be in memory access stage
        }

        

        }

        void execProgram(){
        cycle++;
        printf("Clock cycle number %d\n",cycle);
        if(cycle%2==1 && numberofinstructions>0){
            fetch();
            numberofinstructions--;
             }

         

        }


int main(){

initRegisters();
strcpy(memory[0], "01111100100110101010011111111111");
fetch();
decode();
execute();
printf("%s\n",programCounter);
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



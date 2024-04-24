#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char registers[31][32];
void initRegisters(){
for (int i = 0; i < 31; i++) {
   strcpy(registers[i], "0000000000000000000000000000000");
}
}
const int ZeroRegister = 0;
char programCounter[]="0000000000000000000000000000000";
char memmory[2048][32];
char memmoryDataRegister[32];
int memmoryAdressRegister;
char instructionRegister[32];
void incrementProgramCounter(char *programCounter) {
    long int value = strtol(programCounter, NULL, 2); // Convert binary string to integer
    value++; // Increment the integer
    sprintf(programCounter, "%032ld", value); // Convert the integer back to a binary string
}
int main(){

initRegisters();
strcpy(memmory[0], "000011001001101010100111111111");
while (1)
{
   memmoryAdressRegister=strtol(programCounter,NULL, 2);
   incrementProgramCounter(programCounter);
   strcpy(memmoryDataRegister, memmory[memmoryAdressRegister]);
   strcpy(instructionRegister,memmoryDataRegister);
   printf("instructionRegister: %s\n",instructionRegister);
   char opcode[5]; // Room for 4 characters and the null terminator
   strncpy(opcode, instructionRegister, 4);
   opcode[4] = '\0'; // Ensure null-termination
   char flag;
   int opcodeInt = strtol(opcode,NULL, 2);
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
     
     if(flag=='R')
     {
        for (int i = 5; i <= 9; i++) {
                printf("instruction: %c\n",instructionRegister[i]);
           registers[0][22+i]=instructionRegister[i];
           registers[1][22+i]=instructionRegister[i+5];
           registers[2][22+i]=instructionRegister[i+10];
           for (int i=0; i < 3; i++)
    {
        printf("Register %i :%s\n",i,registers[i]);
    }
            }
     }

     if(flag=='I')
     {
        for (int i = 5; i <= 9; i++) {

        //    (registers[0][22+i])=instructionRegister[i];
        //    (registers[1][22+i])=instructionRegister[i+5];
           //registers[2][22+i]=instructionRegister[i+10];
            }
     }

     if(flag=='J'){
    // int j=4;
    //  char address[32]="00000000000000000000000000000";
      for (int i = 4; i <= 31; i++) {
        programCounter[i]=instructionRegister[i];
        // printf("char: %c\n",instructionRegister[i]);
        // j++;
          }
    //  printf("address: %s\n",address);
    //    strcpy(programCounter, address);
     }
    printf("memmoryAdressRegister: %d\n",memmoryAdressRegister);
    printf("programCounter: %s\n",programCounter);
    printf("flag: %c\n",flag); 
    printf("Instruction: %s\n",instructionRegister);
    int length = sizeof(registers) / sizeof(registers[0]);
    for (int i=0; i < 3; i++)
    {
        printf("Register %i :%c\n",i,registers[0][33]);
    }
    
    break;
}



}









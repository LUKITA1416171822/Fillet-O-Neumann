#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char registers[31][33];
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
strcpy(memmory[0], "0000110010011010101001111111111");
while (1)
{
   memmoryAdressRegister=strtol(programCounter,NULL, 2);
   incrementProgramCounter(programCounter);
   strcpy(memmoryDataRegister, memmory[memmoryAdressRegister]);
   strcpy(instructionRegister,memmoryDataRegister);
   printf("instructionRegister: %s\n",instructionRegister);
   char opcode[5]; // Room for 4 characters and the null terminator
   strncpy(opcode, instructionRegister, 4);
   opcode[5] = '\0'; // Ensure null-termination
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
for (int i = 0; i <= 4; i++) {
    printf("instruction: %c\n",opcode[i]);
}
     if(flag=='R')
     {
        int j=27;
        for (int i = 4; i < 9; i++) {
               // printf("instruction: %c\n",instructionRegister[i]);
           registers[0][j]=instructionRegister[i];
           registers[1][j]=instructionRegister[i+5];
           registers[2][j]=instructionRegister[i+10];
           j++;
          
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
    //printf("%zu",sizeof(registers[0]));
  //  printf("Register 0: %s\n", registers[0][35]);
  
    for (int i = 0; i < 33; ++i) {
         registers[i][32] = '\0';
    }
    //  for (int i=0; i < 3; i++)
    // {
    //     printf("Register %i :%s\n",i,registers[i]);
    // }
    // for(int i=0; i<32; i++){
    // printf("Instruction: %c\n",instructionRegister[i]);
    // }
    int R1 = strtol(registers[0],NULL, 2);
    printf("The binary string \"%s\"   is: %d\n", registers[0], R1);
     int R2 = strtol(registers[1],NULL, 2);
    printf("The binary string \"%s\"  is: %d\n", registers[1], R2);
     int R3 = strtol(registers[2],NULL, 2);
    printf("The binary string \"%s\"  is: %d\n", registers[2], R3);
    break;
}



}



#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/memory.h"
#include "header/parse.h"
int registers[32];
int programCounter=0;
int stall=0;
int endStall=0;
int printad=3;
int memoryDataRegister;
int memoryAdressRegister;
int temporayRegister;//used to store register address preventing its writeback in execute
char instructionRegister[33];
char executeInstruction[33];//used to to prevent instructionRegister being overwritten in in fetch used in case only of jumping uncoditionaly
int numberofinstructions;
int cycle=0;
int opcodeInt;//stored opcode from Decode passed to execute
char flag;//Decide R/J/I format
int reg1;//index of dest register in almost all instructions check instructions same order
int reg2;//index of source register in almost all instructions check instructions same order
int reg3;//index of source  register in almost all instructions check instructions same order
int reg1Value;//value of reg1 if needed
int reg2Value;//value of reg2 if needed
int reg3Value;//value of reg3 if needed
int imm_value;//value of imm if needed
int shamt;//value of shamt if needed
int states[]={-1,-1,-1,-1,-1,-1,-1};//{F,D1,D2,E1,E2,MA,WB} used to know state of each stage if not -1 then it contains address of instruction wanted 
int decodecycle=1;//used for stall
int executeCycle=1;//used for stall
int result;//result of alu
int memoryAccessOn=0;//prevent unncessary execution of MA if not needed instruction is still considered in MA
int writeBackOn=0;//same as  above
int opcodetemp;//prevent overwritten of opcodeint
int dispatch=0;//flush in case of jump
int writeBacktemp;//value to be stored in when doing MOVR
int forwardedValue;//forward value in case of Register is repeated preventing using incorrect value of varriable when needed in decode
int forwardedRegister;//functions as above but as index
int oldPc;// used for printing purposes in case of jump

struct fetch_decode fd;
struct decode_execute de;
struct execute_memory em;
struct memory_writeback mw;
struct writeback_fetch wf;

struct fetch_decode{
    char readIR [33];
    char writeIR [33];
    
};
struct decode_execute{
    int writeOpcodeInt;
    int readOpcodeInt;
    int writeReg1;
    int readReg1;
    int writeReg2;
    int readReg2;
    int writeReg3;
    int readReg3;
    int writeReg1Value;
    int readReg1Value;
    int writeReg2Value;
    int readReg2Value;
    int writeReg3Value;
    int readReg3Value;
    int writeImm_value;
    int readImm_value;
    int writeShamt;
    int readShamt;
    char writeExecuteInstruction[33];
    char readExecuteInstruction[33];

};
struct execute_memory{
    int writeOpcodeTemp;
    int readOpcodeTemp;
    int readTemporayRegister;
    int writeTemporayRegister;
    int readOldPc;
    int writeOldPc;
    int readDispatch;
    int writeDispatch;
    int writeMemoryDataRegister;
    int readMemoryDataRegister;
    int writeMemoryAddressRegister;
    int readMemoryAdressRegister;
    int writeResult;
    int readResult;
};
struct memory_writeback{
    int writeOpcodeTemp;
    int readOpcodeTemp;
    int readTemporayRegister;
    int writeTemporayRegister;
    int readOldPc;
    int writeOldPc;
    int readDispatch;
    int writeDispatch;
    int writeBacktemp;
    int readWriteBacktemp;
    int writeResult;
    int readResult;
};
struct writeback_fetch{
    int oldpc;
    int dispatch;
    int stall;
    int readOldPc;
    int writeOldPc;
    int readDispatch;
    int writeDispatch;
};

// 3mlt fetch, b3daha momken fetch t5las abl decode fa hat5od men new IR mesh old

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
void copyFetchDecode(){

    strcpy(fd.readIR,fd.writeIR);

    
}
void copyDecodeExecute(){

    de.readOpcodeInt = de.writeOpcodeInt;
    de.readReg1 = de.writeReg1;
    de.readReg2 = de.writeReg2;
    de.readReg3 = de.writeReg3;
    de.readReg1Value = de.writeReg1Value;
    de.readReg2Value = de.writeReg2Value;
    de.readReg3Value = de.writeReg3Value;
    de.readImm_value = de.writeImm_value;
    
    de.readShamt = de.writeShamt;
    strcpy(de.readExecuteInstruction , de.writeExecuteInstruction);

    
    
}
void copyExecuteMemory(){

    em.readDispatch = em.writeDispatch;
    em.readMemoryAdressRegister = em.writeMemoryAddressRegister;
    em.readMemoryDataRegister = em.writeMemoryDataRegister;
    em.readOpcodeTemp = em.writeOpcodeTemp;
    em.readTemporayRegister = em.writeTemporayRegister;
    em.readOldPc = em.writeOldPc;
    em.readResult = em.writeResult;
    
    

}
void copyMemoryWriteback(){
    
        mw.readDispatch = mw.writeDispatch;
        mw.readOpcodeTemp = mw.writeOpcodeTemp;
        mw.readTemporayRegister = mw.writeTemporayRegister;
        mw.readOldPc = mw.writeOldPc;
        mw.readResult = mw.writeResult;
        mw.readWriteBacktemp = mw.writeBacktemp;


    
}
void copyWritebackFetch(){

    wf.readDispatch = wf.writeDispatch;
    wf.readOldPc = wf.writeOldPc;
    
    
}


    void* fetch(){
        
        printf("entered Fetch\n");
        //in case of dispatch then just fetch old instruction as important remark
        if(!stall){
            if(dispatch){
                printf("entered dispatch\n");
                strcpy(instructionRegister,memory[wf.readOldPc]);
                states[0]=wf.readOldPc;
                strcpy(fd.writeIR,instructionRegister);
                return NULL;
            }
            strcpy(instructionRegister,memory[programCounter]); 
            for(int i=0;i<2048;i++)
                if(memory[i][0]!='\0')
                    printf("%d\n",i);
            states[0]=programCounter;
        }
        strcpy(fd.writeIR,instructionRegister);
    }

    void* decode(){
        printf("entered Decode\n");
        //stall
        if(!stall){
            if(decodecycle==1){
                decodecycle=0;
            }
            else {
                char opcode[5]="0000";
                strncpy(opcode, fd.readIR, 4);
                //opcode[5] = '\0'; // Ensure null-termination
                opcodeInt = strtol(opcode,NULL, 2);
                de.writeOpcodeInt = opcodeInt;
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
                printf("flag is: %c\n",flag);
                if(flag=='R')
                {  
                char temp1[7]="00000";
                char temp2[7]="00000";
                char temp3[7]="00000";
                char temp4[15]="0000000000000";
                
                strncpy(temp1,fd.readIR+4,5);
                strncpy(temp2,fd.readIR+9,5);
                strncpy(temp3,fd.readIR+14,5);
                strncpy(temp4,fd.readIR+19,13);
                
                

                char temp11[(sizeof(temp1)/sizeof(char)) + 1]; // One extra space for the null terminator
                sprintf(temp11, "0%s", temp1);
                strcpy(temp1, temp11);
                reg1=binaryToDecimal(temp1);
                de.writeReg1 = reg1;

                char temp12[(sizeof(temp2)/sizeof(char)) + 1]; // One extra space for the null terminator
                sprintf(temp12, "0%s", temp2);
                strcpy(temp2, temp12);
                reg2=binaryToDecimal(temp2);
                de.writeReg2 = reg2;

                char temp13[(sizeof(temp3)/sizeof(char)) + 1]; // One extra space for the null terminator
                sprintf(temp13, "0%s", temp3);
                strcpy(temp3, temp13);
                reg3=binaryToDecimal(temp3);
                de.writeReg3 = reg3;

                char temp14[(sizeof(temp4)/sizeof(char)) + 1]; // One extra space for the null terminator
                sprintf(temp14, "0%s", temp4);
                strcpy(temp4, temp14);
                
                
                shamt=binaryToDecimal(temp4);
                de.writeShamt = shamt;
                reg2Value=registers[reg2];
                de.writeReg2Value = reg2Value;
                reg3Value=registers[reg3];
                de.writeReg3Value = reg3Value;

                }

                if(flag=='I')
                {
                char temp1[7]="00000";
                char temp2[7]="00000";
            char imm[19]="000000000000000000";
            
                strncpy(temp1,fd.readIR+4,5);
                char temp[(sizeof(temp1)/sizeof(char)) + 1]; // One extra space for the null terminator
                sprintf(temp, "0%s", temp1);
                strcpy(temp1, temp);

                strncpy(temp2,fd.readIR+9,5);
                char temp100[(sizeof(temp2)/sizeof(char)) + 1]; // One extra space for the null terminator
                sprintf(temp100, "0%s", temp2);
                strcpy(temp2, temp100);

                strncpy(imm,fd.readIR+14,18);
                
                reg1=binaryToDecimal(temp1);
                de.writeReg1 = reg1;
                reg2=binaryToDecimal(temp2);
                de.writeReg2 = reg2;
                printf("reg1: %d\n",reg1);
                printf("reg2: %d\n",reg2);

                printf("imm: %s\n",imm);
                imm_value=binaryToDecimal(imm);
                de.writeImm_value = imm_value;
                reg1Value=registers[reg1];
                de.writeReg1Value = reg1Value;
                reg2Value=registers[reg2];
                de.writeReg2Value = reg2Value;
                
                }
                decodecycle=1;
                //forward correct values if needed to avoid hazards
                strcpy(executeInstruction,fd.readIR);    
                strcpy(de.writeExecuteInstruction , executeInstruction);
                }
        }
        de.writeOpcodeInt = opcodeInt;
        de.writeShamt = shamt;
        de.writeReg1 = reg1;
        de.writeReg2 = reg2;
        de.writeReg3 = reg3;
        de.writeReg1Value = reg1Value;
        de.writeReg2Value = reg2Value;
        de.writeReg3Value = reg3Value;
        de.writeImm_value = imm_value;
        strcpy(de.writeExecuteInstruction , executeInstruction);
    }


void* execute(){
    printf("entered Execute\n");
    //stall
    if(executeCycle==1)
        executeCycle=0;
    else {if (writeBackOn || memoryAccessOn)
            {
                if(forwardedRegister==de.readReg1)
                {  
                    de.readReg1Value=forwardedValue;
                    printf("entered forwarding reg1:%d\n",forwardedValue);
                }
                if(forwardedRegister==de.readReg2 )
                {printf("entered forwarding reg2:%d\n",forwardedValue);
                    de.readReg2Value=forwardedValue;
                }
                if(forwardedRegister==de.readReg3)
                {  
                    de.readReg3Value=forwardedValue;
                }
                 writeBackOn=0;
            }
        if(de.readOpcodeInt==0){  //exec of add operation
     
            printf("operand 1 = %d\n",de.readReg2Value);
            printf("operand 2 = %d\n",de.readReg3Value);
            printf("destination register = %d\n",de.readReg1);
            result=de.readReg2Value+de.readReg3Value;
            em.writeResult = result;
            temporayRegister=de.readReg1;
            em.writeTemporayRegister = temporayRegister;
            printf("entered exec with dest:%d\n",temporayRegister);
            writeBackOn=1;//correct??
           

        }

        if(de.readOpcodeInt==1){  //exec of sub operation
            // int v1=registers[reg2];
            // int v2=registers[reg3];//why immediate not r3
            printf("operand 1 = %d\n",registers[de.readReg2]);
            printf("operand 2 = %d\n",registers[de.readReg3]);
            printf("destination register = %d\n",de.readReg1);
            result=de.readReg2Value-de.readReg3Value;
            em.writeResult = result;
            temporayRegister=de.readReg1;
            em.writeTemporayRegister = temporayRegister;
            writeBackOn=1;
            

        }

        if(de.readOpcodeInt==2){  //exec of mul operation
            // int v1=registers[reg2];
            // int v2=registers[reg3];
            printf("operand 1 = %d\n",registers[de.readReg2]);
            printf("operand 2 = %d\n",registers[de.readReg3]);
            printf("destination register = %d\n",de.readReg1);
            result=de.readReg2Value*de.readReg3Value;
            em.writeResult = result;
            temporayRegister=de.readReg1;
            em.writeTemporayRegister = temporayRegister;
            writeBackOn=1;

        }

        if(de.readOpcodeInt==3){  //exec of MOVI operation
            result=de.readImm_value;
            em.writeResult = result;
            temporayRegister=de.readReg1;
            em.writeTemporayRegister = temporayRegister;
            writeBackOn=1;
            
            
            printf("Immediate value = %d\n",de.readImm_value);
            printf("destination register = %d\n",de.readReg1);

        }

        if(de.readOpcodeInt==4){  //exec of JEQ operation
            // int v1=registers[reg1];
            // int v2=registers[reg2];
            printf("operand 1 JEQ = %d\n",de.readReg1Value);
            printf("operand 2 JEQ = %d\n",de.readReg2Value);
            printf("Immediate value = %d\n",de.readImm_value);
            if(de.readReg1Value==de.readReg2Value){
                printf("entered change PC\n");
                oldPc=programCounter;
                em.writeOldPc = oldPc;
                printf("pc: %d",programCounter);
                 programCounter=programCounter+de.readImm_value-1;
                printf("pc: %d",programCounter);
                  //should this be in write back ?
                  dispatch=1;
                  em.writeDispatch = dispatch;
                  wf.writeDispatch = dispatch;
                  wf.readDispatch = dispatch;
                  wf.readOldPc = oldPc;
                wf.writeOldPc = oldPc;
            }
           


        }

        if(de.readOpcodeInt==5){  //exec of AND operation
            int v1=registers[de.readReg2];
            int v2=registers[de.readReg3];
            result=de.readReg2Value&de.readReg3Value;
            em.writeResult = result;
            temporayRegister=de.readReg1; 
            em.writeTemporayRegister = temporayRegister;
            writeBackOn=1;
            printf("operand 1 = %d\n",registers[de.readReg2]);
            printf("operand 2 = %d\n",registers[de.readReg3]);
            printf("destination register = %d\n",de.readReg1);
          
        }
        

        if(de.readOpcodeInt==6){  //exec of XORI operation
            int v1=registers[de.readReg2];
            int v2=de.readImm_value;
            result=de.readReg2Value^de.readImm_value;
            em.writeResult = result;
            temporayRegister=de.readReg1; 
            em.writeTemporayRegister = temporayRegister;
            printf("operand 1 = %d\n",registers[de.readReg2]);
            printf("operand 2 = %d\n",de.readImm_value);
            printf("destination register = %d\n",de.readReg1);
            writeBackOn=1;
           
        }

        if(de.readOpcodeInt==7){   //exec of JMP operation
            char newpc[]="00000000000000000000000000000000";
            //print stuff
            oldPc=programCounter;
            em.writeOldPc = oldPc;
            intToBinary(programCounter-1,newpc,33);
           // printf("newpc: %s\n",newpc);
            strncpy(newpc+4,de.readExecuteInstruction+4,28);
            // printf("%s \n",executeInstruction); 
           // printf("newpc: %s\n",newpc);          
           
            char temppc[(sizeof(newpc)/sizeof(char)) + 1]; // One extra space for the null terminator
            sprintf(temppc, "0%s", newpc);
            strcpy(newpc, temppc);
            programCounter=binaryToDecimal(newpc);
            printf("address to jump to= %d\n",programCounter);  
            printf("programCounter: %d\n",programCounter);
             // should be in write back stage ?
            dispatch=1;  
            em.writeDispatch = dispatch;
            wf.writeDispatch = dispatch;
                  wf.readDispatch = dispatch;
                  wf.readOldPc = oldPc;
                wf.writeOldPc = oldPc;
                  
                
                 
            
         }
        if(de.readOpcodeInt==8){   //exec of LSL operation
           result=de.readReg2Value<<de.readShamt;
           em.writeResult = result;
           temporayRegister=de.readReg1;  
            em.writeTemporayRegister = temporayRegister;
           writeBackOn=1;
           printf("operand 1 = %d\n",registers[de.readReg2]);
            printf("shift amount = %d\n",de.readShamt);
            printf("destination register = %d\n",de.readReg1);
           
            
        }
        if(de.readOpcodeInt==9){   //exec of LSR operation
            result=de.readReg2Value>>de.readShamt;
            em.writeResult = result;
            temporayRegister=de.readReg1;
            em.writeTemporayRegister = temporayRegister;
            writeBackOn=1;
            printf("operand 1 = %d\n",registers[de.readReg2]);
            printf("shift amount = %d\n",de.readShamt);
            printf("destination register = %d",de.readReg1);
            
        }

         if(de.readOpcodeInt==10){  //exec of MOVR operation
            // int v1=registers[reg2];
            // int v2=imm_value;
            memoryAdressRegister=de.readReg2Value+de.readImm_value;
            em.writeMemoryAddressRegister = memoryAdressRegister;
            temporayRegister=de.readReg1;  
            em.writeTemporayRegister = temporayRegister;
            memoryAccessOn=1;
            writeBackOn=1;
            // printf("operand 1 = %d\n",registers[reg2]);
            // printf("immediate value = %d\n",imm_value);
            printf("memoryAddressRegister = %d\n",memoryAdressRegister);
            printf("destination register = %d",de.readReg1);


        }
         if(de.readOpcodeInt==11){  //exec of MOVM operation
            memoryDataRegister=registers[de.readReg1];
            em.writeMemoryDataRegister = memoryDataRegister;
            // int v2=registers[reg2];
            // int v3=imm_value;
            memoryAdressRegister=de.readReg2Value+de.readImm_value;
            em.writeMemoryAddressRegister = memoryAdressRegister;
            memoryAccessOn=1;
            // writeBackOn=1;
            printf("operand 1 = %d\n",registers[de.readReg1]);
            printf("memoryAddressRegister = %d\n",memoryAdressRegister);
            
        }

        // printf("result: %d\n",reg1);
        opcodetemp=de.readOpcodeInt;
        em.writeOpcodeTemp = opcodetemp;
        executeCycle=1;
        if(writeBackOn){
        forwardedValue = result;
        forwardedRegister = temporayRegister;
        if(de.readOpcodeInt==11 ||  memoryAccessOn)
        {
            forwardedValue=memoryDataRegister;
        
           // forwardedValue = binaryToDecimal(memory[memoryAdressRegister]);
            // forwardedRegister = temporayRegister;
        }
        
    }
            }
            em.writeOpcodeTemp = opcodetemp;
            em.writeTemporayRegister = temporayRegister;
            em.writeOldPc = oldPc;
            em.writeDispatch = dispatch;
            em.writeMemoryDataRegister = memoryDataRegister;
            em.writeMemoryAddressRegister = memoryAdressRegister;
            em.writeResult = result;
            
}
 
void* writeBack(){
            printf("entered WriteBack\n");
            
            if(states[5]==states[6]){
            if(mw.readTemporayRegister==0){
                return NULL;
            }

            else{//MoVR
                if(opcodetemp==10){
                    registers[mw.readTemporayRegister]=mw.readWriteBacktemp;
                }//MOVM
                else{
                    registers[mw.readTemporayRegister]=mw.readResult;
                }
           
            printf("Register %d value changed to %d \n",mw.readTemporayRegister,registers[mw.readTemporayRegister]);
            
        }}
        
        mw.writeDispatch = em.readDispatch;
        }

void* memoryAccess(){
            printf("entered Memory Access\n");
//DECIDE UPON UPCODE TO READ OR WRITE
                if(em.readOpcodeTemp==11){
                    // printf("reg:%d, value:%d\n",temporayRegister,registers[temporayRegister]);
                    intToBinary(em.readMemoryDataRegister,memory[em.readMemoryAdressRegister],33);
                    printf("Value of Location %d in memory changed to %d \n",em.readMemoryAdressRegister,registers[em.readTemporayRegister]);
                    memoryAccessOn=0;
                }
                
                 else if(em.readOpcodeTemp==10){
                    char mem [sizeof(memory[em.readMemoryAdressRegister])/sizeof(char) +1];
                    strcpy(mem,memory[em.readMemoryAdressRegister]);
                    endStall=1;
                    char tempmem[(sizeof(mem)/sizeof(char)) + 1]; // One extra space for the null terminator
                    sprintf(tempmem, "0%s", mem);
                    strcpy(mem, tempmem);

                    writeBacktemp=binaryToDecimal(mem);
                    memoryAccessOn=0;
        }
        mw.writeBacktemp = writeBacktemp;
        mw.writeTemporayRegister = em.readTemporayRegister;
        mw.writeOldPc = em.readOldPc;
        mw.writeDispatch = em.readDispatch;
        mw.writeResult = em.readResult;
        
        }

void execProgram(){
    pthread_t fetch_thread, decode_thread, execute_thread, memoryAccess_thread, writeBack_thread;
    int flagFetchThread = 0;
    parse();
    for(int i = 0; i < 2048; i++) 
        if(memory[i][0] != '\0')
            numberofinstructions++;

    while(1) {
        printf("Print %s\n",fd.readIR);
        printf("Print %s\n",de.readExecuteInstruction);
        printf("Print is 1: %d\n",de.readImm_value);
        printf("Print is 2: %d\n",de.readOpcodeInt);
        printf("Print is 3: %d\n",de.readReg1);
        printf("Print is 4: %d\n",de.readReg2);
        printf("Print is 5: %d\n",de.readReg3);
        printf("Print is 6: %d\n",de.readReg1Value);
        printf("Print is 7: %d\n",de.readReg2Value);
        printf("Print is 8: %d\n",de.readReg3Value);
        printf("Print is 9: %d\n",de.readShamt);
        printf("Print is 10: %d\n",em.readMemoryAdressRegister);
        printf("Print is 11: %d\n",em.readMemoryDataRegister);
        printf("Print is 12: %d\n",em.readOldPc);
        printf("Print is 13: %d\n",em.readResult);
        printf("Print is 14: %d\n",em.readTemporayRegister);
        printf("Print is 15: %d\n",mw.readDispatch);
        printf("Print is 16: %d\n",mw.readOpcodeTemp);
        printf("Print is 17: %d\n",mw.readWriteBacktemp);
        printf("Print is 18: %d\n",mw.readOldPc);
        printf("Print is 19: %d\n",mw.readTemporayRegister);
        printf("Print is 20: %d\n",mw.readResult);
        printf("Print is 21: %d\n",wf.readDispatch);
        printf("Print is 22: %d\n",wf.readOldPc);



        printf("\nClock cycle number %d\n",cycle+1);
        if(states[6]!=-1){
            if(writeBackOn==1){
                pthread_create(&writeBack_thread, NULL, &writeBack, NULL);
                copyWritebackFetch();
            }
        }
            
        if(states[5]!=-1){
            pthread_create(&memoryAccess_thread, NULL, &memoryAccess, NULL);
            copyMemoryWriteback();
        }

        if(states[3]!=-1){
            pthread_create(&execute_thread, NULL, &execute, NULL);
            copyExecuteMemory();
        }

        if(states[2]!=-1 || states[1]!=-1) {
            pthread_create(&decode_thread, NULL, &decode, NULL);
            if(states[2]!=-1)
                copyDecodeExecute();
        } 

        
        
        if(cycle%2==0 && numberofinstructions>0){
            if (programCounter==numberofinstructions) {
                states[0]=-1;
            } else {
                // when dubugging and come here, then continue running, betezbot
                flagFetchThread = 1;
                pthread_create(&fetch_thread, NULL, &fetch, NULL);
                copyFetchDecode();
                // if(!stall)
                //     programCounter++;
            }
        }
        
        if(states[6]!=-1 && writeBackOn==1){
            pthread_join(writeBack_thread, NULL);
            copyWritebackFetch();
        }
        if(states[5]!=-1){
            pthread_join(memoryAccess_thread, NULL);
            copyMemoryWriteback();
        }
        if(states[3]!=-1){
            pthread_join(execute_thread, NULL);
            copyExecuteMemory();
        }
        if(states[2]!=-1 || states[1]!=-1 ){
            pthread_join(decode_thread, NULL);
            if(states[2]!=-1)
                copyDecodeExecute();
         
        }
        if(cycle%2==0 && numberofinstructions>0 && flagFetchThread) {
            pthread_join(fetch_thread, NULL);
            copyFetchDecode();
            flagFetchThread = 0;
            if(!stall)
                programCounter++;
        }
            


         cycle++;
         
     if(states[0]!=-1 &&states[0]!=states[1] )
            // printf("Instruction fetched : %s\n",instructionRegister);
            {printf("Instruction fetched : %s\n",instructions[states[0]]);
          
           }

        if(states[1]!=-1 )
            printf("Instruction decoding : %s\n",instructions[states[1]]);
        
        if(states[3]!=-1)
           printf("Instruction executing : %s\n",instructions[states[3]]);
        if(states[4]!=-1 && states[4]!=states[5] && states[3]==-1)
           printf("Instruction executing : %s\n",instructions[states[4]]);

        
        if(states[5]!=-1 && states[5]!=states[6])
           printf("Instruction in Memory Access : %s\n",instructions[states[5]]);

        if(states[6]!=-1 && states[5]==states[6] )
             printf("Instruction in WriteBack stage : %s\n",instructions[states[6]]);
       

       for(int i = 6; i > 0; i--) {
            printf("states[%d] = %d\n", i, states[i]);
        }
        printf("states[%d] = %d\n", 0, states[0]);
  
         //shift stages
         if(!stall){
            for(int i = 6; i > 0; i--) 
            states[i] = states[i-1];
        }
        else{
            for(int i=6;i>3;i--)
            states[i]=states[i-1];
            states[printad]=-1;
            printad++;
            if (endStall){
                stall=0;
                endStall=0;
                printad=3;
            }
            
        }

        // states[6]=-1;
         if(dispatch)
            {   dispatch = 0;
                //flush
                mw.writeDispatch=0;
                states[0]=-1;
                states[1]=-1;
                states[2]=-1;
                states[3]=-1;
                programCounter--;
                }
         if( states[0]==-1 && states[1]==-1 && states[2]==-1 && states[3]==-1 && states[4]==-1 && states[5]==-1 ){
            printf("Breaked\n");
            break;
        }
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
    for(int i = 1023 ; i < 2048 ; i++) {
        // If the first character of the current string is not '\0', print it
             if(memory[i][0] != '\0'){
            printf("%d : %s, %s\n",i, instructions[i],memory[i]);}
            
        
    }
    return 0;
}
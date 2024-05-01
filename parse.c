#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *file;
char tmp[33];
char result[33];

void regToBin(char str[]){ 
    tmp[0] = '\0';
    int num = atoi(str + 1);  
    char bin[6];  

    for (int i = 4; i >= 0; --i, num >>= 1) {
        bin[i] = (num & 1) + '0';
    }
    bin[5] = '\0';  // Null-terminate the string

    strcat(tmp,bin);
}
char* numToBin18(char str[]) {
    static char bin[19];  // Binary representation of the number (18 bits plus null character)
    int num = atoi(str);  // Convert the string to an integer

    for (int i = 17; i >= 0; --i, num >>= 1) {
        bin[i] = (num & 1) + '0';
    }
    bin[18] = '\0';  // Null-terminate the string

    return bin;
}

char* numToBin28(char str[]) {
    static char bin[29];  // Binary representation of the number (18 bits plus null character)
    int num = atoi(str);  // Convert the string to an integer

    for (int i = 27; i >= 0; --i, num >>= 1) {
        bin[i] = (num & 1) + '0';
    }
    bin[28] = '\0';  // Null-terminate the string

    return bin;
}

char* numToBin13(char str[]) {
    static char bin[14];  // Binary representation of the number (18 bits plus null character)
    int num = atoi(str);  // Convert the string to an integer

    for (int i = 12; i >= 0; --i, num >>= 1) {
        bin[i] = (num & 1) + '0';
    }
    bin[13] = '\0';  // Null-terminate the string

    return bin;
}

void convert(char chars[][33]) {
    char opcode[33] = "";
    char R1[33] = "";
    char R2[33] = "";
    char R3[33] = "";
    char shamt[33] = "";
    char imm[33] = "";
    char address[33] = "";
    // char result[33] = "";
    
    if (strcmp(chars[0], "ADD") == 0) {
        strcat(opcode, "0000");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        regToBin(chars[3]);
        strcat(R3, tmp);
        
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,strcat(R3,"0000000000000")))));
    } 
        
    
    else if (strcmp(chars[0], "SUB") == 0) {
        strcat(opcode, "0001");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        regToBin(chars[3]);
        strcat(R3, tmp);
        
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,strcat(R3,"0000000000000")))));
    } else if (strcmp(chars[0], "MUL") == 0) {
        strcat(opcode, "0010");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        regToBin(chars[3]);
        strcat(R3, tmp);
        
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,strcat(R3,"0000000000000")))));
    } 
    else if (strcmp(chars[0], "MOVI") == 0) {
        strcat(opcode, "0011");
        regToBin(chars[1]);
        strcat(R1, tmp);
        strcat(R2, "00000");
        strcat(imm, numToBin18(chars[2]));
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,imm))));
    } else if (strcmp(chars[0], "JEQ") == 0) {
        strcat(opcode, "0100");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        strcat(imm, numToBin18(chars[3]));
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,imm))));
    } 
    else if (strcmp(chars[0], "AND") == 0) {
        strcat(opcode, "0100");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        regToBin(chars[3]);
        strcat(R3, tmp);
        
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,strcat(R3,"0000000000000")))));
    } 
    else if (strcmp(chars[0], "XORI") == 0) {
        strcat(opcode, "0110");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        strcat(imm, numToBin18(chars[3]));
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,imm))));
    }
     else if (strcmp(chars[0], "JMP") == 0) {
        strcat(opcode, "0111");
        strcat(address, numToBin28(chars[1]));
        strcat(result, strcat(opcode,address));
    } 
    else if (strcmp(chars[0], "LSL") == 0) {
        strcat(opcode, "1000");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        strcat(shamt, numToBin13(chars[3]));
        char tmp1[33] = "00000";
        strcat(tmp1, shamt);
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,tmp1))));
    } 
    else if (strcmp(chars[0], "LSR") == 0) {
        strcat(opcode, "1001");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        strcat(shamt, numToBin13(chars[3]));
        char tmp1[33] = "00000";
        strcat(tmp1, shamt);
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,tmp1))));
    } 
    else if (strcmp(chars[0], "MOVR") == 0) {
        strcat(opcode, "1010");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        strcat(imm, numToBin18(chars[3]));
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,imm))));
    } else if (strcmp(chars[0], "MOVM") == 0) {
        strcat(opcode, "1011");
        regToBin(chars[1]);
        strcat(R1, tmp);
        regToBin(chars[2]);
        strcat(R2, tmp);
        strcat(imm, numToBin18(chars[3]));
        strcat(result, strcat(opcode,strcat(R1,strcat(R2,imm))));
    } else {
        // Handle other cases
    }
    
}

void parse() {
    char line[256];

    file = fopen("Assembly.txt", "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }

    FILE *outputFile = fopen("Output.txt", "w");
    if(outputFile == NULL) {
        printf("Could not open file Output.txt\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        
        int c = 0;
        for(int i = 0; i < strlen(line); i++) {
            if(line[i] == ' ') {
              c++; 
            } 
        }  // Null-terminate the string

        char chars[c + 1][33];
        
        for(int i = 0; i < c + 1; i++) {
            chars[i][0] = '\0';
        }

        int c2 = 0;
        char instr[5] = "";
        for(int i = 0; i < strlen(line); i++) {
            if(line[i] == ' ') {
                strcat(chars[c2], instr);
                c2++;
                instr[0] = '\0';
            }else{
                char tmp[2] = {line[i], '\0'};  // Create a temporary string
                strcat(instr, tmp);
            }
        }
        
        chars[c2][0] = '\0';
        
        strcat(chars[c2],instr);
        
        
        convert(chars);

        printf("%s\n", result);

        fprintf(outputFile, "%s\n", result);

        result[0] = '\0';
    }

    fclose(file);
    fclose(outputFile);
}


int main() {

    parse();

    
    return 0;
}
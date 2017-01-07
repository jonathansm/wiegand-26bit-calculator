
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_BITS 26
#define MAX_FACILITY_BITS 8
#define MAX_UNIQUE_BITS 16
#define OUTPUT_SPACE 40
#define PARITY_BITS_TO_TEST 12
#define PROXMARK_FRONT_BITS 32

int binaryToDec(char *binary, int arrayCount, int bitsToSkip);
int binaryToHex (char *binary);
char* hexToBinary (char *hex);
char* decToBinary(int dec, int bits);
char calcParityBit(char * binary, bool parityTest);
int makeProxmarkTagID (char* binary);
void printHelp();


int main(int argc, char **argv)
{
    
    int facilityFlag = 0;
    int uniqueFlag = 0;
    int binaryFlag = 0;
    int hexFlag = 0;
    int proxkmarkFlag = 0;
    
    char *facilityCode;
    char *uniqueCode;
    char *binaryCode;
    char *hexCode;
    char *proxkmarkCode;
    
    int facilityCodeInt = 0;
    int uniqueCodeInt = 0;
    int hexCodeInt = 0;
    int proxkmarkCodeInt = 0;
    
    extern char *optarg;
    extern int optind;
    int c;
    int err = 0;
    
    while ((c = getopt(argc, argv, "f:u:b:x:p:h")) != -1)
        switch (c) {
            case 'f':
                facilityFlag = 1;
                facilityCode = optarg;
                break;
            case 'u':
                uniqueFlag = 1;
                uniqueCode = optarg;
                break;
            case 'b':
                binaryFlag = 1;
                binaryCode = optarg;
                break;
            case 'x':
                hexFlag = 1;
                hexCode = optarg;
                break;
            case 'p':
                proxkmarkFlag = 1;
                proxkmarkCode = optarg;
                break;
            case 'h':
                printHelp();
                exit(1);
                break;
            case '?':
                err = 1;
                break;
        }
    if (argc <= 1) // Need atleast one option
    {
        printHelp();
        exit(1);
    }
    else if (err)
        exit(1);


    
    if ((facilityFlag == 1 && uniqueFlag == 0) || (facilityFlag == 0 && uniqueFlag == 1))
    {
        printf("\nYou need both the Facility Code and Unique Code\n\n");
        exit(1);
    }
    else if (facilityFlag == 1 && uniqueFlag == 1)
    {
        char *facilityCodeBin = decToBinary(atoi(facilityCode), MAX_FACILITY_BITS);
        char *uniqueCodeBin = decToBinary(atoi(uniqueCode), MAX_UNIQUE_BITS);
        
        char *combinedCodeBin = malloc(strlen(facilityCodeBin)+strlen(uniqueCodeBin)+1);
        strcpy(combinedCodeBin, facilityCodeBin);
        strcat(combinedCodeBin, uniqueCodeBin);
        
        char firstBit = calcParityBit(combinedCodeBin, true);
        char lastBit = calcParityBit(combinedCodeBin, false);
        
        for (int i=0; combinedCodeBin[i] != '\0'; i++)
        {
            binaryCode[i+1] = combinedCodeBin[i];
        }
        
        binaryCode[0] = firstBit;
        binaryCode[MAX_BITS-1] = lastBit;
        binaryCode[MAX_BITS] = '\0';
        
    }
    else if (proxkmarkFlag == 1)
    {
        for (int i = 0; i < 2; i++)
            proxkmarkCode++;
        
        if (proxkmarkCode[0] == '0')
            proxkmarkCode++;
        
        char *cleanedProxmarkBinary = hexToBinary(proxkmarkCode);
        
        
        for (int i = 0; i < 2; i++)
            cleanedProxmarkBinary++;
        
        strncpy(binaryCode, cleanedProxmarkBinary, MAX_BITS);
        binaryCode[MAX_BITS] = '\0';
        
    }
    else if (hexFlag == 1)
    {
        binaryCode = hexToBinary(hexCode);
    }
    
    
    if (strlen(binaryCode) == MAX_BITS)
    {
        facilityCodeInt = binaryToDec(binaryCode, MAX_FACILITY_BITS, 1);
        uniqueCodeInt = binaryToDec(binaryCode, MAX_UNIQUE_BITS, 9);
        hexCodeInt = binaryToHex(binaryCode);
        proxkmarkCodeInt = makeProxmarkTagID(binaryCode);
    }
    else
    {
        printf("\nBinary input needs to be 26bits!!!\n\n");
        exit(1);
    }
    
    
    printf("\n-----------  Wiegland Card Information  -----------\n");
    printf("| %-20s %26d |\n", "Facility Code", facilityCodeInt);
    printf("| %-20s %26d |\n", "Unique Code", uniqueCodeInt );
    printf("| %-20s %15s %x%08x |\n", "Proxmark Code","", PROXMARK_FRONT_BITS, proxkmarkCodeInt );
    printf("| %-20s %18s %07X |\n", "Hex Value","",hexCodeInt );
    printf("| %-20s %26s |\n", "Binary Value", binaryCode);
    printf("---------------------------------------------------\n\n");
    
    exit(0);
}

int binaryToDec(char *binary, int arrayCount, int bitsToSkip)
{
    int p = 0;
    int dec = 0;
    
    for (int j = arrayCount - 1 + bitsToSkip; j >= 0 + bitsToSkip; j-- ) {
        dec +=  (binary[j] - 48) * (int)pow((double)2, p);
        p++;
    }
    
    return dec;
}

char* decToBinary(int dec, int bits)
{
    char* binaryNumber = malloc(sizeof(char*) * 3);
    
    for(int i = bits - 1; i >= 0; i--){
        if((dec & (1 << i)) != 0){
            strcat(binaryNumber, "1");
        }else{
            strcat(binaryNumber, "0");
        }
    }
    
    return  binaryNumber;
}

int binaryToHex (char *binary)
{
    int hex = 0;
    do {
        int b = *binary  == '1'?1:0;
        hex = (hex<<1)|b;
        binary++;
    } while (*binary);
    
    return hex;
    
}

char* hexToBinary (char *hex)
{
    
    char *hexDigitToBinary[16] = {"0000", "0001", "0010", "0011", "0100", "0101",
        "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
    char hexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char* binaryNumber = (char *) malloc(sizeof(char) * 3);
    
    for(int i = 0; hex[i] != '\0'; i++)  {
        for(int j = 0; j < 16; j++){
            if(toupper(hex[i]) == hexDigits[j]){
                if (hex[i] == '0' && i == 0)
                    strcat(binaryNumber, "00");
                else if (hex[i] == '1' && i == 0)
                    strcat(binaryNumber, "01");
                else if (hex[i] == '2' && i == 0)
                    strcat(binaryNumber, "10");
                else
                    strcat(binaryNumber, hexDigitToBinary[j]);
            }
        }
    }
    
    return binaryNumber;
    
}

// Even = True
// Odd = False

char calcParityBit(char * binary, bool parityTest)
{
    int parityCount = 0;
    int parityTestOffset = 0;
    char parityBit = '0';
    
    if (!parityTest)
        parityTestOffset = 12;
    
    for(int i = 0 + parityTestOffset; i < PARITY_BITS_TO_TEST + parityTestOffset; i++)
    {
        if (binary[i] == '1')
            parityCount++;
    }
    
    if ((parityCount % 2 && parityTest) || (parityCount % 2 == 0 && !parityTest))
    {
        parityBit = '1';
    }
    
    return parityBit;
    
}

/*
 From what I can tell all the Proxmark Tag ID is the HEX value of 20 then a 1 bit added to the 26bit binary code then the 27 bits in Hex
 This what I'm going with until I find something else. So far out of the dozens of cards I've tested it has always worked
 */

int makeProxmarkTagID (char* binary)
{
    char tagID[MAX_BITS+1];
    
    for (int i=0; binary[i] != '\0'; i++)
    {
        tagID[i+1] = binary[i];
    }
    
    tagID[0] = '1';
    
    
    return binaryToHex(tagID);
    
}

void printHelp()
{
    printf("\nInput any 26bit Wiegand form i.e Facility code, and unique code, Hex, binary or Proxmark TagID\n");
    printf("the output will be all corresponding formats. This can be helpfull when cloning HID cards when\n");
    printf("you only have the facility code and unique code. The Proxmark ID is a 'special' hex code generated\n");
    printf("in the client application\n");
    printf("Variables:\n");
    printf("-p Proxmark generated TagID\n");
    printf("-f Facility or site code of the HID tag. Must use with -u\n");
    printf("-u Unique or card code of the HID tag. Must use with -f\n");
    printf("-x Hex value of the 26bit HID tag\n");
    printf("-b Binary value of the 26bit HID tag\n");
    printf("Examples:\n");
    printf("./wiegland-converter -p 2005c7c200\n");
    printf("./wiegland-converter -f 227 -u 57600\n");
    printf("./wiegland-converter -x 1C7C200\n");
    printf("./wiegland-converter -b 01110001111100001000000000\n");
    printf("\nOutput Example:\n");
    printf("-----------  Wiegland Card Information  -----------\n");
    printf("| Facility Code                               227 |\n");
    printf("| Unique Code                               57600 |\n");
    printf("| Proxmark Code                        2005c7c200 |\n");
    printf("| Hex Value                               1C7C200 |\n");
    printf("| Binary Value         01110001111100001000000000 |\n");
    printf("---------------------------------------------------\n\n");
}



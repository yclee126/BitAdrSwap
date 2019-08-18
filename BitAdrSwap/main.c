/*
BitAdrSwap v1.2
by yclee126
Aug 08, 2019
https://www.github.com

It Swaps byte bits and address bits!
Type target bit order to start swapping.
Typing wrong order sets to original order.

Supports 32bit address.

Changelog
=========

v1.0
Initial complete version

v1.1
Added original order set function

v1.2
Fixed excess address bit number counting
Fixed excess address bit swapping (performance improved)

*/

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <stdbool.h>
#define VERSIONINFO "BitAdrSwap v1.2\n"
#define WEBSITEINFO "https://github.com/yclee126/BitAdrSwap\n"
#pragma warning(disable: 4996)

int main() {

	printf(VERSIONINFO);
	printf("===============\n");
	printf(WEBSITEINFO);
	printf("\n");
	printf("Type target bit order to swap.\n");
	printf("source.hex -> converted.hex\n");
	printf("Type anything weird to skip typing the original order.\n");

	FILE *sFile; //source file (binary mode)
	FILE *cFile; //converted file (binary mode)

	if ((sFile = fopen("source.hex", "rb")) == NULL) {
		printf("\nSource file read failed.\n");
		system("PAUSE");
		return 0;
	}
	if ((cFile = fopen("converted.hex", "wb")) == NULL) {
		printf("\nConverted file write failed.\n");
		printf("Check for disk space or read only constrain\n");
		fclose(sFile);
		system("PAUSE");
		return 0;
	}

	//check file size
	fseek(sFile, 0, SEEK_END);
	uint32_t const sFileSize = ftell(sFile);
	if (sFileSize == 0) {
		printf("\nSource file is empty\n");
		goto end;
	}
	uint8_t adrBits = 32;
	uint32_t const sFileMaxAdr = sFileSize - 1; //sFileSize - 1 because it's the last address number (address starts from 0)
	while ((sFileMaxAdr & (1 << (adrBits - 1))) < 1) {
		adrBits--;
	}

	uint8_t bitTable[8]; //input buffer
	uint8_t bitMap[8]; //actual bitshift map
	uint8_t adrTable[32];
	uint8_t adrMap[32];
	bool bitFound;
	bool adrFound;

	//bit order input
	printf("\nEnter target byte bit order\n");
	for (uint8_t i = 0; i < 8; i++) {
		printf("%d ", 8 - (i + 1));
	}
	printf("\n");
	for (uint8_t i = 0; i < 8; i++) {
		scanf("%hhd", &bitMap[8 - (i + 1)]);
	}
	while ((getchar()) != '\n');
	for (uint8_t i = 0; i < 8; i++) { //order check
		bitFound = false;
		for (uint8_t j = 0; j < 8; j++) {
			if (bitMap[j] == i) {
				bitFound = true;
				break;
			}
		}
		if (bitFound == false) {
			printf("The byte bit order is invalid\n");
			printf("Original order is set.\n");
			break;
		}
	}
	for (uint8_t i = 0; i < 8; i++) { //order to num
		uint8_t j = 0;
		if (bitFound) {
			while (bitMap[j] != i) {
				j++;
			}
		}
		else {
			j = i;
		}
		bitTable[i] = j;
	}

	//address order input
	printf("\nEnter target address bit order\n");
	for (uint8_t i = 0; i < adrBits; i++) {
		printf("%02d ", adrBits - (i + 1));
	}
	printf("\n");
	for (uint8_t i = 0; i < adrBits; i++) {
		scanf("%hhd", &adrMap[adrBits - (i + 1)]);
	}
	while ((getchar()) != '\n');
	for (uint8_t i = 0; i < adrBits; i++) { //order check
		adrFound = false;
		for (uint8_t j = 0; j < adrBits; j++) {
			if (adrMap[j] == i) {
				adrFound = true;
				break;
			}
		}
		if (adrFound == false) {
			printf("The address bit order is invalid\n");
			printf("Original order is set.\n");
			break;
		}
	}
	for (uint8_t i = 0; i < adrBits; i++) { //order to num
		uint8_t j = 0;
		if (adrFound) {
			while (adrMap[j] != i) {
				j++;
			}
		}
		else {
			j = i;
		}
		adrTable[i] = j;
	}
	if (!bitFound && !adrFound) {
		printf("\nNo change was made to all target bits\n");
		goto end;
	}

	//swapping start
	printf("\nProcessing...\n");
	fseek(sFile, 0, SEEK_SET);
	uint32_t sCur = 0;
	uint32_t cAdr;
	uint8_t sBuf;
	uint8_t cBuf;
	while (fread(&sBuf, 1, 1, sFile)) {
		sCur++;
		cBuf = 0;
		cAdr = 0;
		for (uint8_t i = 0; i < 8; i++) { //swap byte bits
			if (sBuf & (1 << i)) {
				cBuf |= (1 << bitTable[i]);
			}
		}
		for (uint8_t i = 0; i < adrBits; i++) { //swap address bits
			if ((sCur - 1) & (1 << i)) {
				cAdr |= (1 << adrTable[i]);
			}
		}
		fseek(cFile, cAdr, SEEK_SET);
		fwrite(&cBuf, 1, 1, cFile); //write to the new file
	}

	FILE *cResult; //report file
	if ((cResult = fopen("SwapReport.txt", "w")) == NULL) {
		printf("\nReport file write failed.\n");
		printf("Check for disk space or read only constrain\n");
		goto end;
	}

	fputs(VERSIONINFO, cResult);
	fputs("===============\n", cResult);
	fputs(WEBSITEINFO, cResult);
	fputs("\n", cResult);
	fputs("Before\n", cResult);
	fputs("After\n", cResult);
	fputs("\n", cResult);

	fputs("Byte bits :\n", cResult);
	for (uint8_t i = 0; i < 8; i++) {
		fprintf(cResult, "%d ", 8 - (i + 1));
	}
	fseek(cResult, -1, SEEK_CUR);
	fputs("\n", cResult);
	for (uint8_t i = 0; i < 8; i++) {
		fprintf(cResult, "%d ", bitFound ? bitMap[8 - (i + 1)] : 8 - (i + 1));
	}
	fseek(cResult, -1, SEEK_CUR);
	fputs("\n", cResult);
	fputs("\n", cResult);


	fputs("Address bits :\n", cResult);
	for (uint8_t i = 0; i < adrBits; i++) {
		fprintf(cResult, "%02d ", adrBits - (i + 1));
	}
	fseek(cResult, -1, SEEK_CUR);
	fputs("\n", cResult);
	for (uint8_t i = 0; i < adrBits; i++) {
		fprintf(cResult, "%02d ", adrFound ? adrMap[adrBits - (i + 1)] : adrBits - (i + 1));
	}
	fseek(cResult, -1, SEEK_CUR);
	fputs("\n", cResult);
	fputs("\n", cResult);

	fputs("Total size :\n", cResult);
	fprintf(cResult, "%ld", sFileSize);
	fputs(" bytes", cResult);
	fputs("\n", cResult);
	fseek(cFile, 0, SEEK_END);
	fprintf(cResult, "%ld", ftell(cFile));
	fputs(" bytes", cResult);

	fclose(cResult);

	printf("\nOperation complete!\n\n");

end:;
	fclose(sFile);
	fclose(cFile);

	//Halt program
	system("PAUSE");
	return 0;
}

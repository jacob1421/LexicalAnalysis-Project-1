/*
	File: Program01.c
	Project: Lexical Analysis
	Assignment: Programming Project #1
	Programmer: Jacob Bremiller
	Date: 02/22/2019
	University: McMurry University
	Course: COSC-4310 & Spring 2020
	Instructor: Mr.Brozovic
	Compiler: Visual Studio 2019
	Description: The program will take a input file with extension of (.c) and an output file with extension of (.txt). The .c file will be processed one char at a time to find the
	lexeme's. When the start of a lexeme is found it will be processed and returned to main via a struct. In addition, it will loop until it has either found a lexeme start or the end of file. Once the program
	has recieved the EOF of the file it will print of the statistics(TokenName: Number of Tokens Processed with TokenName and the Total number of lines processed.) Lastly, as text is output to the console it is also written
	to the output file specified when running the program.
*/

#define _CRT_SECURE_NO_WARNINGS
#define MAX_LEXEME_SIZE 129 // 128 + 1 for the null terminator
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

/* Token Types */
typedef enum {
	PPDIR = 0,
	IDENT = 1,
	KEYWD = 2,
	INTGR = 3,
	FLOAT = 4,
	STRNG = 5,
	CHAR = 6,
	CMMNT = 7,
	END_OF_FILE = 8
}LexTokenType;

/* ASCII characters mapped to their respective decimal values */
typedef enum {
	/* ASCII */
	NULL_SYM = 0,
	START_OF_HEADER = 1,
	START__OF_TEXT = 2,
	END_OF_TEXT = 3,
	END_OF_TRANSMISSION = 4,
	ENQUIRY = 5,
	ACKNOWLEDGE = 6,
	BELL = 7,
	BACKSPACE = 8,
	HORIZONTAL_TAB = 9,
	LINE_FEED = 10,
	VERTICAL_TAB = 11,
	FORM_FEED = 12,
	CARRIAGE_RETURN = 13,
	SHIFT_OUT = 14,
	SHIFT_IN = 15,
	DATA_LINK_ESCAPE = 16,
	DEVICE_CONTROL_ONE = 17,
	DEVICE_CONTROL_TWO = 18,
	DEVICE_CONTROL_THREE = 19,
	DEVICE_CONTROL_FOUR = 20,
	NEGATIVE_ACKNOWLEDGE = 21,
	SYNCHRONIZE = 22,
	END_OF_TRANSMISSION_BLOCK = 23,
	CANCEL = 24,
	END_OF_MEDIUM = 25,
	SUBSTITUTE = 26,
	ESCAPE = 27,
	FILE_SEPARATOR = 28,
	GROUP_SEPARATOR = 29,
	RECORD_SEPARATOR = 30,
	UNIT_SEPERATOR = 31,
	SPACE = 32,
	EXCLAMATION_MARK = 33,
	DOUBLE_QUOTE = 34,
	NUMBER_SIGN = 35,
	DOLLAR_SIGN = 36,
	PERCENT_SIGN = 37,
	AMPERSAND = 38,
	SINGLE_QUOTE = 39,
	LEFT_PARENTHESES = 40,
	RIGHT_PARENTHESES = 41,
	ASTERISK = 42,
	ADDITION = 43,
	COMMA = 44,
	SUBTRACT = 45,
	PERIOD = 46,
	FORWARD_SLASH = 47,
	DIGIT_ZERO = 48,
	DIGIT_ONE = 49,
	DIGIT_TWO = 50,
	DIGIT_THREE = 51,
	DIGIT_FOUR = 52,
	DIGIT_FIVE = 53,
	DIGIT_SIX = 54,
	DIGIT_SEVEN = 55,
	DIGIT_EIGHT = 56,
	DIGIT_NINE = 57,
	COLON = 58,
	SEMI_COLON = 59,
	LESS_THAN_SIGN = 60,
	EQUAL_SIGN = 61,
	GREATER_THAN_SIGN = 62,
	QUESTION_MARK = 63,
	AT_SIGN = 64,
	UPPER_LETTER_A = 65,
	UPPER_LETTER_B = 66,
	UPPER_LETTER_C = 67,
	UPPER_LETTER_D = 68,
	UPPER_LETTER_E = 69,
	UPPER_LETTER_F = 70,
	UPPER_LETTER_G = 71,
	UPPER_LETTER_H = 72,
	UPPER_LETTER_I = 73,
	UPPER_LETTER_J = 74,
	UPPER_LETTER_K = 75,
	UPPER_LETTER_L = 76,
	UPPER_LETTER_M = 77,
	UPPER_LETTER_N = 78,
	UPPER_LETTER_O = 79,
	UPPER_LETTER_P = 80,
	UPPER_LETTER_Q = 81,
	UPPER_LETTER_R = 82,
	UPPER_LETTER_S = 83,
	UPPER_LETTER_T = 84,
	UPPER_LETTER_U = 85,
	UPPER_LETTER_V = 86,
	UPPER_LETTER_W = 87,
	UPPER_LETTER_X = 88,
	UPPER_LETTER_Y = 89,
	UPPER_LETTER_Z = 90,
	LEFT_SQUARE_BRACKET = 91,
	BACKSLASH = 92,
	RIGHT_SQUARE_BRACKET = 93,
	CARET = 94,
	UNDERSCORE = 95,
	ACCENT = 96,
	LOWER_LETTER_A = 97,
	LOWER_LETTER_B = 98,
	LOWER_LETTER_C = 99,
	LOWER_LETTER_D = 100,
	LOWER_LETTER_E = 101,
	LOWER_LETTER_F = 102,
	LOWER_LETTER_G = 103,
	LOWER_LETTER_H = 104,
	LOWER_LETTER_I = 105,
	LOWER_LETTER_J = 106,
	LOWER_LETTER_K = 107,
	LOWER_LETTER_L = 108,
	LOWER_LETTER_M = 109,
	LOWER_LETTER_N = 110,
	LOWER_LETTER_O = 111,
	LOWER_LETTER_P = 112,
	LOWER_LETTER_Q = 113,
	LOWER_LETTER_R = 114,
	LOWER_LETTER_S = 115,
	LOWER_LETTER_T = 116,
	LOWER_LETTER_U = 117,
	LOWER_LETTER_V = 118,
	LOWER_LETTER_W = 119,
	LOWER_LETTER_X = 120,
	LOWER_LETTER_Y = 121,
	LOWER_LETTER_Z = 122,
	LEFT_CURLY_BRACKET = 123,
	VERTICAL_BAR = 124,
	RIGHT_CURLY_BRACKET = 125,
	TILDE = 126,
	DELETE = 127,

	/* Non ASCII */
	_EOF = 999
}CharToken;

/* TRUE OR FALSE */
typedef enum {
	TRUE = 1,
	FALSE = 0
}Boolean;

/* Lexeme Struct */
typedef struct {
	unsigned int filePosition[2];
	char lexemeString[MAX_LEXEME_SIZE];
	LexTokenType lexToken;
	Boolean tokenCompleted;
}Lexeme;

/* Prototypes */
void getNextChar(FILE* cFilePtr, CharToken* previousCharTokenPtr, CharToken* currentCharTokenPtr, unsigned int* filePositionPtr);
char* tokenIdentifierToString(const LexTokenType tokenName);
Lexeme * lexScanner(FILE* cFilePtr);
void printProcessedTokenData(FILE* txtFilePtr, const unsigned int * tokensProcessed, const unsigned int * linesProcessed);
void printAndWrite(FILE* txtFilePtr, Lexeme * lexemeDataPtr, unsigned int * tokensProcessed);
LexTokenType isKeyword_or_Ident(char* lookupString);
LexTokenType isFloat_or_Digit(char* lookupString);
Boolean isAlphabetic(const CharToken aCharToken);
Boolean isDigit(const CharToken aCharToken);

int main(int argc, char *argv[]) {
	FILE *cFilePtr, *txtFilePtr;
	Lexeme* lexemeDataPtr;
	unsigned int processedTokenCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned int linesProcessed = 0;
	int stopFlag = 0;

	/* File Error Checking and Handling */
	if (argc != 3) {
		printf("Please check your arguments. Expecting format: LexicalAnalysis <.c input file> <.txt output file>\n");
		return 1;
	}
	
	cFilePtr = fopen(argv[1], "r");
	txtFilePtr = fopen(argv[2], "w");

	if(!cFilePtr) {
		perror("Input File Error");
		printf("Filename: %s\n", argv[1]);
		return 1;
	}
	else if (!txtFilePtr) {
		perror("Output File Error");
		printf("Filename: %s\n", argv[2]);
		return 1;
	}

	while (!stopFlag) {
		//Get next lexeme
		lexemeDataPtr = lexScanner(cFilePtr);

		//Print and save the lexeme struct data.
		printAndWrite(txtFilePtr, lexemeDataPtr, processedTokenCounter);
		
		//Stop if we are at the EOF
		if (lexemeDataPtr->lexToken == END_OF_FILE) {
			stopFlag = 1;
			linesProcessed = lexemeDataPtr->filePosition[0];
		}

		//Free the malloc created in lexScanner.
		free(lexemeDataPtr);
	}
	//Print and write the processed token data.
	printProcessedTokenData(txtFilePtr, processedTokenCounter, &linesProcessed);

	//Close the input and output file streams.
	fclose(cFilePtr);
	fclose(txtFilePtr);
	return 0;
}
/*
	Name: lexScanner
	Returns: Lexeme *
	Parameters:
		In: FILE * cFilePtr
		In/Out: CharToken * previousCharToken, CharToken *currentCharToken, unsigned int * filePosition
		Out: Lexeme * lexemeDataPtr
	Description: Takes a file pointer and packages the next lexeme into a struct with the lexeme string, token, file position and a completed flag.
*/
Lexeme * lexScanner(FILE* cFilePtr) {
	/* Line Number, Column Number */
	static unsigned int filePosition[2] = {1, 0};
	static CharToken previousCharToken = NULL_SYM;
	static CharToken currentCharToken = NULL_SYM;
	static LexTokenType lastTokenProcessed;
	static Boolean lastTokenCompleted = TRUE;
	Boolean processingLexeme = FALSE;

	//Lexeme Data Struct
	Lexeme * lexemeDataPtr = (Lexeme *)malloc(sizeof(Lexeme));
	//Error checking malloc
	if (lexemeDataPtr == NULL) {
		perror("Error malloc failed");
		exit(EXIT_FAILURE);
	}
	memset(lexemeDataPtr, 0, sizeof(Lexeme));
	int numbCharsWritten = 0;

	while (!processingLexeme) {
		//Get next char and update position until we find the start of a lexeme

		getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
		
		if ((currentCharToken == NUMBER_SIGN && lastTokenCompleted)) {
			/* 
				PPDIR -
				First column has a NUMBER_SIGN
				Process until new line or Reached MAX_LEXEME_SIZE(128 Chars + 1 Terminator) - *Read remainder of lexeme
				Return the LexemeData to Main 
			*/
			if (filePosition[1] == 1) {
				//Set flag and lexeme data
				processingLexeme = TRUE;
				lexemeDataPtr->filePosition[0] = filePosition[0];
				lexemeDataPtr->filePosition[1] = filePosition[1];
				//Read the lexeme
				while ((numbCharsWritten < (MAX_LEXEME_SIZE - 1)) && currentCharToken != LINE_FEED) {
					lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
				lexemeDataPtr->lexemeString[numbCharsWritten] = NULL_SYM;

				//Throw away any chars over the MAX_LEXEME_SIZE(If there are any).
				while (currentCharToken != LINE_FEED) {
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
				lexemeDataPtr->lexToken = PPDIR;
				lexemeDataPtr->tokenCompleted = TRUE;
			}
		}
		else if ((isAlphabetic(currentCharToken) && lastTokenCompleted)) {
			/*	
				IDENT and KEYWD(Ignore possibility of underscore being first character)
				Starts with alphabetic character
				Process until we dont find a LETTER or UNDERSCORE or we hit a newline or Reached MAX_LEXEME_SIZE(128 Chars + 1 Terminator) - *Read remainder of lexeme
				Use helper function to iterate each keyword to determine if KEYWD or IDENT.
				Return the LexemeData to Main
			*/
			//Set flag and lexeme data
			processingLexeme = TRUE;
			lexemeDataPtr->filePosition[0] = filePosition[0];
			lexemeDataPtr->filePosition[1] = filePosition[1];
			//Read the lexeme
			while ((numbCharsWritten < (MAX_LEXEME_SIZE - 1)) && (isAlphabetic(currentCharToken) || (currentCharToken == UNDERSCORE) || isDigit(currentCharToken)) && (currentCharToken != LINE_FEED)) {
				lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
				getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
			}
			lexemeDataPtr->lexemeString[numbCharsWritten] = NULL_SYM;

			//Throw away any chars over the MAX_LEXEME_SIZE(If there are any).
			while ((isAlphabetic(currentCharToken) || (currentCharToken == UNDERSCORE)) && (currentCharToken != LINE_FEED)) {
				getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
			}
			//Lookup string to check if its a keyword
			lexemeDataPtr->lexToken = isKeyword_or_Ident(lexemeDataPtr->lexemeString);
			lexemeDataPtr->tokenCompleted = TRUE;
		}
		else if ((isDigit(currentCharToken) && lastTokenCompleted)) {
			/*
				INTGR and FLOAT(Ignore negative sign, type suffix and scientific notation)
				Starts with digit
				Process until we dont find a digit or Reached MAX_LEXEME_SIZE(128 Chars + 1 Terminator) - *Read remainder of lexeme
				Return the LexemeData to Main
			*/

			//Set flag and lexeme data
			processingLexeme = TRUE;
			lexemeDataPtr->filePosition[0] = filePosition[0];
			lexemeDataPtr->filePosition[1] = filePosition[1];
			//Read the lexeme
			while ((numbCharsWritten < (MAX_LEXEME_SIZE - 1)) && (isDigit(currentCharToken) || (currentCharToken == PERIOD)) && (currentCharToken != LINE_FEED)) {
				lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
				getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
			}
			lexemeDataPtr->lexemeString[numbCharsWritten] = NULL_SYM;

			//Throw away any chars over the MAX_LEXEME_SIZE(If there are any).
			while ((isDigit(currentCharToken) || (currentCharToken == PERIOD)) && (currentCharToken != LINE_FEED)) {
				getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
			}
			//Lookup string to check if its a keyword
			lexemeDataPtr->lexToken = isFloat_or_Digit(lexemeDataPtr->lexemeString);
			lexemeDataPtr->tokenCompleted = TRUE;
		}
		else if ((currentCharToken == DOUBLE_QUOTE && lastTokenCompleted) || (!lastTokenCompleted && lastTokenProcessed == STRNG)) {
			/*
				STRNG
				Starts with double quote
				Process until we find a non escaped double quote or Reached MAX_LEXEME_SIZE(128 Chars + 1 Terminator) - *Read remainder of lexeme
				Return the LexemeData to Main
			*/

			//Set flag and lexeme data
			processingLexeme = TRUE;
			lexemeDataPtr->filePosition[0] = filePosition[0];
			lexemeDataPtr->filePosition[1] = filePosition[1];
			
			//Primer
			lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
			getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
			//Read the lexeme
			int stopFlag = 1;
			while ((numbCharsWritten < (MAX_LEXEME_SIZE - 1)) && stopFlag){
				lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
				if ((currentCharToken == DOUBLE_QUOTE && previousCharToken != BACKSLASH)) {
					stopFlag = 0;
				}else {
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
			}
			lexemeDataPtr->lexemeString[numbCharsWritten] = NULL_SYM;

			//Throw away any chars over the MAX_LEXEME_SIZE(If there are any).
			stopFlag = 1;
			while (stopFlag) {
				if ((currentCharToken == DOUBLE_QUOTE && previousCharToken != BACKSLASH)) {
					stopFlag = 0;
				}else {
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
			}

			lexemeDataPtr->lexToken = STRNG;
			lexemeDataPtr->tokenCompleted = TRUE;
		}
		else if ((currentCharToken == SINGLE_QUOTE && lastTokenCompleted) || (!lastTokenCompleted && lastTokenProcessed == CHAR)) {
			/*
				CHAR
				Starts with single quote
				Process until we find a non escaped single quote or Reached MAX_LEXEME_SIZE(128 Chars + 1 Terminator)
				Return the LexemeData to Main
			*/

			//Set flag and lexeme data
			processingLexeme = TRUE;
			lexemeDataPtr->filePosition[0] = filePosition[0];
			lexemeDataPtr->filePosition[1] = filePosition[1];

			//Primer
			lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
			getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
			//Read the lexeme
			int stopFlag = 1;
			while ((numbCharsWritten < (MAX_LEXEME_SIZE - 1)) && stopFlag) {
				lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
				if ((currentCharToken == SINGLE_QUOTE && previousCharToken != BACKSLASH)) {
					stopFlag = 0;
				}
				else {
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
			}
			lexemeDataPtr->lexemeString[numbCharsWritten] = NULL_SYM;

			//Throw away any chars over the MAX_LEXEME_SIZE(If there are any).
			stopFlag = 1;
			while (stopFlag) {
				if ((currentCharToken == SINGLE_QUOTE && previousCharToken != BACKSLASH)) {
					stopFlag = 0;
				}
				else {
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
			}

			lexemeDataPtr->lexToken = CHAR;
			lexemeDataPtr->tokenCompleted = TRUE;
		}
		else if (((previousCharToken == FORWARD_SLASH && currentCharToken == ASTERISK) && lastTokenCompleted) || (!lastTokenCompleted && lastTokenProcessed == CMMNT)) {
			/*	
				CMMNT
				Starts when(previousCharToken is a FORWARD_SLASHand currentCharToken is a ASTERISK) or (mutli_line_comment flag is set)
				If multi_line_comment not set then
				Set multi_line_comment flag = TRUE
				Copy previousCharToken and currentCharToken into lexeme
				else
				Copy currentCharToken into lexeme

				Process until(we find a NEW_LINE or Reached MAX_LEXEME_SIZE(128 Chars + 1 Terminator))
				Return the LexemeData to Main
				(or )
				Until(we have previousCharToken is a ASTERISK and currentCharToken is a FORWARD_SLASH or Reached MAX_LEXEME_SIZE(128 Chars + 1 Terminator)).
				Set multi_line_comment flag = FALSE
				Return the LexemeData to Main
			*/

			//Set flag and lexeme data
			processingLexeme = TRUE;
			lexemeDataPtr->filePosition[0] = filePosition[0];
			lexemeDataPtr->filePosition[1] = filePosition[1];
			
			//Add these to lexeme this is a start of a comment
			if (previousCharToken == FORWARD_SLASH && currentCharToken == ASTERISK) {
				lexemeDataPtr->lexemeString[numbCharsWritten++] = previousCharToken;
				lexemeDataPtr->filePosition[1] = (filePosition[1] - 1);
			}

			//Read the lexeme
			int stopFlag = 1;
			while ((numbCharsWritten < (MAX_LEXEME_SIZE - 1)) && stopFlag) {
				//Dont include a new line
				if (currentCharToken != LINE_FEED) {
					lexemeDataPtr->lexemeString[numbCharsWritten++] = currentCharToken;
				}
				if ((previousCharToken == ASTERISK && currentCharToken == FORWARD_SLASH) || currentCharToken == LINE_FEED) {
					stopFlag = 0;
					if (currentCharToken == LINE_FEED) {
						lexemeDataPtr->tokenCompleted = FALSE;
					}else {
						lexemeDataPtr->tokenCompleted = TRUE;
					}
				}else {
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
			}
			lexemeDataPtr->lexemeString[numbCharsWritten] = NULL_SYM;

			//Throw away any chars over the MAX_LEXEME_SIZE(If there are any).
			stopFlag = 1;
			while (stopFlag && !lexemeDataPtr->tokenCompleted) {
				if (currentCharToken == LINE_FEED) {
					stopFlag = 0;
				}
				else {
					getNextChar(cFilePtr, &previousCharToken, &currentCharToken, filePosition);
				}
			}
			//Continues in the next line. Columns 1
			if (!lastTokenCompleted) {
				lexemeDataPtr->filePosition[1] = 1;
			}
			lexemeDataPtr->lexToken = CMMNT;
		}
		else if (currentCharToken == _EOF) {
			/*	
				END_OF_FILE
				When feof(cFilePtr) is not zero
				Return LexemeData to Main
			*/
			processingLexeme = TRUE;
			lexemeDataPtr->filePosition[0] = filePosition[0];
			lexemeDataPtr->filePosition[1] = filePosition[1];
			lexemeDataPtr->lexemeString[numbCharsWritten] = NULL_SYM;
			lexemeDataPtr->lexToken = END_OF_FILE;
			lexemeDataPtr->tokenCompleted = TRUE;
		}
		else {
			/* Still looking for a lexeme start */
			processingLexeme = FALSE;
		}
	}

	//Update Static Variables
	lastTokenProcessed = lexemeDataPtr->lexToken;
	lastTokenCompleted = lexemeDataPtr->tokenCompleted;

	return lexemeDataPtr;
}
/*
	Name: getNextChar
	Returns: NA
	Parameters:
		In: FILE * cFilePtr, CharToken* previousCharTokenPtr, CharToken * currentCharTokenPtr, unsigned int * filePositionPtr
		In/Out: unsigned int * filePositionPtr, CharToken* previousCharTokenPtr, CharToken * currentCharTokenPtr
		Out: NA
	Description: Gets the next character from the file stream. Updates the current and previous token. Lastly, it will update the file position.
*/
void getNextChar(FILE * cFilePtr, CharToken* previousCharTokenPtr, CharToken * currentCharTokenPtr, unsigned int * filePositionPtr) {
	//Set previousCharToken to the currentCharToken
	*previousCharTokenPtr = *currentCharTokenPtr;

	//Check if EOF
	if (feof(cFilePtr) != 0) {
		//Found EOF
		*currentCharTokenPtr = _EOF;
		filePositionPtr[0] += 1;
		filePositionPtr[1] = 0;
		return;
	}

	//Get and set the next currentCharToken
	*currentCharTokenPtr = fgetc(cFilePtr);

	//Calculate the new file position	
	if (*currentCharTokenPtr == LINE_FEED) {
		filePositionPtr[0] += 1;
		filePositionPtr[1] = 0;
	}else if (*currentCharTokenPtr == HORIZONTAL_TAB) {
		filePositionPtr[1] += 1;
		filePositionPtr[1] = (((filePositionPtr[1] + 3) / 4) * 4);
	}else {
		filePositionPtr[1] += 1;
	}
}
/*
	Name: tokenIdentifierToString
	Returns: char *
	Parameters:
		In: const LexTokenType tokenType
		In/Out: NA
		Out: Char *. The variable is a constant.
	Description: Takes a LexTokenType and returns the string representation of the LexTokenType
*/
char * tokenIdentifierToString(const LexTokenType tokenType) {
	switch (tokenType) {
		case PPDIR:
			return "PPDIR";
		break;
		case IDENT:
			return "IDENT";
		break;
		case KEYWD:
			return "KEYWD";
		break;
		case INTGR:
			return "INTGR";
		break;
		case FLOAT:
			return "FLOAT";
		break;
		case STRNG:
			return "STRNG";
		break;
		case CHAR:
			return "CHAR";
		break;
		case CMMNT:
			return "CMMNT";
		break;
		case END_OF_FILE:	
			return "EOF";
		break;
		default:
			return "UNKNOWN_TOKEN";
		break;
	}
}
/*
	Name: isKeyword_or_Ident
	Returns: LexTokenType
	Parameters:
		In: char * lookupString
		In/Out: NA
		Out: LexTokenType lexToken
	Description: Function takes a string. It will then look the string up in a table of consts. If the string exists in the table then it will change the LexTokenType to a KEYWD.
*/
LexTokenType isKeyword_or_Ident(char* lookupString) {
	LexTokenType lexToken = IDENT;
	
	const char * c_keywords[32] = {
		"auto",
		"break",
		"case",
		"char",
		"const",
		"continue",
		"default",
		"do",
		"double",
		"else",
		"enum",
		"extern",
		"float",
		"for",
		"goto",
		"if",
		"int",
		"long",
		"register",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"struct",
		"switch",
		"typedef",
		"union",
		"unsigned",
		"void",
		"volatile",
		"while"
	};
	
	for (int numbKeywords = 0; ((numbKeywords <= 31) && (lexToken != KEYWD)); numbKeywords++) {
		if (!strcmp(c_keywords[numbKeywords], lookupString)) {
			lexToken = KEYWD;
		}
	}
	
	return lexToken;
}
/*
	Name: isFloat_or_Digit
	Returns: LexTokenType
	Parameters:
		In: char * lookupString
		In/Out: NA
		Out: LexTokenType lexToken
	Description: Function takes a string input. The function then checks if a . exists in the string. If a . exists then it is considered a float. LexTokenType is set to a FLOAT
*/
LexTokenType isFloat_or_Digit(char * lookupString) {
	LexTokenType lexToken = INTGR;

	if (strchr(lookupString, '.') != NULL) {
		//This is a float
		lexToken = FLOAT;
	}

	return lexToken;
}
/*
	Name: isAlphabetic
	Returns: Boolean
	Parameters:
		In: const CharToken aCharToken
		In/Out: NA
		Out: Boolean
	Description: Function takes in a single CharToken. If the token is between 65 and 90 or the token is between 97 and 122 then it is a letter. These numbers are based on the ASCII representation of a char.
*/
Boolean isAlphabetic(const CharToken aCharToken) {
	if ((aCharToken >= 65 && aCharToken <= 90) || (aCharToken >= 97 && aCharToken <= 122)) {
		return TRUE;
	}
	return FALSE;
}
/*
	Name: isDigit
	Returns: Boolean
	Parameters:
		In: const CharToken aCharToken
		In/Out: NA
		Out: Boolean
	Description: Function takes in a single CharToken. If the token is between 48 and 57 then the CharToken is considered a digit. These numbers are based on the ASCII representation of a char.
*/
Boolean isDigit(const CharToken aCharToken) {
	if ((aCharToken >= 48 && aCharToken <= 57)) {
		return TRUE;
	}
	return FALSE;
}
/*
	Name: printAndWrite
	Returns: NA
	Parameters:
		In: FILE* txtFilePtr, Lexeme* lexemeDataPtr, unsigned int * tokensProcessed
		In/Out: NA
		Out: unsigned int * tokensProcessed
	Description: Function takes a output file pointer to write the lexeme data too (Line Number, Column Number, Token Name and the Lexeme.
	The function will also increment the token processed if the lexemeDataPtr struct has a tokenCompleted flag set to TRUE.
*/
void printAndWrite(FILE* txtFilePtr, Lexeme* lexemeDataPtr, unsigned int * tokensProcessed) {
	char* tokenStringName = tokenIdentifierToString(lexemeDataPtr->lexToken);

	/* Only assign first 8 TokenTypes */
	if (lexemeDataPtr->lexToken <= 7) {
		if (lexemeDataPtr->tokenCompleted) {
			tokensProcessed[lexemeDataPtr->lexToken] += 1;
		}
	}

	/* End of file doesnt follow the format? */
	if (lexemeDataPtr->lexToken == END_OF_FILE) {
		printf("(%3i, %3i)\t%s\t\n", lexemeDataPtr->filePosition[0], lexemeDataPtr->filePosition[1], tokenStringName);
		//Write to file
		fprintf(txtFilePtr, "(%3i, %3i)\t%s\t\n", lexemeDataPtr->filePosition[0], lexemeDataPtr->filePosition[1], tokenStringName);
	}else {
		/* Space the token by the max token char length. */
		printf("(%3i, %3i)\t%-5s\t%s\n", lexemeDataPtr->filePosition[0], lexemeDataPtr->filePosition[1], tokenStringName, lexemeDataPtr->lexemeString);
		//Write to file
		fprintf(txtFilePtr, "(%3i, %3i)\t%-5s\t%s\n", lexemeDataPtr->filePosition[0], lexemeDataPtr->filePosition[1], tokenStringName, lexemeDataPtr->lexemeString);
	}
}
/*
	Name: printProcessedTokenData
	Returns: NA
	Parameters:
		In: FILE* txtFilePtr, const unsigned int* tokensProcessed, const unsigned int* linesProcessed
		In/Out: NA
		Out: NA
	Description: Function takes a output file pointer to write the number of tokens processed for each token. Also it will have the number of lines processed.
*/
void printProcessedTokenData(FILE* txtFilePtr, const unsigned int* tokensProcessed, const unsigned int* linesProcessed) {
	printf("\n\nProcessed %4i lines\n", *linesProcessed);
	printf("%10s%4i PPDIRs\n", "", tokensProcessed[0]);
	printf("%10s%4i IDENTs\n", "", tokensProcessed[1]);
	printf("%10s%4i KEYWDs\n", "", tokensProcessed[2]);
	printf("%10s%4i INTGRs\n", "", tokensProcessed[3]);
	printf("%10s%4i FLOATs\n", "", tokensProcessed[4]);
	printf("%10s%4i STRNGs\n", "", tokensProcessed[5]);
	printf("%10s%4i CHARs\n", "", tokensProcessed[6]);
	printf("%10s%4i CMMNTs\n", "", tokensProcessed[7]);
	//Write to file
	fprintf(txtFilePtr, "\n\nProcessed %4i lines\n", *linesProcessed);
	fprintf(txtFilePtr, "%10s%4i PPDIRs\n", "", tokensProcessed[0]);
	fprintf(txtFilePtr, "%10s%4i IDENTs\n", "", tokensProcessed[1]);
	fprintf(txtFilePtr, "%10s%4i KEYWDs\n", "", tokensProcessed[2]);
	fprintf(txtFilePtr, "%10s%4i INTGRs\n", "", tokensProcessed[3]);
	fprintf(txtFilePtr, "%10s%4i FLOATs\n", "", tokensProcessed[4]);
	fprintf(txtFilePtr, "%10s%4i STRNGs\n", "", tokensProcessed[5]);
	fprintf(txtFilePtr, "%10s%4i CHARs\n", "", tokensProcessed[6]);
	fprintf(txtFilePtr, "%10s%4i CMMNTs\n", "", tokensProcessed[7]);
}

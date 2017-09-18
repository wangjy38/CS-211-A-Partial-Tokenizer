/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

enum states {
    State0,
    State1,
    State2,
    State3,
    State4,
    State5,
    State6,
    State7,
    State8,
    State9,
    State10,
    State11,
    State12,
    Malformed,
    Invalid
    
} curr_State;

char error;

struct TokenizerT_ {
    int Pointer;   			
    int String_length;		
    char *input_String;	
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

int isInvalid(char c, int xBefore, int dotBefore, int numBefore, int zeroBefore, int eBefore, int zeroThefirst, int onetonineBefore){// this function decide whether one character is a invalid 
    if ((c >= '8' && c <='9')&& (zeroThefirst==1)) return 1; 
    if( (c >= '0' &&c <= '9') || ((c >= 'A' && c <='F') && (xBefore == 1)) || ((c >= 'a' && c <= 'f') &&(xBefore == 1)) || ((c=='X')&& (zeroBefore==1)) || ((c=='x')&&(zeroBefore==1)) ||( (c=='.')&& dotBefore == 0 && numBefore == 1&&(onetonineBefore==1))|| ((c=='E')&&(numBefore==1)) || ((c=='e')&&(numBefore==1)) || ((c=='+')&&(eBefore==1)&&(numBefore==1)) || ((c=='-')&&(eBefore==1)&&(numBefore==1)) ){
        return 0;
    }else
        return 1;
    
}

TokenizerT *TKCreate( char * ts ) {
    if (ts == NULL)
        return NULL;
    else {
    char copy[strlen(ts)]; 					
    strcpy(copy,ts);
    
    TokenizerT *tk = (TokenizerT*) malloc (sizeof(struct TokenizerT_));   
    tk -> Pointer = 0;
    tk -> String_length = (int)strlen(ts);
    tk -> input_String = ts;
    
    return tk;           

    }
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
    free (tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
    int Current = tk -> Pointer;
    int arrBound = (tk -> String_length) - 1;
    int end;
    char *input_String_ = tk -> input_String;
   
    if (Current > arrBound) { 			
        return 0;
    }
    
    while (isspace(input_String_[Current]) && Current < arrBound) {			
        Current++;
    }
    
    if (Current == arrBound && isspace(input_String_[Current]))
        return 0;				
    
    end = Current; 
    int xBefore=0;
    int dotBefore=0;
    int numBefore=0;
    int zeroBefore=0;
    int eBefore=0;
    int foundInvalid=0;
    int zeroThefirst=0;
    int onetonineBefore=0;
    if (input_String_[Current]== '0') zeroThefirst=1;
    while (!isspace(input_String_[end]) && end <= arrBound&&!isInvalid(input_String_[end],xBefore,dotBefore,numBefore,zeroBefore,eBefore,zeroThefirst,onetonineBefore)) {  
       if (input_String_[end]=='x'||input_String_[end]=='X') {
           xBefore=1; 
           zeroThefirst=0;
           }
       if (input_String_[end]=='e'||input_String_[end]=='E') eBefore=1;
       if (input_String_[end]=='.') {
           dotBefore=1;
           numBefore=0; 
           zeroThefirst=0;
          } 
       if (input_String_[end]<='9' && input_String_[end]>='0') numBefore=1; 
       if (input_String_[end]=='0') zeroBefore=1; 
       if (input_String_[end]<='9' && input_String_[end]>='1') onetonineBefore=1; 
       if (input_String_[end]=='0' && input_String_[end+1]=='.' && end==Current) onetonineBefore=1; 
       end++;
    }
    if (isInvalid(input_String_[end],xBefore,dotBefore,numBefore,zeroBefore,eBefore,zeroThefirst,onetonineBefore)) foundInvalid = 1;
    //printf ("%d", end);
    //printf ("%s", "    "); 
 
    int tokenSize = end - Current;
    //printf("%c", input_String_[Current]);
    if ((isInvalid(input_String_[Current],xBefore,dotBefore,numBefore,zeroBefore,eBefore,zeroThefirst,onetonineBefore))) {
        //tk -> Pointer--;
        tokenSize++;
        end++;
        tk -> Pointer = end;
        }else {
               if (foundInvalid==1) {
                   tk ->Pointer = end;
               }
               else{
                    tk -> Pointer = end + 1;
                    }
    //tokenSize--;
    }
    //printf ("%d",tokenSize);
    //tk -> Pointer = end + 1;
    //if ((isInvalid(input_String_[end],xBefore,dotBefore,numBefore,zeroBefore))) tk -> Pointer--;
    char *token = (char*) malloc ((tokenSize+2)*sizeof(char));
    
    int i;
    for (i = 0; i < tokenSize; i++) {
        token[i] = input_String_[i+Current];
    }
    token[i] = '\0';
    //printf ("%s",token);
    //printf ("%c",' ');  
    return token;
    
}


int malChar(char ptr) {
    error = ptr;
    return 0;
}
int invalid(char ptr) {
    error = ptr;
    return 0;
}



// 12 states according to the finite state map
int state0(char x) {
    if (x == '0') {
        curr_State = State1;
    }
    else if ((x >= '1' && x <= '9')) {
        curr_State = State7;
    }
    else {
        curr_State = Invalid;
        invalid(x);
    }
    return 0;
}


int state1(char x) {
    
    if (x >= '0' && x <= '7') {
        curr_State = State2;
    }
    else if (x == 'x' || x == 'X') {
        curr_State = State3;
    }
    else if (x == '.') {
        curr_State = State5;
    }
    else{
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}


int state2(char x) {
    if (x >= '0' && x <= '7') {
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}


int state3(char x) {
    if ((x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F')) {
        curr_State = State4;
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}
int state4(char x){
    if ((x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F')) {
    
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}


int state5(char x) {
    if (x >= '0' && x <= '9') {
        curr_State = State6;
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}


int state6 (char x) {
   
    if (x == 'e' || x == 'E') {
        curr_State = State10;
    }
    else if (x >= '0' && x <= '9') {
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}
int state7 (char x) {
    
    if (x >= '0' && x <= '9') {
    }
    else if (x == '.') {
        curr_State=State8;
    }
    else if (x == 'E' || x == 'e'){
        curr_State=State10;
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}
int state8 (char x) {

    if (x >= '0' && x <= '9') {
        curr_State= State9;
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}


int state9(char x) {
    if (x >= '0' && x <= '9') {
        
    }
    else if (x == 'e' || x == 'E'){
        curr_State = State10;
    }
    else {
        curr_State = Malformed;
        malChar(x);
        
    }
    return 0;
}


int state10(char x) {
    if ((x >= '0' && x <= '9')){
    
        curr_State=State12;
    }
    else if (x == '-' || x == '+'){
        curr_State	= State11;
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}
int state11(char x) {
    if ((x >= '0' && x <= '9')){
        
        curr_State=State12;
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}
int state12(char x) {
    if ((x >= '0' && x <= '9')){
        
        curr_State=State12;
    }
    else {
        curr_State = Malformed;
        malChar(x);
    }
    return 0;
}
int IdentifyToken (char *ptr) { // Switch states 
    while (*ptr != '\0') {
        switch(curr_State) {
            case (Malformed): {     /*temporary*/
                return 0;
            }
            case (Invalid):{
                return 0;
            }
            case(State0): {
                state0(*ptr);
                break;
            }
            case(State1): {
                state1(*ptr);
                break;
            }
            case(State2): {
                state2(*ptr);
                break;
            }
            case(State3): {
                state3(*ptr);
                break;
            }
            case(State4): {
                state4(*ptr);
                break;
            }
            case(State5): {
                state5(*ptr);
                break;
            }
            case(State6): {
                state6(*ptr);
                break;
            }
            case(State7): {
                state7(*ptr);
                break;
            }
            case(State8): {
                state8(*ptr);
                break;
            }
            case(State9): {
                state9(*ptr);
                break;
            }
            case(State10): {
                state10(*ptr);
                break;
            }
            case(State11): {
                state11(*ptr);
                break;
            }
            case(State12): {
                state12(*ptr);
                break;
            }
        
                default:
                break;
        }
        ptr++;
    }
    
    return 0;
}

char* TKGetState() {
    switch (curr_State) {   // print out the tyoe of token according it the final state 
        case(Malformed): {
            return "Malformed";
            break;
        }
        case(Invalid):{
            return "invalid";
            break;
        }
        case (State0): {
            return "Error";
            break;
        }
        case (State1): {
            return "zero";
            break;
        }
        case (State2): {
            return "octal";
            break;
        }
        case (State3): {
            return "Malformed";
            break;
        }
        case (State4): {
            return "hexadecimal";
            break;
        }
        case (State5): {
            return "Malformed";
            break;
        }
        case (State6): {
            return "float";
            break;
        }
        case (State7): {
            return "decimal";
            break;
        }
        case (State8): {
            return "Malformed";
            break;
        }
        case (State9): {
            return "float";
            break;
        }
        case (State10): {
            return "Malformed";
            break;
        }
        case (State11): {
            return "Malformed";
            break;
        }
        case (State12): {
            return "float";
            break;
        }
        default:
            return NULL;
            break;
    }
}
/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
    if (argc == 1 || argv[1][0] == '\0') {									//in case no arguments are given
        return 0;
    }
    
    TokenizerT *tokenizer = TKCreate (argv[1]);        //creation of tokenizerT
    
    curr_State = State0;
    char *token =TKGetNextToken(tokenizer);
    char *status;
    
    while (token != 0) { 
        IdentifyToken(token);
        status = TKGetState();
        fprintf(stdout,"%s %s ",status,token);
        if (curr_State == Malformed) {
            fprintf(stdout,"Error caused by: [0x%x]\n",error); // print out Error message. 
        } else if (curr_State == Invalid) {
            fprintf(stdout,"[0x%x]\n",error); // print out Error message.  
        }
        else
            fprintf(stdout,"\n");
        curr_State = State0;
        token = TKGetNextToken(tokenizer);
    }
    
    free (token);
    TKDestroy(tokenizer);
				
    return 0;
}


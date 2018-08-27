/*
 * SPFIARParser.c
 *
 *  Created on: 12 בדצמ 2017
 *      Author: omerg
 */
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include "SPMainAux.h"
#include "SPFIARParser.h"

bool spParserIsInt(const char* str){
	bool b;
	int len = strlen(str);
	char *p, s[len];
	if(len == 0)
		return false;
	if(len==1)
		return ((*str)>='0' && (*str)<='9');
	strcpy(s,str);
	p=s;
	if(!((*p)=='-' || ((*p)>='0' && (*p)<='9'))){
		return false;
	}
	b =(intToBool(forall(p,len-1,isDigit)));
	return b;
}


SPCommand spParserPraseLine(const char* str){
	SPCommand spc;
	spc.validArg=false;
	char *p,s[SP_MAX_LINE_LENGTH];
	int size = strlen(str);
	if(size>SP_MAX_LINE_LENGTH){
		spc.cmd = SP_INVALID_LINE;
		return spc;
	}
	strcpy(s,str);
	p = strtok(s," \t\n\r");
	if(p==NULL){
		spc.cmd = SP_INVALID_LINE;
		return spc;
	}
	spc.cmd = toSP_COMMAND(p);
	if(spc.cmd == SP_ADD_DISC){
		p=strtok(NULL," \t\n\r");
		if(p==NULL){
			spc.cmd = SP_INVALID_LINE;
		}
		else if(spParserIsInt(p)){
			spc.arg=atoi(p);
			spc.validArg=true;
		}
	}
	if(!(strtok(NULL," \t\n\r") == NULL))
		spc.cmd = SP_INVALID_LINE;
	return spc;
}




/*
** EPITECH PROJECT, 2019
** AIA_gomoku_2019
** File description:
** piskpipe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "pisqpipe.h"

int width, height;
int info_timeout_turn = 30000; //in milliseconds
int info_timeout_match = 1000000000;
int info_time_left = 1000000000;
int info_max_memory = 0; //in bytes zero if unlimited
int info_game_type = 1; //0:human opponent, 1:AI opponent, 2:tournament, 3:network tournament
int info_continuous = 0; /* 0:single game, 1:continuous */
int terminateAI; /* return from brain_turn when terminateAI>0 */
unsigned start_time; /* tick count at the beginning of turn */
char dataFolder[256]; /* folder for persistent files */

static char cmd[256];
void event1, event2;        //POSSIBLE SOURCE D'ERREUR


/** write a line to STDOUT */
int pipeOut(char *fmt, ...)
{
	int i;
	va_list va;
	va_start(va, fmt);
	i=vprintf(fmt, va);
	putchar('\n');
	fflush(stdout);
	va_end(va);
	return i;
}

/** read a line from STDIN */
static void get_line()
{
	int c, bytes;

	bytes=0;
	do{
		c=getchar();
		if(c==EOF) exit(0);
		if(bytes<sizeof(cmd)) cmd[bytes++]=(char)c;
	} while(c!='\n');
	cmd[bytes-1]=0;
	if(cmd[bytes-2]=='\r') cmd[bytes-2]=0;
}

/** parse coordinates x,y */
static int parse_coord(const char *param, int *x, int *y)
{
	if(sscanf(param, "%d,%d", x, y)!=2 ||
		*x<0 || *y<0 || *x>=width || *y>=height){
		return 0;
	}
	return 1;
}

/** parse coordinates x,y and player number z */
static void parse_3int_chk(const char *param, int *x, int *y, int *z)
{
	if(sscanf(param, "%d,%d,%d", x, y, z)!=3 || *x<0 || *y<0 ||
		*x>=width || *y>=height) *z=0;
}

/** return pointer to word after command if input starts with command, otherwise return NULL */
static const char *get_cmd_param(const char *command, const char *input)
{
	int n1, n2;
	n1=(int)strlen(command);
	n2=(int)strlen(input);
	if(n1>n2 || strncasecmp(command, input, n1)) return NULL; /* it is not command */
    //***************************************************************************************************************//
    //LA LIGNE CI-DESSUS PEUT ETRE UNE SOURCE DE PROBLEME
    //***************************************************************************************************************//
	input+=strlen(command);
	while(isspace(input[0])) input++;
	return input;
}

/** send suggest */
void suggest(int x, int y)
{
	pipeOut("SUGGEST %d,%d", x, y);
}

/** write move to the pipe and update internal data structures */
void do_mymove(int x, int y)
{
	brain_my(x, y);
	pipeOut("%d,%d", x, y);
}

/** start thinking */
static void turn()
{
	terminateAI=0;
    (void)event2;           //POSSIBLE SOURCE D'ERREUR
	(void)event1;
}

/** stop thinking */
static void stop()
{
	terminateAI=1;
	//WaitForSingleObject(event2, 0xFFFFFFFF);      //POSSIBLE SOURCE D'ERREUR
}

static void start()
{
	//start_time=GetTickCount();            //POSSIBLE SOURCE D'ERREUR
	stop();
	if(!width){
		width=height=20;
		brain_init();
	}
}

/** do command cmd */
static void do_command()
{
	const char *param;
	const char *info;
	char *t;
	int x, y, who, e;
    
	if((param=get_cmd_param("info", cmd))!=0) {
		if((info=get_cmd_param("max_memory", param))!=0) info_max_memory=atoi(info);
		if((info=get_cmd_param("timeout_match", param))!=0) info_timeout_match=atoi(info);
		if((info=get_cmd_param("timeout_turn", param))!=0) info_timeout_turn=atoi(info);
		if((info=get_cmd_param("time_left", param))!=0) info_time_left=atoi(info);
		if((info=get_cmd_param("game_type", param))!=0) info_game_type=atoi(info);
		if((info=get_cmd_param("rule", param))!=0){ e=atoi(info); info_exact5=e&1; info_continuous=(e>>1)&1; info_renju=(e>>2)&1; }
		if((info=get_cmd_param("folder", param))!=0) strncpy(dataFolder, info, sizeof(dataFolder)-1);
#ifdef DEBUG_EVAL
		if((info=get_cmd_param("evaluate", param))!=0){ if(parse_coord(info, &x, &y)) brain_eval(x, y); }
#endif
		/* unknown info is ignored */
	}
	else if((param=get_cmd_param("start", cmd))!=0) {
		if(sscanf(param, "%d", &width)!=1 || width<5){
			width=0;
			pipeOut("ERROR bad START parameter");
		}
		else{
			height=width;
			start();
			brain_init();
		}
	}
	else if((param=get_cmd_param("rectstart", cmd))!=0) {
		if(sscanf(param, "%d ,%d", &width, &height)!=2 || width<5 || height<5){
			width=height=0;
			pipeOut("ERROR bad RECTSTART parameters");
		}
		else{
			start();
			brain_init();
		}
	}
	else if((param=get_cmd_param("restart", cmd))!=0) {
		start();
		brain_restart();
	}
	else if((param=get_cmd_param("turn", cmd))!=0) {
		start();
		if(!parse_coord(param, &x, &y)){
			pipeOut("ERROR bad coordinates");
		}
		else{
			brain_opponents(x, y);
			turn();
		}
	}
	else if((param=get_cmd_param("play", cmd))!=0) {
		start();
		if(!parse_coord(param, &x, &y)){
			pipeOut("ERROR bad coordinates");
		}
		else{
			do_mymove(x, y);
		}
	}
	else if((param=get_cmd_param("begin", cmd))!=0) {
		start();
		turn();
	}
	else if((param=get_cmd_param("about", cmd))!=0) {
#ifdef ABOUT_FUNC
		brain_about();
#else
		pipeOut("%s", infotext);
#endif
	}
	else if((param=get_cmd_param("end", cmd))!=0) {
		stop();
		brain_end();
		exit(0);
	}
	else if((param=get_cmd_param("board", cmd))!=0) {
		start();
		for(;;){ /* fill the whole board */
			get_line();
			parse_3int_chk(cmd, &x, &y, &who);
			if(who==1) brain_my(x, y);
			else if(who==2) brain_opponents(x, y);
			else if(who==3) brain_block(x, y);
			else{
				if(strcasecmp(cmd, "done")) pipeOut("ERROR x,y,who or DONE expected after BOARD");
				break;
			}
		}
		turn();
	}
	else if((param=get_cmd_param("takeback", cmd))!=0) {
		start();
		t="ERROR bad coordinates";
		if(parse_coord(param, &x, &y)){
			e= brain_takeback(x, y);
			if(e==0) t="OK";
			else if(e==1) t="UNKNOWN";
		}
		pipeOut(t);
	}
	else{
		pipeOut("UNKNOWN command");
	}
}


/** main function for AI console application  */
int main()
{
#ifdef DEBUG
	SetErrorMode(0);
#endif
	// event1=CreateEvent(0, FALSE, FALSE, 0);					//POSSIBLE SOURCE D'ERREUR
	// event2=CreateEvent(0, TRUE, TRUE, 0);
	for(;;){
		get_line();
		do_command();
	}
}
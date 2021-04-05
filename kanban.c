/*
 *
 *File:        kanban.c
 *Author:      Allan Donizette Cravid Fernades 97281
 *Description: a schedulling system for tasks management
 *  
 */


/*  Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*  Macros*/               
#define START 1        /*true value for the command line parsing*/
#define TASKSMAX  10000/*maximum number of tasks*/ 
#define TASKINFO 50    /*maximum number of characters of a task description*/
#define ACTIVITYMAX 13 /*maximum number of activities in the system --> 10
however, the value of ACTIVITYMAX 13 already takes in account the three 
standard actvitities */
#define ACTIVITYINFO 21/*maximum number of characters of an activity description*/
#define USERMAX 50     /*maximum number of users in the system*/
#define USERNAME 20    /*maximum number of characters of an user description*/
#define ZERO 0         /*comparison/computation value*/
#define ONE 1          /*comparison/computation value*/
#define TWO 2          /*comparison/computation value*/
#define ZERO 0         /*comparison/computation value*/
#define THREE 3        /*computation value*/
#define STAGE1         "TO DO"       /* first stage of an activity*/
#define STAGE2         "IN PROGRESS" /*second stage of an activity*/
#define STAGE3         "DONE"        /*last stage of an activity*/
#define TASK           "task"
#define DURATION       "duration"
#define SLACK          "slack"
#define ZEROCHAR       '0'
#define NUL            '\0'
#define NLINE          '\n'

/* Macros used in error handling*/
#define MAXTASKS       "too many tasks"
#define MAXUSERS       "too many users"
#define MAXACT         "too many activities"
#define INFODUPLICATED "duplicate description"
#define ACTDUPLICATED  "duplicate activity" 
#define INVALIDTIME    "invalid duration"
#define NONVALIDTIME   "invalid time"
#define NONVALIDINFO   "invalid description"
#define MAXUSERS       "too many users"
#define TASKFSYSTEM    "no such task"
#define USERFSYSTEM    "no such user"
#define ACTFSYSTEM     "no such activity"
#define TASKTSTARTED   "task already started"
#define USERINSYSTEM   "user already exists"

/* booleans definiton*/

typedef enum {false = ZERO,true = !false} boolean;

/*  Definition of Structures type*/

/*
 * Structure Name: Activity_t
 * ----------------------
 * Description: Activity structure definition
 * A task is characterized by the following:
 * 
 *         * a name,       what is it with want to accomplish 
 * 
 */

typedef struct{

    char name[ACTIVITYINFO];
    char stage[ACTIVITYINFO];
    

}Activity_t;


/*
 * Structure Name: User_t
 * ----------------------
 * Description: User structure definition
 * A task is characterized by the following:
 * 
 *         * a name
 * 
 */

typedef struct{

    char name[USERNAME];

}User_t;

/*
 * Structure Name: Task_t
 * ----------------------
 * Description: Task structure definition
 * A task is characterized by the following:
 * 
 *          * an identifier,    id
 *          * a description,    info
 *          * an user
 *          * an activity 
 *          * a duration
 *          * a starting time   instant
 * 
 */

typedef struct{ 

    short int id;
    char info[TASKINFO];
    User_t user;
    Activity_t activity;
    short int duration;
    short int instant;

}Tasks_t;

/*  (Global)*/

/* array variables*/

/*  an array of Users in which the index indentifies the current user,
    the system supports a maximum of 50 users*/
User_t users[USERMAX];

/*  an array of Activities in which the index indentifies the current
   activity,the system allows a maximum of 10 activities */
Activity_t activities[ACTIVITYMAX];

/* an array of Tasks, the index + 1 indentifies the current task 
   externally,the system supports a maximum of 10000 tasks*/
Tasks_t tasks[TASKSMAX];


int listby_id[TASKSMAX];     /*stores the saved ids,command d*/
int listby_idCMDd[TASKSMAX]; /*stores the saved ids, command a*/

/*  variables*/
int tsk_counter;        /*counts the number of tasks in the system*/
int usr_counter;        /*counts the number of users in the system*/
int act_counter;        /*counts the number of activities in the system*/
int clock;              /*current time in the system*/

/*  Function Prototypes*/

void addTaskToSystem();  
void listTask();         
void advanceTimeSystem();
void addUserListUser();  
void moveTask();         
void listTaskActvity(); 
void addOrListActivity();


/* Auxiliary Functions*/

int isTaskinSystem(int id); 
int nonNegativeIntChecker(int time); 
int isUserinSystem(char name[]);
int taskAlreadyStarted(int id,char activity[]);
int taskDone(char activity[]);
int isActivityinSystem(char name[]); 
int isTaskInfoDuplicated(char s[]);  
int isActivityInfoDuplicated(char s[]);
int testActivityLower(char activity[]);
void listTasksValid(int c,int i,int k);
void createTask(int duration , char info[]);
void createUser(char user[]);
void createUserValid(char user[]);
void testMoveTask(int id,char user[], char activity[]);
void processActivity(char activity[],int counter);
void testCreateActivity(char activity[]);

/*  Sorting*/

/*auxiliary arrays used in sorting,command l and d*/
int auxCMD[TASKSMAX + ONE];        
int aux[TASKSMAX + ONE];
      

/*sortformat = ONE, is associated  with sort by alphabetical order 
 of description*/
/*sortformat = ZERO, is associated with sort by starting time*/
void mergesort(int a[],int aux[], int l, int r,int sortformat);
void merge(int a[],int aux[],int l, int m, int r,int sortformat); 
void printSortAlphabetical();
void printListTask(int i);
int cmpInfo(int,int);
int cmpStartingTime(int,int);

int  main(){

    char cmd;
    act_counter = THREE; /* sets the activity counter to 3*/
    
    /*/*  the system already has beforehand three different activites
    i.e. TO DO (STAGE1), IN PROGRESS (STAGE2) and DONE (STAGE3) */
    strcpy(activities[ZERO].name,STAGE1);strcpy(activities[ONE].name,STAGE2);
    strcpy(activities[TWO].name,STAGE3);

    while(START){

        cmd = getchar();
        switch(cmd){
            case    'q':
                return EXIT_SUCCESS;
            case    't':
                addTaskToSystem();
                break;
            case    'l':
                listTask();
                break;
            case    'n':
                advanceTimeSystem();
                break;
            case    'u':
                addUserListUser(); 
                break;
            case    'm':
                moveTask();
                break;
            case    'd':
                listTaskActvity();
                break;
            case    'a':
                addOrListActivity();
                break; 
        }
    }
    return EXIT_FAILURE;
}




/*---------------------------------------------------------------------------*/

/* Auxiliary Functions */

/*checks for non negative integers*/
int  nonNegativeIntChecker(int num){
    
    int res;
    res = (num >= false) ? true:false;
    return res;
}

/* checks if the given task is already in the system*/
int isTaskInfoDuplicated(char s[]){

    int i;
    
    for(i = 0; i < tsk_counter;i++){
        if(strcmp(users[i].name,s) == false){
            return true;
        }
    }
    return false;
}

/*  checks if the given activity is already in the system*/
int isActivityInfoDuplicated(char s[]){

    int i;
    
    for(i = THREE; i < act_counter;i++){
        if(strcmp(activities[i].name,s) == false){
            return true;
        }
    }
    return false;

}

/* verifies if an user name is already in the system*/
int isUserinSystem(char name[]){

    int i;

    for(i = 0; i < usr_counter; i++){
        if(strcmp(users[i].name,name) == false){
            return true;
        }
    }
    return false;
}

/* verifies if the task with the given id is in the system*/
int isTaskinSystem(int id){

    int i;

    for(i = ONE; i <= tsk_counter; i++){
        if(tasks[i - ONE].id == id){ /*id starts at one, */
            return true;                     /*the - ONE removes the offset*/
        }
    }
    return false;
}

/* verifies if the activity is in the system*/
int isActivityinSystem(char activity[]){

    int i;

    for(i = 0; i < act_counter; i++){
        if(strcmp(activities[i].name,activity) == false){
            return true;
        }
    }
    return false;
}

/*  verifies if a task already started*/
/*  verifies if a task already started*/
int taskAlreadyStarted(int id,char activity[]){

    return(tasks[id - ONE].instant > ZERO && (strcmp(activity,STAGE1) == false)
          ); 
}

/*  verifies if a task has reached its last stage, DONE*/
int taskDone(char activity[]){
    return(strcmp(STAGE3,activity) == false);
}


/* checks for a valid activity name*/
int testActivityLower(char activity[]){

    int i;
    for(i = 0; activity[i] != NUL; i++){
        if(islower(activity[i])){
            return true;
        }
    }
    return false;
}

/* condition used in the sorting algorithm, sort by alphabetical order */
int cmpInfo(int i, int j){
    return ((strcmp(tasks[i].info,tasks[j].info) < ZERO) ? true: false);
}


/* condition used in the sorting algorithm, sort by ascending order 
of starting time */
int cmpStartingTime(int i,int j){
    
    if(tasks[i].instant < tasks[j].instant){
        return true;
    }
    else if( tasks[i].instant > tasks[j].instant){
        return false;
    }
    else{
        return THREE;
    }  
}


/* prints list task message, command l*/
void printListTask(int i){

    printf("%d %s #%d %s\n",tasks[i].id,tasks[i].activity.name,
    tasks[i].duration,tasks[i].info);
}


/* prints task in activity message, command d*/
void printTaskActivity(int i){

    printf("%d %d %s\n",tasks[i].id,tasks[i].instant,tasks[i].info);
}

/* prints task in alphabetical order*/
void printSortAlphabetical(){
    int i;

    for(i = 0; i < tsk_counter; i++){
            auxCMD[i] = i; /*add values to an array of tasks indexes*/
    }
    mergesort(auxCMD,aux,ZERO,tsk_counter - ONE,ONE);
    for(i = 0; i < tsk_counter; i++){ /*output format: command l*/
            printListTask(auxCMD[i]);
    }
}

/* checks conditions associated with command l*/
void listTasksValid(int c, int i,int k){
    if(!(isspace(c))){ /*checks for characters != whitespace*/
        if(isTaskinSystem((c - ZEROCHAR)) == true){
            listby_id[i] = c - ZEROCHAR;
            /*lists tasks by order of the given ids*/
            printListTask(listby_id[i]-ONE);
            i++;
            k++;      
            }
            else{
                printf("%s\n",TASKFSYSTEM);
            }  
    }
}

/*  creates a new task*/
void createTask(int duration , char info[]){

    tasks[tsk_counter].duration = duration;
    tasks[tsk_counter].id = tsk_counter + ONE;
    tasks[tsk_counter].instant = ZERO;
    strcpy(tasks[tsk_counter].info,info);
    strcpy(tasks[tsk_counter].activity.name,STAGE1);
    printf("%s %d\n",TASK,tasks[tsk_counter].id);
    tsk_counter++;
}

/* creates a new user*/
void createUser(char user[]){

    strcpy(users[usr_counter].name,user); 
    usr_counter++;      /*increases the number of users in the system*/
}

/*  checks conditions associated with command u */
void createUserValid(char user[]){

    if(isUserinSystem(user)){  /*tests for errors*/
            printf("%s\n",USERINSYSTEM);
    }
    else if(usr_counter > USERMAX){
        printf("%s\n",MAXUSERS);
    }
}

/*  tests for errors associated with command a or
    creates a new activity*/
void testCreateActivity( char activity[]){

    if(isActivityInfoDuplicated(activity)){
        printf("%s\n",ACTDUPLICATED);
    }
    else if(testActivityLower(activity) == true){
        printf("invalid description");
    }
    else if(act_counter > ACTIVITYMAX ){
        printf("%s\n",MAXACT);
    }
    else{             /*creates a new activity*/
        strcpy(activities[act_counter].name,activity);
        act_counter++;/*increases the number of activities in the system*/
    }
}

/*  tests for erros associated with command m or moves to 
    a given activity*/
void testMoveTask(int id,char user[], char activity[]){

    int slack,duration;
    slack = duration = 0;

    if(isTaskinSystem(id) == false){ 
        printf("%s\n",TASKFSYSTEM);return;
    }
    else if(taskAlreadyStarted(id,activity)){
        printf("%s\n",TASKTSTARTED);return;
    }
    else if(isUserinSystem(user) == false){
        printf("%s\n",USERFSYSTEM);return;
    }
    else if((isActivityinSystem(activity) == false)){
        printf("%s\n",ACTFSYSTEM);return;
    }
    if(tasks[id-ONE].instant == ZERO){
            /*updates the starting time of a given activity*/
            tasks[id-ONE].instant = clock;
            strcpy(tasks[id-ONE].user.name,user); 
            strcpy(tasks[id-ONE].activity.name,activity);
            duration = clock - tasks[id - ONE].instant;
            slack = duration -  tasks[id - ONE].duration;
            if(taskDone(activity)){
                printf("%s=%d %s=%d\n",DURATION,duration,SLACK,slack);
            }
    }
    else if(taskDone(activity)){
        duration = clock - tasks[id - ONE].instant;
        slack = duration -  tasks[id - ONE].duration; 
        printf("%s=%d %s=%d\n",DURATION,duration,SLACK,slack);
        strcpy(tasks[id-ONE].activity.name,activity);   
    }
    else{
          strcpy(tasks[id-ONE].user.name,user);
            strcpy(tasks[id-ONE].activity.name,activity);
    }
}

/* saves the id of tasks associated with the given activity*/
void processActivity(char activity[],int counter){

    int i;

    for(i = 0; i < tsk_counter; i++){/*checks for activities equal to the given*/
        if(strcmp(tasks[i].activity.name,activity) == false){
            listby_idCMDd[i] = tasks[i].id;/*saves the id of tasks which the
            associated activity is the given one*/
            counter++; /* counts the number occurrences of tasks associated 
            with the given activity*/
        }
    }
    for(i = 0; i < tsk_counter; i++){
            auxCMD[i] = i; /*add values to an array of tasks indexes*/
    }
    mergesort(auxCMD,aux,ZERO,counter -ONE,ZERO);
    for(i = 0; i < counter; i++){
        printTaskActivity(auxCMD[i]);
    }
}

/*---------------------------------------------------------------------------*/

/*
 *  Function: addTaskToSystem
 * 
 *  brief: adds a new task to the system
 *  reads: t <duration> <description>
 *  outputs: task <id> where <id> is the identifier of the created task
 * 
 *  errors: too many tasks
 *          duplicate description
 *          invalid duration
 * 
 *  command: t
 */

void addTaskToSystem(){    

    int duration;
    char info[TASKINFO];
    scanf("%d",&duration);
    getchar();             /*removes the first character after the duration*/
    scanf("%[^\n]s",info); /*the description cannot start with a whitespace*/
  
    if(!(info[0] == NUL) && nonNegativeIntChecker(duration)){
        if(tsk_counter > TASKSMAX){
            printf("%s\n",MAXTASKS);
        }
        else if(isTaskInfoDuplicated(info)){
            printf("%s\n",INFODUPLICATED);
        }
        else{
            /*creates a new task */
            createTask(duration,info);
        }       
    }
    else{
        printf("%s\n",INVALIDTIME);
    }
}

/*
 *  Function: listTask
 * 
 *  brief: lists tasks
 *  reads: l [<id> <id> ...]
 *  outputs: <id> <activity> #<duration> <description>
 * 
 *  errors: <id>: no such task
 *          
 *  command: l
 */
void  listTask(){ 

    int c,i,k,savechar,testchar;
    k = ZERO, i = ONE;

     /*removes the first whitespace/ newline character immediately*/ 
    testchar = getchar();  /*after the l command*/

    if(testchar == NLINE){/*lists tasks by alphabetical order of description*/
        printSortAlphabetical(ZERO);
    }
    else{ /*list by order of the given ids*/
  /*converts to the correspondent integer and checks if task in the system*/
            /*saves the character after the whitespace to use later*/  
        savechar = getchar();                    
        if(isTaskinSystem((savechar - ZEROCHAR)) == true){
             listby_id[ZERO] = (savechar - ZEROCHAR);
              /*lists tasks by order of the given ids*/
             printListTask(listby_id[ZERO]-ONE);
             k++;
        }
        else{
            printf("%s\n",TASKFSYSTEM);
            i = ZERO;
        }
        while((c = getchar())!= '\n' && c != EOF){
            listTasksValid(c,i,k);
        }
    }
}

/*
 *  Function: advanceTimeSystem
 * 
 *  brief: advances the system time
 *  reads: n <duration>
 *  outputs: <instant> where <instant> is the new current time.
 * 
 *  errors: invalid time
 *          
 *  command: n
 */

void  advanceTimeSystem(){

    int duration;
    scanf("%d",&duration);

    if(!nonNegativeIntChecker(duration)){
        printf("%s\n",NONVALIDTIME);
    }
    else{
        clock += duration;
        printf("%d\n",clock);   
    }
}

/*
 *  Function: addUserListUser
 * 
 *  brief: adds or lists users
 *  reads: u [<user>]
 *  outputs: lists users by insertion order,one per line or nothing if
 *  an user is created
 * 
 *  errors: user already exists
 *          too many users
 *          
 *  command: u
 */

void  addUserListUser(){       

    char user[USERNAME];
    int i = ZERO,j,savechar,c;     
    
    /*saves the character after the whitespace to use later*/  
    savechar = getchar();

    /*checks whether or not the saved character is a newline */
    if(savechar != NLINE){/*verifies if we're dealing with a list users or a
                              create user command*/
        while ((c=getchar())!='\n' && c!=EOF && c!=' '&& i < USERNAME){
            user[i++] = c;
        }
        user[i] = NUL;
        createUserValid(user);/*tests for errors*/
        createUser(user);/*creates a new user*/
    }
    else{   /*lists all users in the system by insertion order*/
            for(j = 0; j < usr_counter; j++){
                printf("%s\n",users[j].name);
        }
    } 
}


/*
 *  Function: moveTask
 * 
 *  brief: moves a task from an activity to another
 *  reads: m <id> <user> <activity>
 *  outputs: duration=<timesf> slack=<slack> 
 * ;timesf: time spent throughout stage1 to stage 3
 * ;slack: timesf - expected time 
 *  
 *  errors: no such task
 *          task already started
 *          no such user
 *          no such activity
 *          
 *  command: m
 */

void moveTask(){

    int id;
    char user[USERNAME], activity[ACTIVITYINFO];

    scanf("%d %s %[^\n]s",&id,user,activity);
    /*tests for errors and moves a task if possible*/
    testMoveTask(id,user,activity);
}

/*
 *  Function: listTaskActvity
 * 
 *  brief: lists all tasks in a given activity
 *  reads: d <ativity>
 *  outputs: <id> <starting time> <description>
 *                                        
 *  errors: no such activity
 *          
 *  command: d
 */

void listTaskActvity(){     

    char activity[ACTIVITYINFO];
    int counter = ZERO;
    /*removes the first white space immediately after the u command */    
    getchar();   

    scanf("%[^\n]s",activity);
    /*tests for errors*/
    if(isActivityinSystem(activity)== false){
        printf("%s\n",ACTFSYSTEM);
    }
    else{
        processActivity(activity,counter);
    }
}

/*
 *  Function: addOrListActivity
 * 
 *  brief: adds an activity or lists all
 *  reads: a [<activity>]
 *  outputs: lists name of activities by insertion order or nothing 
 *  if an activity is created
 *                                         
 *  errors: duplicate activity
 *          invalid description
 *          too many activities
 *          
 *  command: a
 */

void addOrListActivity(){  

    int savechar,c,i= ZERO,j;
    char activity[ACTIVITYINFO];

    savechar = getchar();  

    if(savechar != NLINE){/*verifies if we're dealing with a list activities
                             or a create activity command*/
        /*saves the first non whitespace character to an auxiliary array*/
	    while ((c=getchar())!='\n' && c!=EOF && i < ACTIVITYINFO){
            activity[i++] = c; 
        }
        activity[i] = NUL;
        testCreateActivity(activity);
    }
    else{  
                   /*lists all activities in the system by insertion order*/
        for(j = 0; j < act_counter; j++){
            printf("%s\n",activities[j].name);
        }
    }

}

/*  sorting*/


/*
 * algorithm of choice: mergesort, top-down mergesort
 * time complexity: N log N 
 * 
 * sorts a file of N elements in time proportional to N log N
 * no matter the input
 * 
 * 
 */

void mergesort(int a[],int aux[],int l, int r,int sortformat){

	int m = (r + l) / 2;
	if (r <= l) return;

	mergesort(a,aux, l, m,sortformat);  /* sorts the left part of the array*/
	mergesort(a,aux, m + 1, r,sortformat);/*sorts the right part of the array*/
	merge(a,aux, l, m, r,sortformat);  /*merges both sorted arrays into 
                                       one sorted array */
}

void merge(int a[],int aux[],int l, int m, int r,int sortformat){

	
    int i, j, k;
    for (i = m + 1; i > l; i--)  /*builds the aux array */
	    aux[i - 1] = a[i - 1];
    for (j = m; j < r; j++)
	    aux[r + m - j] = a[j + 1];
	                        /*sorts the aux array*/
    if(sortformat == ONE){ /* sorts alphabetically*/
        for (k = l; k <= r; k++){
            if(cmpInfo(aux[j],aux[i]))
	    	    a[k] = aux[j--];
	        else
	  	    a[k] = aux[i++];
        }
    }
    else{/*sorts by starting time**/
        for(k = l;k <= r; k++){
            if(cmpStartingTime(aux[j],(aux[i])))
                a[k] = aux[j--];
            else if(cmpStartingTime(aux[j],(aux[i])) == false){
                a[k] = aux[i++];
            }else if(cmpStartingTime(aux[j],(aux[i])) == THREE){
                if(cmpInfo(aux[j],aux[i])) /*in a tie situation the tiebreaker*/
			        a[k] = aux[j--];       /*is the task's description*/
		        else
			        a[k] = aux[i++];
            }
        }
    }
}

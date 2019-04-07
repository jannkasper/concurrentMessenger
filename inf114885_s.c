#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
int amountUser = 10;
int topicLimit = 10;


int findUser (int idCopy, char nameCopy[10], int idS [amountUser], char nameS[amountUser][10], int activeS[amountUser]) {
        for (int i = 0; i<amountUser; i++) {
            if(idS[i] == 0) {
                return i;
                
            } else if (idS[i] == idCopy && strcmp(nameCopy, nameS[i])==0) {
                return i;
            };
        };
        return -1;    
}

int findUserForBan (char nameCopy[10], int idS [amountUser], char nameS[amountUser][10]) {
        for (int i = 0; i<amountUser; i++) {
            if (strcmp(nameCopy, nameS[i])==0) {
                return i;
            };
        };
        return -1;    
}
//-------- CHECK IF USER1 BANNED MESSAGE FROM USER2  --------
int checkBanList(int id, int banUserS[amountUser-1])
{
    for(int j = 0; j<(amountUser-1); j++ )
    {
        if(banUserS[j] == id) { return 1;}
    }
    return 0;
}
//-------- USER ADD SUBSCRIPTION FROM LIST  --------
int addTopic (int numberUser, char topic, char topicS[amountUser][topicLimit]) {
    for (int i = 0; i<topicLimit; i++) {
        if(topicS[numberUser][i] == topic)
            return -2;
        if (topicS[numberUser][i] <32 || topicS[numberUser][i] > 126 ) {
            return i;
        }
    }
    return -1;
}
//-------- USER DELETE SUBSCRIPTION FROM LIST  --------
int deleteTopic (int numberUser, char topic, char topicS[amountUser][topicLimit]) {
    for (int i = 0; i<topicLimit; i++) {
        if(topicS[numberUser][i] == topic)
            return i;
        if (topicS[numberUser][i] == '\0' ) {
            printf("%d \n", i);
            return -2;
        }
    }
    return -1;
}
//-------- CHECK IF USER HAVE SUBSCRIPTION --------
int findTopic (int numberUser, char topic, char topicS[amountUser][topicLimit]) {
    for (int i = 0; i<topicLimit; i++) {
        if(topicS[numberUser][i] == topic)
            return i;
        if (topicS[numberUser][i] <32 || topicS[numberUser][i] > 126 ) {
            return -1;
        }
    }
    return -1;
}
//-------- READ USERS AND SUBSCRIPTION FROM FILE --------
void readFile (char nameS[amountUser][10], int idS[amountUser], char topicS[amountUser][topicLimit])
{
    int wczytaj = open ("userList.txt",O_RDONLY, 0777);
    if(wczytaj == -1)
   {
      printf("File error!");   
      //exit(1);
   }
    printf("\n ---USERS FROM FILE---\n");
    int position = 0;  
    int sz;
    char w;
    while((sz = read(wczytaj,&w,1))>0)
    {   //write(zapisz,&w,sizeof(w));
        int letter = 0;
        memset(nameS[position], 0, sizeof nameS[position]);
        while (w != 124 && sz>0)
        {
            nameS[position][letter] = w;
            sz = read(wczytaj,&w,1);
            letter++;
        }
        sz = read(wczytaj,&w,1);
        char number[10];
        letter =0;
        while (w != 124 && sz>0)
        {
            number[letter] = w;
            sz = read(wczytaj,&w,1);
            letter++;
        }
        idS[position] = atoi(number);
        memset(number, 0, sizeof number);
        
        sz = read(wczytaj,&w,1);
        letter =0;
        memset(topicS[position], 0, sizeof topicS[position]);
        while (w!=124 && sz>0)
        {
            topicS[position][letter] = w;
            sz = read(wczytaj,&w,1);
            letter++;
        }
        if (idS[position] != 0 )
        {
        printf("name: %s\tid: %d\ttopics: %s \n",nameS[position], idS[position], topicS[position]);
        }
        position++;
    }
}
//-------- SAVE USERS AND SUBSCRIPTION IN FILE --------
void saveFile (char nameS[amountUser][10], int idS[amountUser], char topicS[amountUser][topicLimit])
{
    int zapisz = open ("userList.txt",O_WRONLY, 0777);
    if(zapisz == -1)
   {
      printf("File error!");   
      //exit(1);
   }
   int i =0;
   char extra = '|';
   while(idS[i] != 0)
   {
       char number[10];
       memset(number, 0, sizeof number);
       sprintf(number, "%d", idS[i]);
       write(zapisz,&nameS[i],10);
       write(zapisz,&extra,1);
       write(zapisz,&number,10);
       write(zapisz,&extra,1);
       write(zapisz,&topicS[i],10);
       write(zapisz,&extra,1);
       memset(number, 0, sizeof number);
       i++;
   }
}
int main (int argc, char *argv[]){
    int id = msgget(1000,0777|IPC_CREAT);
    pid_t childPID = fork();
    if (childPID == 0)
    {
        printf("!!!---PUSH ENTER IN ANY MOMENT TO FINISH WORK OF SERVER---!!!");
        struct msgbuf{
            long typ;
            char topic;
            char text[100];
            int id;
            int priority;
            char name[10];
        }msgS;
        
        char nameS[amountUser][10];
        int idS [amountUser];
        char topicS [amountUser][topicLimit];
        int activeS [amountUser];
        int banUserS [amountUser][amountUser-1];
        for (int i =0; i<amountUser; i++) 
        {
            memset(nameS[i], 0, sizeof nameS[i]);
            memset(topicS[i], 0, sizeof topicS[i]);
            memset(banUserS[i], 0, sizeof banUserS[i]);
            idS[i] = 0;
            activeS[i] = 0;
        }
        readFile(nameS, idS, topicS);
        if (fork() == 0)
        {
        while(1){
            //-------- MAKE USER ACTIVE ON LIST --------
            if (msgrcv(id, &msgS, sizeof(msgS)-sizeof(long), 1, IPC_NOWAIT) != -1 )
            {
                int position = findUser (msgS.id, msgS.name,  idS, nameS, activeS);
                idS[position] = msgS.id;
                strcpy(nameS[position], msgS.name);
                activeS[position] = 1;
                printf("\n ---NEW USER ONLINE---\n id: %d name: %s\n",msgS.id, msgS.name);
                saveFile(nameS, idS, topicS);
                    
            }
            //------- ADD SUBSTRIPTION FOR USER -------
            if (msgrcv(id, &msgS, sizeof(msgS)-sizeof(long), 2, IPC_NOWAIT) != -1 )
            {
                int numberUser= findUser (msgS.id, msgS.name,  idS, nameS, activeS);
                int numberTopic = addTopic(numberUser, msgS.topic, topicS);
                if (numberTopic == -2)
                {
                    const char *fail= "FAIL! SUBSCRIPTION EXIST";
                    strcpy (msgS.text, fail);
                    int user = msgget(msgS.id,0777|IPC_CREAT);
                        msgS.typ=5;
                    msgsnd(user, &msgS, sizeof(msgS)-sizeof(long),IPC_NOWAIT);
                    
                }
                if (numberTopic >=0)
                {
                    topicS[numberUser][numberTopic] = msgS.topic;
                    printf("\n ---USER ADD TOPIC---\n name: %s topic: %c\n",nameS[numberUser], topicS[numberUser][numberTopic]);
                    //printf("TOPIC %c \n", topicS[numberUser][numberTopic]);
                    saveFile(nameS, idS, topicS);
                }
            }
            if (msgrcv(id, &msgS, sizeof(msgS)-sizeof(long), 6, IPC_NOWAIT) != -1 )
            {
                int numberUser= findUser (msgS.id, msgS.name,  idS, nameS, activeS);
                int numberTopic = deleteTopic(numberUser, msgS.topic, topicS);
                if (numberTopic == -2)
                {
                    const char *fail= "FAIL! SUBSCRIPTION DOESNT EXIST";
                    strcpy (msgS.text, fail);
                    int user = msgget(msgS.id,0777|IPC_CREAT);
                        msgS.typ=5;
                    msgsnd(user, &msgS, sizeof(msgS)-sizeof(long),IPC_NOWAIT);
                    
                }
                if (numberTopic >=0)
                {
                    topicS[numberUser][numberTopic] = '\0';
                    for (int numberTopicClean = numberTopic; numberTopicClean<topicLimit; numberTopicClean++)
                    {
                        if (topicS[numberUser][numberTopicClean+1] == '\0') {
                            topicS[numberUser][numberTopicClean] = '\0';
                            break;
                        }
                      topicS[numberUser][numberTopicClean] = topicS[numberUser][numberTopicClean+1];  
                    }
                    printf("\n ---USER DELETE TOPIC---\n name: %s topic: %c\n",nameS[numberUser], msgS.topic);
                    //printf("TOPIC %c \n", topicS[numberUser][numberTopic]);
                    saveFile(nameS, idS, topicS);
                }
            }
            //------- SEND MESSAGE FOR USERS WHO SUBSCRIBE -------
            if (msgrcv(id, &msgS, sizeof(msgS)-sizeof(long), 3, IPC_NOWAIT) != -1 )
            {
                int sendUser = findUser (msgS.id, msgS.name,  idS, nameS, activeS);
                for (int i = 0; i <amountUser; i++)
                {
                    int check = checkBanList(msgS.id, banUserS[i]);
                    if (check)
                    { printf("\n ---MESSAGE BLOCKED---\n user: %s blocked message from user: %s id: %d\n",nameS[i], msgS.name, msgS.id);}
                    if(i == sendUser || check || activeS[i] == 0) 
                        continue;
                    if(findTopic(i,msgS.topic, topicS) >=0)
                    {
                        int user = msgget(idS[i],0777|IPC_CREAT);
                        msgS.typ= msgS.priority;
                        msgsnd(user, &msgS, sizeof(msgS)-sizeof(long),IPC_NOWAIT);
                        printf("\n ---NEW MESSAGE---\n show user: %s topic: %c message: %s\n",nameS[i] ,msgS.topic, msgS.text);
                        
                    }
                }
                    //printf("%c\n",msgS.topic);
                    //printf("%s\n",msgS.text);
            }
            //------- BLOCK MESSAGES FROM USER -------
            if (msgrcv(id, &msgS, sizeof(msgS)-sizeof(long), 4, IPC_NOWAIT) != -1 )
            {
                int banUser = findUserForBan(msgS.text,  idS, nameS);
                int  userRequest= findUser (msgS.id, msgS.name,  idS, nameS, activeS);
                if (banUser>=0) {
                    for (int i =0; i <amountUser-1; i++)
                        if(banUserS[userRequest][i] <= 0) {
                            banUserS[userRequest][i] = idS[banUser];
                            printf("\n ---USER BLOCKED---\n user: %s blocked user: %s id:%d \n",msgS.name, nameS[banUser], idS[banUser]);
                            break;
                        }
                }
                    
                    
            }
            
            
        }
        }
    }
    char endChar;
    scanf("%c",&endChar);
    msgctl(id, IPC_RMID, NULL);
    kill(childPID,SIGTERM);
    printf("!!!---SERVER FINISHED WORK---!!!");
    return 0;
}



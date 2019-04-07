#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
int main (int argc, char *argv[]){
    int id = msgget(1000,0777|IPC_CREAT);
    struct msgbuf{
        long typ;
        char topic;
        char text[100];
        int id;
        int priority;
        char name[10];
    }msgU;
    
    //------------- ENTRY ID -------------
    int repeats =0;
    printf("Entry ID: ");
    while(((scanf("%d", &msgU.id) == 0) || (msgU.id <= 0)) && (msgU.id != '\n')){
    while(getchar() !='\n');
    repeats++;
    if(repeats == 3) {
        printf("Banned: wrong ID");
        return 0;
    }
    printf("Entry ID [repeat]: ");
    }
    for (int i =0; i<10; i++) { msgU.name[i] = '\0';}
    //------------- ENTRY NAME -------------
    printf("Entry username: ");
    repeats =1;
    int check = 1;
    while (check) {
        scanf("%s", msgU.name);
        check =0;
        if (repeats == 3) {
            printf("Banned: wrong name");
            return 0;
        }
        for (int i =0; i<10; i++)
        {
            if ( !((msgU.name[i]>64 && msgU.name[i]<91) || (msgU.name[i]>96 && msgU.name[i]<123) || msgU.name[i] == '\0'))
            {
                check =1;
                repeats++;
                printf("Entry username: ");
                memset(msgU.name,0,strlen(msgU.name));
                break;
            }
        }
    
        
        
    }
    //-------- USER INFO TO SERVER --------
    msgU.typ=1;
    msgsnd(id, &msgU, sizeof(msgU)-sizeof(long),IPC_NOWAIT);
    
    //------------ ASYNCHRONOUS ------------
    int asynchronous = 0;
    printf("Enable asynchronous [1/0]: ");
    scanf("%d", &asynchronous);
    int idk = msgget(msgU.id,0777|IPC_CREAT);
    pid_t childPID = fork();
    //------ CHILD PROCESS FOR ASYNCHRONOUS ANSWERS ------
    if (childPID == 0)
    {
        while(1)
        {
            for (int priorityLoop = 100; priorityLoop<=110; priorityLoop++) 
            {
                if (asynchronous == 1)
                {
                    if (msgrcv(idk, &msgU, sizeof(msgU)-sizeof(long), priorityLoop, IPC_NOWAIT) != -1)
                    {

                        //if(!strcmp(msgU.text,"exit"))
                        //break;
                        printf("\n ---NEW MESSAGE---\n topic %c : %s\n",msgU.topic, msgU.text);
                        fflush(stdout);
                        printf(" Select option: ");
                    }
                }
                if (msgrcv(idk, &msgU, sizeof(msgU)-sizeof(long), 5, IPC_NOWAIT) != -1 )
                {

                    //if(!strcmp(msgU.text,"exit"))
                    //break;
                    printf("\n ---FAIL---\n %s\n", msgU.text);
                    fflush(stdout);
                    printf(" Select option: ");

                }
            }
        }
    } else {
        //------------ FUNCITONS LOOP ------------
        int check = 1;
        printf("---CONTROL PANEL--- \n [1] add subscription\n [2] send new message \n [3] open menu \n [4] block user \n [5] show messages \n [6] delete subscription\n [7] exit \n");
        while (check) {
            int priorityLoop;
            printf("Select option: ");
            int option;
            scanf(" %d",&option);
            switch(option) {
                case 1: //------- ADD SUBSTRIPTION -------
                    printf("Entry new subscription: ");
                    scanf(" %c",&msgU.topic);
                    msgU.typ=2;
                    msgsnd(id, &msgU, sizeof(msgU)-sizeof(long),IPC_NOWAIT);
                    break;
                case 2: //------- SEND MESSAGE -------
                    printf("Entry subscription: ");
                    scanf(" %c",&msgU.topic);
                    printf("Entry priority[0-10]: ");
                    scanf(" %d",&msgU.priority);
                    msgU.priority += 100;
                    printf("Entry message: ");
                    scanf(" %s",msgU.text);
                    msgU.typ=3;
                    msgsnd(id, &msgU, sizeof(msgU)-sizeof(long),IPC_NOWAIT);
                    break;
                case 3: //------- SHOW PANEL -------
                    printf("---CONTROL PANEL--- \n [1] add subscription\n [2] send new message \n [3] open menu \n [4] block user \n [5] show messages \n [6] delete subscription\n [7] exit \n");
                    break;
                case 4: //------- BLOCK USER -------
                    printf("Entry user login: ");
                    scanf(" %s",msgU.text);
                    msgU.typ=4;
                    msgsnd(id, &msgU, sizeof(msgU)-sizeof(long),IPC_NOWAIT);
                    break;
                case 5: //------- GET SYNCHRONOUS MESSAGE -------
                    priorityLoop =100;
                    while (priorityLoop <= 110) 
                    {
                        //int idk = msgget(msgU.id,0777|IPC_CREAT);
                        if (msgrcv(idk, &msgU, sizeof(msgU)-sizeof(long), priorityLoop, IPC_NOWAIT) != -1 )
                        {
                            printf("\n ---NEW MESSAGE---\n topic %c : %s\n",msgU.topic, msgU.text);
                            continue;
                        }
                        if (msgrcv(idk, &msgU, sizeof(msgU)-sizeof(long), 5, IPC_NOWAIT) != -1 )
                        {
                            printf("\n ---FAIL---\n %s\n", msgU.text);
                        }
                        priorityLoop++;
                    }
                    break;
                case 6:
                    printf("Entry existing subscription: ");
                    scanf(" %c",&msgU.topic);
                    msgU.typ=6;
                    msgsnd(id, &msgU, sizeof(msgU)-sizeof(long),IPC_NOWAIT);
                    break;
                case 7: //------- QUIT PROGRAM -------
                    check = 0;
                    msgctl(idk, IPC_RMID, NULL);
                    kill(childPID,SIGTERM);
                    break;
                    
            }
        }
    }

    return 0;
}

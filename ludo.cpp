// Zuraiz Zahoor Ajaz i191793 AI-K
// Bilal Ahmad i191874 AI-K

#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h>
#include <string.h> 
#include <stdio.h>
#include <time.h>
#include<semaphore.h>

using namespace std;

sem_t sema[6];
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
int dice;
int bari=0;
int savepos[] = {16,24,29,37,42,50,55,63};
char playerCharacterList[] = {'A','B','C','D'};
int homepos[] = {16,29,42,55};
int criticalHomePos[]={66,27,40,53};
int criticalHomeTurnPos[]={68,73,78,83};
int winPos []= {72,77,82,87};

int posA[] = {88,89,90,91};
int posB[] = {92,93,94,95};
int posC[] = {96,97,98,99};
int posD[] = {100,101,102,103};

int homecounter=0;
int noOfPlayers=4;
string* playerNameList ;
string* wonPlayerList ;
int wonPlayerListIndex=0;
char* ludo=new char[104]; 

pthread_t player_tid[4];
pthread_t master_tid;
class playerClass 
{ 
    public: 
    char token[4]; // Types of token: A B C D 
    int CurrentPos[4]; 
    int StartPos[4];
    int noOfMoves[4];
    bool home[4];
    int noOfTokens;
    int noOfAliveTokens;
    bool playerWon=false;
    int noOfWonTokens;
    int playerIndex;       
    string playerName; 
}; 
    
    
playerClass A,B,C,D;

struct parameterThreads{
    playerClass* player;
};

void board(char* const ludo){
    cout<<" ____________________________________________________________"<<endl; 
    cout<<"|          |          |_"<<ludo[26]<<"__|_"<<ludo[27]<<"__|_"<<ludo[28]<<"__|           |           |"<<endl; 
    cout<<"|    "<<ludo[0]<<"     |    "<<ludo[1]<<"     |_"<<ludo[25]<<"__|_"<<ludo[73]<<"__|_"<<ludo[29]<<"__|    "<<ludo[4]<<"      |    "<<ludo[5]<<"      |"<<endl; 
    cout<<"|__________|__________|_"<<ludo[24]<<"__|_"<<ludo[74]<<"__|_"<<ludo[30]<<"__|___________|___________|"<<endl; 
    cout<<"|          |          |_"<<ludo[23]<<"__|_"<<ludo[75]<<"__|_"<<ludo[31]<<"__|           |           |"<<endl; 
    cout<<"|    "<<ludo[2]<<"     |    "<<ludo[3]<<"     |_"<<ludo[22]<<"__|_"<<ludo[76]<<"__|_"<<ludo[32]<<"__|    "<<ludo[6]<<"      |    "<<ludo[7]<<"      |"<<endl; 
    cout<<"|__________|__________|_"<<ludo[21]<<"__|_"<<ludo[77]<<"__|_"<<ludo[33]<<"__|___________|___________|"<<endl; 
    cout<<"|_"<<ludo[67]<<"_|_"<<ludo[16]<<"_|"<<ludo[17]<<"_|"<<ludo[18]<<"_|"<<ludo[19]<<"_|_"<<ludo[20]<<"_|    "<<ludo[92]<<" "<<ludo[93]<<" "<<ludo[94]<<" "<<ludo[95]<<"     |_"<<ludo[34]<<"_|_"<<ludo[35]<<"_|_"<<ludo[36]<<"_|_"<<ludo[37]<<"_|"<<ludo[38]<<"_|_"<<ludo[39]<<"_|"<<endl; 
    cout<<"|   |   |  |  |  |   |                |   |   |   |   |  |   |"<<endl; 
    cout<<"| "<<ludo[66]<<" | "<<ludo[68]<<" |"<<ludo[69]<<" |"<<ludo[70]<<" |"<<ludo[71]<<" | "<<ludo[72]<<" |  "<<ludo[88]<<ludo[89]<<ludo[90]<<ludo[91]<<"    "<<ludo[99]<<ludo[98]<<ludo[97]<<ludo[96]<<"  | "<<ludo[82]<<" | "<<ludo[81]<<" | "<<ludo[80]<<" | "<<ludo[79]<<" |"<<ludo[78]<<" | "<<ludo[40]<<" |"<<endl; 
    cout<<"|___|___|__|__|__|___|    "<<ludo[100]<<" "<<ludo[101]<<" "<<ludo[102]<<" "<<ludo[103]<<"     |___|___|___|___|__|___|"<<endl; 
    cout<<"|_"<<ludo[65]<<"_|_"<<ludo[64]<<"_|"<<ludo[63]<<"_|"<<ludo[62]<<"_|"<<ludo[61]<<"_|_"<<ludo[60]<<"_|________________|_"<<ludo[46]<<"_|_"<<ludo[45]<<"_|_"<<ludo[44]<<"_|_"<<ludo[43]<<"_|"<<ludo[42]<<"_|_"<<ludo[41]<<"_|"<<endl; 
    cout<<"|          |          |_"<<ludo[59]<<"__|_"<<ludo[87]<<"__|_"<<ludo[47]<<"__|           |           |"<<endl; 
    cout<<"|    "<<ludo[12]<<"     |    "<<ludo[13]<<"     |_"<<ludo[58]<<"__|_"<<ludo[86]<<"__|_"<<ludo[48]<<"__|    "<<ludo[8]<<"      |    "<<ludo[9]<<"      |"<<endl; 
    cout<<"|__________|__________|_"<<ludo[57]<<"__|_"<<ludo[85]<<"__|_"<<ludo[49]<<"__|___________|___________|"<<endl; 
    cout<<"|          |          |_"<<ludo[56]<<"__|_"<<ludo[84]<<"__|_"<<ludo[50]<<"__|           |           |"<<endl; 
    cout<<"|    "<<ludo[14]<<"     |    "<<ludo[15]<<"     |_"<<ludo[55]<<"__|_"<<ludo[83]<<"__|_"<<ludo[51]<<"__|    "<<ludo[10]<<"      |    "<<ludo[11]<<"      |"<<endl; 
    cout<<"|__________|__________|_"<<ludo[54]<<"__|_"<<ludo[53]<<"__|_"<<ludo[52]<<"__|___________|___________|";
    cout<<endl;
}
int takeOutTokenDisplay(playerClass& player){
    cout<<endl<<" takeOutTokenDisplay"<<endl;
    int k=1;
    for (int i = 0; i < player.noOfTokens; i++)
    {
        if(player.home[i] == true){
            ludo[player.CurrentPos[i]] = '0' + k; 
        }
        k++;
    }                
    board(ludo);
    cout<<endl<<" Which token you want to take out? (";
    for (int i = 0; i < k-1; i++){
        if(player.home[i] == true){
            cout<<i+1<<",";
        }
    }
    cout<<"): ";
    int input;
    // cin>>input;
    int randomNum =rand() % 4;;
    while(player.home[randomNum] != true){
        randomNum = rand() % 4;
    }
    input = randomNum + 1;
    cout<<input<<endl;
    input-=1;
    for (int i = 0; i < player.noOfTokens; i++)
    {
        if(player.home[i] == true){
            ludo[player.CurrentPos[i]] = player.token[i]; 
        }
    }
    return input;
}
void moveTokenCheck(playerClass& player,int input,int turn){
    cout<<" moveTokenCheck"<<endl;
    int t = player.CurrentPos[input]+turn;
    if (!(t==16 || t==24 || t==29 || t==37 || t==42 || t==50 || t==55 || t==63))
    {
        if (ludo[player.CurrentPos[input]+turn]!=' '  && ludo[player.CurrentPos[input]+turn]!='@' && ludo[player.CurrentPos[input]+turn]!=player.token[input] )
        {
            cout<<player.token[input]<<" killed "<<ludo[player.CurrentPos[input]+turn]<<endl;
            // sleep(3);
            int killedTokenIndex;
            // sleep(10);
            for (int i = 0; i < 4; i++)
            {
                if (ludo[player.CurrentPos[input]+turn] == playerCharacterList[i])
                {
                    int killedTokenIndex=i;
                    playerClass* p;
                    if (i==0)
                        p=&A;
                    else if(i==1)
                        p=&B;
                    else if(i==2)
                        p=&C;
                    else if(i==3)
                        p=&D;    
                    for (int i = 0; i < p->noOfTokens; i++)
                    {
                        if (p->CurrentPos[i] == player.CurrentPos[input]+turn)
                        {
                            cout<<"Killed here"<<endl;
                                
                            p->CurrentPos[i] = p->StartPos[i];
                            ludo[p->StartPos[i]] = p->token[i];
                            p->noOfMoves[i] = 0;
                            p->home[i] = true; 
                            p->noOfAliveTokens--;
                            // sleep(20);
                            break;
                        }
                    }
                    break;
                }
            }

        }
    }
    t = player.CurrentPos[input];
    if (t==16 || t==24 || t==29 || t==37 || t==42 || t==50 || t==55 || t==63)
    {
        ludo[player.CurrentPos[input]]='@';
    }
    // sleep(1);
}
void moveToken3(playerClass& player,int input,int turn,int i){
    cout<<" moveToken3"<<endl;
    moveTokenCheck(player,input,turn);
   
    player.CurrentPos[input] = i;
    player.noOfMoves[input]+=turn;
    ludo[player.CurrentPos[input]] = player.token[input];
}
void moveToken2(playerClass& player,int input,int turn){
    cout<<" moveToken2"<<endl;
    moveTokenCheck(player,input,turn);
    player.CurrentPos[input]+=turn;
    player.noOfMoves[input]+=turn;

    ludo[player.CurrentPos[input]] = player.token[input]; 
}
int moveTokenDisplay(playerClass& player){
    int k=1;
    cout<<endl<<" moveTokenDisplay"<<endl<<"No Of win :"<<player.noOfWonTokens<<endl;
    cout<<"No Of Alive :"<<player.noOfAliveTokens<<endl;
    for (int i = 0; i < 4; i++)
    {
        if(player.home[i]  != true && player.noOfMoves[i]  < 56 ){
            ludo[player.CurrentPos[i]] = '0' + k; 
        }
        k++;
    }                
    board(ludo);
    cout<<endl<<" Which token do you want to move? (";
    for (int i = 0; i < k-1; i++){
        if(player.home[i]  != true && player.noOfMoves[i]  < 56 ){
            cout<<i+1<<",";
        }
    }
    cout<<"): ";
    int input;
    // cin>>input;
    int randomNum =rand() % 4;;
    while(player.home[randomNum] == true || player.noOfMoves[randomNum]>=56){
        randomNum = rand() % 4;
    }
    input = randomNum+1;
    cout<<input<<endl;
    
    input-=1;
    for (int i = 0; i < player.noOfTokens; i++)
    {
        if(player.home[i]  != true && player.noOfMoves[i]  < 56 ){
            ludo[player.CurrentPos[i]] = player.token[i]; 
        }
    }  
    return input;
}
void moveTokenFun(playerClass& player,int input,int turn){
    ludo[player.CurrentPos[input]] = ' ';
    int t = player.CurrentPos[input];

    
    int ind = player.playerIndex;
    if (player.noOfMoves[input]+turn>56){
        cout<<"Move Not Allowed ! NoOfMoves: "<<player.noOfMoves[input]<<endl;
        // board(ludo);
        ludo[player.CurrentPos[input]] = player.token[input];
        return;
    }
    if (player.noOfMoves[input]+turn==56){
        switch (ind)
        {
            case 0:
                player.CurrentPos[input] = posA[input];
                break;
            case 1:
                player.CurrentPos[input] = posB[input];
                break;
            case 2:
                player.CurrentPos[input] = posC[input];
                break; 
            case 3:
                player.CurrentPos[input] = posD[input];
                break;     
            default:
                cout<<"Error at switch"<<endl;
                break;
        }
        player.noOfMoves[input]=56;
        ludo[player.CurrentPos[input]] = player.token[input];
        player.noOfWonTokens++;
        if (player.noOfWonTokens==4)
        {
            cout<<"Player Won!"<<endl;
            sem_wait(&sema[5]);
            player.playerWon=true;
            sem_post(&sema[5]);
            return;
        }
        
        return;
    }   
    if (ind != 0 )
    {
        if (player.CurrentPos[input]+turn>67 && player.CurrentPos[input]+turn<=73){
            int i = player.CurrentPos[input]+turn-67+homepos[0]-1;
            moveToken3(player,input,turn,i);
            return;
        }
    }

    if (player.noOfMoves[input]+turn>49 && player.noOfMoves[input]<=49)
    {
    
        if (player.CurrentPos[input]+turn > criticalHomePos[ind] && ind==0 )
        {
            int i = 1+player.CurrentPos[input]+turn-criticalHomePos[ind];
            moveToken3(player,input,turn,player.CurrentPos[input] + i);
            return;
        }
        if (player.CurrentPos[input]+turn > criticalHomePos[ind] )
        {
            int i = player.CurrentPos[input]-1+turn-criticalHomePos[ind]+criticalHomeTurnPos[ind];
            moveToken3(player,input,turn,i);
            return;
        }
    }
    
    // player.CurrentPos[input]+=turn;
    // player.noOfMoves[input]+=turn;

    // ludo[player.CurrentPos[input]] = player.token[input]; 
    moveToken2(player,input,turn);

}
void moveHome(playerClass& player,int input){
    ludo[player.CurrentPos[input]] = ' ';
    player.noOfMoves[input]=0;
    player.CurrentPos[input] = homepos[player.playerIndex];
    player.home[input] = false;
    ludo[player.CurrentPos[input]] = player.token[input]; 
    player.noOfAliveTokens++;
}
void* move(void * pk){
    // struct parameterThreads *ptr = (parameterThreads pk *) pk;
    struct parameterThreads *ptr = (struct parameterThreads *) pk;
    playerClass* player = ptr->player;
    if ( player->noOfWonTokens ==4)
    {
        // sem_post(&sema[0]);cout<<"\t\t\t\t Ludo UnLocked"<<endl;
        return pk;
    }
        sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked"<<endl;

    // playerClass player = *(playerClass*&)pk;
    
        int i=0;
        cout<<endl<<"*******************"<<endl;
        cout<<"   Player "<<player->token[i]<<" move!"<<endl;
        cout<<"*******************"<<endl;
        sem_wait(&sema[1]);cout<<endl<<"\t\t\t\t\t\t Dice Locked"<<endl;

        dice = rand() % 6+1;
        // dice = 6;//Delete this
        int turns[3]={0};
        turns[0] = dice;
        if ((player->noOfWonTokens == player->noOfAliveTokens) && player->noOfAliveTokens!=0 && turns[0]!=6 )
        {
            // cout<<endl<<"No Move!"<<"  noOFWon:"<<player->noOfWonTokens<<endl;
            cout<<endl<<"\t\t\t\t\t\t Dice UnLocked"<<endl;sem_post(&sema[1]);
            cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
            return pk;
        }
        if (player->noOfWonTokens == 4)
        {
            cout<<endl<<"\t\t\t\t\t\t Dice UnLocked"<<endl;sem_post(&sema[1]);
            cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
        }
        
        cout<<" Dice: ";
        cout<<dice;

        //  if not 6 and noOfAliveTokens == 0
        
        if(dice!=6 && player->noOfAliveTokens==0){
                cout<<" No Move "<<endl;
                cout<<endl<<"\t\t\t\t\t\t Dice UnLocked"<<endl;sem_post(&sema[1]);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                return pk;
        }

        
        
        //  if or not 6 6 6
        if(dice == 6 ){
            dice = rand() % 6+1;
            cout<<" "<<dice<<" ";
            turns[1] = dice;
            if(dice == 6 ){
                dice = rand() % 6+1;
                cout<<dice<<" ";
                turns[2] = dice;
                if(dice == 6){
                    cout<<"\tThree Sixes! No Move"<<endl;
                    cout<<endl<<"\t\t\t\t\t\t Dice UnLocked"<<endl;sem_post(&sema[1]);
                    cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                    return pk;
                }
            }
            cout<<endl;
        }
        cout<<endl<<"\t\t\t\t\t\t Dice UnLocked"<<endl;sem_post(&sema[1]);

        if(turns[0] == 6 ){
            if(player->noOfAliveTokens==0){
                player->CurrentPos[0] = homepos[player->playerIndex];
                player->noOfAliveTokens = 1;
                player->home[0] = false;
                ludo[player->StartPos[0]] = ' ';
                if(turns[1] != 6) {
                    moveToken2(*player,0,turns[1]);
                    cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                    return pk;
                }else if(turns[1] == 6){
                    cout<<" Do you want to take out another token? (y/n): ";
                    char input;
                    // cin>>input;
                    int randomNum = rand() % 2+1;
                    if (randomNum == 1)
                    {
                        input = 'y';
                    }else input = 'n';
                    
                    cout<<input<<endl;
                    moveToken2(*player,0,turns[2]);

                    if(input=='y' || input=='Y'){
                        moveHome(*player,1);
                    }else if(input=='n' || input=='N'){
                        moveToken2(*player,0,turns[1]);
                    }
                }
            }
            else if(player->noOfAliveTokens<=4 && player->noOfWonTokens!=4){
                int six=0;
                while(turns[six] == 6){
                    if(player->noOfAliveTokens == 4 && player->noOfWonTokens!=4){
                        int input = moveTokenDisplay(*player);
                        moveTokenFun(*player,input,turns[six]);
                        if(player->playerWon==true){
                            cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                            return pk ;
                        }
                    }
                    else
                    {
                        if (player->noOfAliveTokens<3)
                            cout<<" Do you want to take out another token? (y/n): ";
                        else 
                            cout<<" Do you want to take out token? (y/n): ";
                        
                        char input;//cin>>input;
                        int randomNum = rand() % 2+1;
                        if (randomNum == 1)
                        {
                            input = 'y';
                        }else input = 'n';
                        if ((player->noOfWonTokens==player->noOfAliveTokens) && player->noOfAliveTokens!=0)
                        {
                            input='y';
                        }
                        cout<<input<<endl;
                        if (player->noOfAliveTokens==4)
                        {
                            input='n';
                        }
                        

                        if((input=='y' || input=='Y')  ){
                            
                            int input2 = takeOutTokenDisplay(*player);
                            moveHome(*player,input2);
                        }else{
                            if (player->noOfWonTokens!=4)
                            {
                                int input2 = moveTokenDisplay(*player);
                                moveTokenFun(*player,input2,turns[six]);
                                if(player->playerWon==true){
                                    cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                                    return pk;
                                }
                            }                            
                        }
                    }
                    six++;
                }
                if (!(player->noOfWonTokens==player->noOfAliveTokens))
                {
                    int input = moveTokenDisplay(*player);
                    moveTokenFun(*player,input,turns[six]);   
                    if(player->playerWon==true){
                        cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                        return pk ;
                    }
                }
            }
            cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
            return pk;
        }
        else
        {
            cout<<endl;
            if (player->noOfAliveTokens>1 && player->noOfWonTokens!=4)
            {
                int input = moveTokenDisplay(*player);
                moveTokenFun(*player,input,turns[0]);
                if(player->playerWon==true){
                    cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                    return pk;
                }
            }else{
                moveTokenFun(*player,0,turns[0]);
                if(player->playerWon==true){
                    cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                    return pk;
                }
            }
        }
        cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
        return pk;
    
}
void initGame(playerClass& A,playerClass& B,playerClass& C,playerClass& D){
    int i; 
    int k=0;
    for(i=0;i<104;i++) {
        ludo[i]=' ';
        if (i==savepos[k])
        {
            ludo[i]='@';
            k++;
        }        
    }

    A.noOfTokens = 4;
    B.noOfTokens = 4;
    C.noOfTokens = 4;
    D.noOfTokens = 4;

    A.noOfAliveTokens = 0;
    B.noOfAliveTokens = 0;
    C.noOfAliveTokens = 0;
    D.noOfAliveTokens = 0;
    for(i=0;i<4;i++)      // Initialization of variables 
    { 
        A.token[i] = playerCharacterList[0];A.home[i] = true; 
        B.token[i] = playerCharacterList[1];B.home[i] = true; 
        C.token[i] = playerCharacterList[2];C.home[i] = true; 
        D.token[i] = playerCharacterList[3];D.home[i] = true; 
    } 
    A.CurrentPos[0]=0;  A.CurrentPos[1]=1; A.CurrentPos[2]=2; A.CurrentPos[3]=3; 
    B.CurrentPos[0]=4;  B.CurrentPos[1]=5; B.CurrentPos[2]=6; B.CurrentPos[3]=7; 
    C.CurrentPos[0]=8;  C.CurrentPos[1]=9; C.CurrentPos[2]=10;C.CurrentPos[3]=11; 
    D.CurrentPos[0]=12; D.CurrentPos[1]=13;D.CurrentPos[2]=14;D.CurrentPos[3]=15; 
    for(i=0;i<4;i++) 
    { 
        ludo[A.CurrentPos[i]]=A.token[i]; 
        ludo[B.CurrentPos[i]]=B.token[i]; 
        ludo[C.CurrentPos[i]]=C.token[i]; 
        ludo[D.CurrentPos[i]]=D.token[i]; 
    }
    
    A.StartPos[0]=0; A.StartPos[1]=1; A.StartPos[2]=2; A.StartPos[3]=3; 
    B.StartPos[0]=4; B.StartPos[1]=5; B.StartPos[2]=6; B.StartPos[3]=7; 
    C.StartPos[0]=8; C.StartPos[1]=9; C.StartPos[2]=10; C.StartPos[3]=11; 
    D.StartPos[0]=12; D.StartPos[1]=13; D.StartPos[2]=14; D.StartPos[3]=15; 

    A.playerIndex=0; 
    B.playerIndex=1; 
    C.playerIndex=2; 
    D.playerIndex=3;

    A.noOfWonTokens=0;
    B.noOfWonTokens=0;
    C.noOfWonTokens=0;
    D.noOfWonTokens=0;

sem_wait(&sema[5]);            
    A.playerWon =false;
    B.playerWon =false;
    C.playerWon =false;
    D.playerWon =false;
sem_post(&sema[5]);
    // sem_wait(&sema[4]);
    A.playerName=playerNameList[0];
    B.playerName=playerNameList[1];
    C.playerName=playerNameList[2];
    D.playerName=playerNameList[3];
    // sem_post(&sema[4]);
}
void display(char*& ludo){
    int i; 
    int k=0;
    for(i=0;i<104;i++) {
        ludo[i]=' ';
        if (i==savepos[k])
        {
            ludo[i]='@';
            k++;
        }        
    }
    for (int i = 0; i < A.noOfTokens; i++)
    {
        ludo[A.CurrentPos[i]]=A.token[i];
        ludo[B.CurrentPos[i]]=B.token[i];
        ludo[C.CurrentPos[i]]=C.token[i];
        ludo[D.CurrentPos[i]]=D.token[i];
    }
}
int main3(){
    
    for (int i = 0; i < 79; i++)
    {
        ludo[i] = '0' + i ;   
    }
    for (int i = 79; i < 104; i++)
    {
        ludo[i] = '0' + i - 79;   
    }
    board(ludo);
    cout<<endl;
    for (int i = 0; i < 104; i++)
    {
        if (i>79)
        {
            cout<<ludo[i]<<" : "<<i<<"\t";
            if(i%10 ==0 )
            cout<<endl;continue;
        }
        
        cout<<ludo[i]<<" : "<<ludo[i] - '0'<<"\t";
        if(i%10 ==0 )
            cout<<endl;
    }
}
void* master_Function(void* arg){
    	
    // struct parameterThreads *args = calloc (sizeof (struct parameterThreads), 1);
    struct parameterThreads *args1 = (parameterThreads *)malloc (sizeof (playerClass));
    struct parameterThreads *args2 = (parameterThreads *)malloc (sizeof (playerClass));
    struct parameterThreads *args3 = (parameterThreads *)malloc (sizeof (playerClass));
    struct parameterThreads *args4 = (parameterThreads *)malloc (sizeof (playerClass));

    cout<<endl<<"    *******************   Number of players: 4   *******************"<<endl;
    cout<<endl<<endl<<"  ----Moves-------------------------Ludo------------Dice----------"<<endl<<endl;
    sem_wait(&sema[4]);
    playerNameList = new string[noOfPlayers];
    cout<<"Enter First player name: ";
    // cin>>playerNameList[0];
    playerNameList[0] = "Zuraiz";
    cout<<playerNameList[0]<<endl;
    cout<<"Enter Second player name: ";
    // cin>>playerNameList[2];
    playerNameList[2] = "ALi";
    cout<<playerNameList[2]<<endl;
    playerNameList[1]="No One";
    playerNameList[3]="No One";
    cout<<" "<<playerNameList[0]<<"\tis A"<<endl;
    cout<<" "<<playerNameList[2]<<"\tis C"<<endl;
    sem_post(&sema[4]);

    // sleep(1);
    sem_wait(&sema[3]);
    wonPlayerList=new string[noOfPlayers];
    sem_post(&sema[3]);

    
    {
        
        // if(noOfPlayers < 2){
        //     cout<<"Wrong input"<<endl;
        //     exit(1);
        // }else if(noOfPlayers == 2){
        //     int temp = homepos[1];
        //     homepos[1] = homepos[2];
        //     homepos[2] =temp;

        //     char temp2 = playerCharacterList[1];
        //     playerCharacterList[1] = playerCharacterList[2];
        //     playerCharacterList[2] =temp2;
        // }else if(noOfPlayers == 3){
        //     int temp = homepos[1];
        //     homepos[1] = homepos[3];
        //     homepos[3] =temp;

        //     char temp2 = playerCharacterList[1];
        //     playerCharacterList[1] = playerCharacterList[3];
        //     playerCharacterList[3] =temp2;
        // }
    }
    sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked"<<endl;
    initGame(A,B,C,D);
    cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
    int i=5;
    sem_wait(&sema[5]);
    while (/*(A.playerWon==false || A.playerWon==false ||*/( C.playerWon==false || B.playerWon==false || D.playerWon==false) )
    {
        sem_post(&sema[5]);
        sem_wait(&sema[5]);
        if (C.playerWon == false)
        {
            args3->player = &C;
            pthread_create(&player_tid[2],NULL,move,args3);
            if (C.noOfWonTokens == 4)
            {
                C.playerWon = true;
                
                cout<<C.playerName<<endl;// sleep(10);
                string temp = C.playerName;
                // sem_wait(&sema[3]);
                wonPlayerList[0] = temp;
                pthread_cancel(player_tid[2]);
                // break;
                // sem_post(&sema[3]);
            }
            else if (C.noOfAliveTokens>0 && C.noOfWonTokens<4)
            {
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here"<<endl;
                display(ludo);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here2"<<endl;
                board(ludo);sleep(1);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
            }
        }
        sem_post(&sema[5]);

        //B
        sem_wait(&sema[5]);
        if (B.playerWon == false)
        {
            args2->player = &B;
            pthread_create(&player_tid[1],NULL,move,args2);
            if (B.noOfWonTokens == 4)
            {
                B.playerWon = true;
                
                cout<<B.playerName<<endl;// sleep(10);
                string temp = B.playerName;
                // sem_wait(&sema[3]);
                wonPlayerList[0] = temp;
                pthread_cancel(player_tid[1]);
                // break;
                // sem_post(&sema[3]);
            }
            else if (B.noOfAliveTokens>0 && B.noOfWonTokens<4)
            {
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here"<<endl;
                display(ludo);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here2"<<endl;
                board(ludo);sleep(1);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
            }
        }
        sem_post(&sema[5]);


        // A
        /*
        sem_wait(&sema[5]);
        if (A.playerWon == false)
        {
            args1->player = &A;
            pthread_create(&player_tid[0],NULL,move,args1);
            if (A.noOfWonTokens == 4)
            {
                A.playerWon = true;
                
                cout<<A.playerName<<endl;// sleep(10);
                string temp = A.playerName;
                // sem_wait(&sema[3]);
                wonPlayerList[0] = temp;
                pthread_cancel(player_tid[0]);
                break;
                // sem_post(&sema[3]);
            }
            else if (A.noOfAliveTokens>0 && A.noOfWonTokens<4)
            {
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here"<<endl;
                display(ludo);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here2"<<endl;
                board(ludo);sleep(0.5);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
            }
        }
        sem_post(&sema[5]);
        */
        //C
        sem_wait(&sema[5]);
        if (D.playerWon == false)
        {
            args4->player = &D;
            pthread_create(&player_tid[3],NULL,move,args4);
            if (D.noOfWonTokens == 4)
            {
                D.playerWon = true;
                
                cout<<D.playerName<<endl;// sleep(10);
                string temp = D.playerName;
                // sem_wait(&sema[3]);
                wonPlayerList[0] = temp;
                pthread_cancel(player_tid[3]);
                // break;
                // sem_post(&sema[3]);
            }
            else if (D.noOfAliveTokens>0 && D.noOfWonTokens<4)
            {
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here"<<endl;
                display(ludo);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
                sem_wait(&sema[0]);cout<<"\t\t\t\t Ludo Locked Here2"<<endl;
                board(ludo);sleep(1);
                cout<<"\t\t\t\t Ludo UnLocked"<<endl;sem_post(&sema[0]);
            }
        }
        sem_post(&sema[5]);
        i--;
    }   
    
    // cout<<endl<<endl<<" Game Over!\n "<<wonPlayerList[0]<<" Came First\n "<<wonPlayerList[1]<<" Came Second\n "
    // <<wonPlayerList[2]<<" Came Third\n "<<wonPlayerList[3]<<" Came Fourth"<<endl;
    pthread_exit(NULL);
}
int main(){
    // sleep(0.1);
    srand(time(NULL));
    sem_init(&sema[0], 0, 1);// Ludo
    sem_init(&sema[1], 0, 1);// Dice
    sem_init(&sema[2], 0, 1);// struct
    sem_init(&sema[3], 0, 1);// wonPlayerList
    sem_init(&sema[4], 0, 1);// PlayerList
    sem_init(&sema[5], 0, 1);// sem
    // initGame(B,A,C,D);
    // initGame(B,C,A,D);
    // initGame(B,C,D,A);
    // board(ludo);
    // cout<<endl;
    
    pthread_create(&master_tid,NULL,master_Function,NULL);

    // masterFunction(NULL);

    sem_destroy(&sema[0]);
    sem_destroy(&sema[1]);
    sem_destroy(&sema[2]);
    sem_destroy(&sema[3]);
    sem_destroy(&sema[4]);
    sem_destroy(&sema[5]);

    pthread_exit(NULL);

    board(ludo);
}

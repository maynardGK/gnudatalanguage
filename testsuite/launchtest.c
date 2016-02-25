#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
int main(int c,char**v) {
char*p;
if(c!=2) return 1;
for(p=v[1];*p;++p);
if(p-4<v[1]) return 1;
if(p[-1]!='o'||p[-2]!='r'||p[-3]!='p'||p[-4]!='.') return 1;
*(p-4)=0;

_putenv("LC_COLLATE=C");
_putenv("GDL_PATH=${BASE_SOURCE}/testsuite/;${BASE_SOURCE}/src/pro/");
_putenv("GDL_STARTUP=");
_putenv("IDL_STARTUP=");

char *tmp = (char *)malloc( sizeof(char) * (strlen("${BASE_BINARY}/src/gdl -quiet -e ") + strlen(v[1]) + 1));
//PROCESS_INFORMATION processInformation = {0};
//STARTUPINFO startupInfo                = {0};
//startupInfo.cb                         = sizeof(startupInfo);
sprintf(tmp, "${BASE_BINARY}/src/gdl.exe -quiet -e %s", v[1]);
//int rtn = CreateProcess(NULL, tmp, NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInformation);
int rtn = system(tmp);
char cmd[512];
snprintf(cmd, 511, "fgrep -q %s.pro ${BASE_SOURCE}/testsuite/tests.xfail",  v[1]);
int xfail = (system(cmd) == 0);
printf("%i: %s\n", xfail, cmd)

if (rtn == 77) {
    printf("TEST SKIPPED");
    exit(0);
} if (rtn == -1 || rtn == 127) {
    printf("LAUNCH PROCESS FAILED");
    exit(1);
} else if (rtn == 0) {
    if (xfail) {
      printf("TEST XFAIL but successful");
    }
    exit(0);
} else {
    if (xfail) {
      printf("TEST XFAILED");
      exit(0);
    } else {
      exit(rtn);
    }
}
}

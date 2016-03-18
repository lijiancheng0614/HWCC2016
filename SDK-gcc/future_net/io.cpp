#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "lib_io.h"

#define INLINE  static __inline
#define PRINT   printf

#define MAX_RECORD  100
#define MAX_LINE_LEN 4000

typedef struct
{
    char *record[MAX_RECORD];
    int head;
    int tail;
    int cnt;
}RECORD_QUEUE_S;

static char g_result[MAX_LINE_LEN] = "NA";

INLINE void write_file(const bool cover, const char * const buff, const char * const filename);

void record_result(unsigned short edge)
{
    static int len = 0;
    if (len > (MAX_LINE_LEN - 10))
        return;
    if (len > 0)
        len += sprintf(g_result + len, "|");
    len += sprintf(g_result + len, "%d", edge);
}

void print_time(const char *head)
{ 
    struct timeb rawtime; 
    struct tm * timeinfo; 
    ftime(&rawtime); 
    timeinfo = localtime(&rawtime.time);

    static int ms = rawtime.millitm;
    static unsigned long s = rawtime.time;
    int out_ms = rawtime.millitm - ms;
    unsigned long out_s = rawtime.time - s;
    ms = rawtime.millitm;
    s = rawtime.time;

    if (out_ms < 0)
    {
        out_ms += 1000;
        out_s -= 1;
    }
    printf("%s time: %s \tused time: %lu s %d ms.\n", head, asctime(timeinfo), out_s, out_ms); 
}

bool read(FILE *fp, int &x)
{
    // read positive integer
    char c = getc(fp);
    while (c < '0' || c > '9')
        c = getc(fp);
    for (x = 0; c >= '0' && c <= '9'; c = getc(fp))
        x = x * 10 + c - '0';
    if (c == '\r')
        c = getc(fp);
    if (c == '\n')
        return 0;
    return 1;
}

vector<vi> read_file(const char * const filename)
{
    vector<vi> ans;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        PRINT("Fail to open %s, %s.\n", filename, strerror(errno));
        return ans;
    }
    PRINT("Open %s.\n", filename);

    while (!feof(fp))
    {
        vi a;
        int x;
        while (read(fp, x) && !feof(fp))
            a.push_back(x);
        ans.push_back(a);
    }
    fclose(fp);
    PRINT("%d lines in %s.\n", (int)ans.size(), filename);
    return ans;
}

void write_result(const char * const filename)
{
    if (g_result[0] == '\0')
        return;

    write_file(1, g_result, filename);
}

INLINE void write_file(const bool cover, const char * const buff, const char * const filename)
{
    if (buff == NULL)
        return;

    const char *write_type = cover ? "w" : "a";//1:覆盖写文件，0:追加写文件
    FILE *fp = fopen(filename, write_type);
    if (fp == NULL)
    {
        PRINT("Fail to open file %s, %s.\n", filename, strerror(errno));
        return;
    }
    PRINT("Open file %s OK.\n", filename);
    fputs(buff, fp);
    fputs("\n", fp);
    fclose(fp);
}


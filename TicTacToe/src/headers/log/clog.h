#ifndef _CLOG_H_
#define _CLOG_H_

void Clog(const char* msg, const char* log_level);
void Clog(const char* fmsg, const char* smsg, const char* log_level);
void Clog(const char* msg, int d, const char* log_level);
void Clog(const char* msg, float f, const char* log_level);
void Clog(const char* msg, double dd, const char* log_level);
const char* CreateLogDirectory_func(void);
void WriteLog_func(const char* msg, const char* log_dir, const char* log_level);
void DeleteLog_func(const char* log_level);
void DeleteLogs_func(void);
char* CombineMessages_func(const char* msg1, const char* msg2);
char* CombineMessages_func(const char* msg, int d);
char* CombineMessages_func(const char* msg, float f);
char* CombineMessages_func(const char* msg, double dd);

#endif

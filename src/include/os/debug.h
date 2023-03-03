#ifndef DEBUG_H
#define DEBUG_H

void debugk(char* file, int line, const char* fmt, ...);

#define DEBUGK(fmt, args...) debugk(__BASE_FILE__, __LINE__, fmt, ##args)

#endif //DEBUG_H
#define ERROR 0
#define INFO 1
#define VERBOSE 2
#define DEBUG 3

#define LEVEL INFO

#define LOG_ERR(message) printf("[!][%s:%d] %s\n", __FILE__, __LINE__, message), exit(1)

#if LEVEL >= INFO
#define LOG_INFO(message) printf("[*] %s\n", message)
#else
#define LOG_INFO(message)
#endif // DEBUG

#if LEVEL >= VERBOSE
#define LOG_VER(format, ...) printf("[*][%s:%d] ",__FILE__, __LINE__), printf(format, __VA_ARGS__)
#else
#define LOG_VER(format, ...)
#endif // DEBUG

#if LEVEL >= DEBUG
#define LOG_DEBUG(format, ...) printf("[?][%s:%d] ", __FILE__, __LINE__), printf(format, __VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif // DEBUG

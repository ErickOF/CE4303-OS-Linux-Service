#ifndef CONFIG_HANDLER
#define CONFIG_HANDLER

extern char dirCol[256];
extern char dirHis[256];
extern char dirLog[256];
extern int filters, kavg, kmedian;

void createFolders();
void readConfig();

#endif

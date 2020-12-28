/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief driver program
 * @cdate 2020-10-27
 * @mdate 2020-11-07
 */

#include "plugin.h"
#include <dlfcn.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_PLUGIN 20
#define MAX_STR 1024

// static void getType(Type *type, char *argv[]);
static int getPluginType(char *argv[],int pluginCnt, Handle **handle);

/**
 * @param argv[1]: input file name
 * @param argv[2]: sorting type
 */
int main(int argc, char *argv[]){
    srand(time(NULL));
    if (argc != 3){
        printf(
            "Error: arguments not match!\n"
            "Usage: ./cmplxsort [Input File Name] [Sorting Type]\n"
            "Supported file types for now: .txt .csv\n"
            );
        return -1;
    }
    // Load and extract plugins
    Handle *handle[MAX_PLUGIN];
    for(int i=0;i<MAX_PLUGIN;i++) handle[i]=NULL;
    int pluginCnt = pluginInit(handle);
    // Load file types and sorting types
    // Type type;
    // getType(&type, argv);
    // run the program
    int pluginId = getPluginType(argv,pluginCnt,handle);
    handle[pluginId]->ui_cmd(argv);
    dlclose(handle[pluginId]->handle);
    // free the handle
    // printf("%d\n",pluginCnt);fflush(NULL);
    for(int i=0;i<pluginCnt;i++) if(handle[i]!=NULL) free(handle[i]);
    
    return 0;
}

static int getPluginType(char *argv[],int pluginCnt, Handle **handle){
    char tmpArg[MAX_STR];
    strcpy(tmpArg, argv[1]);
    char *token = strtok(tmpArg, ".");
    // token = strtok(NULL, ".");
    token = strtok(NULL, " \n");
    // printf("TEST: %s\n",token);
    // for(int i=0;i<pluginCnt;i++) printf("%s\n",handle[i]->type);
    for(int i=0;i<pluginCnt;i++){
        if(!strcmp(token,handle[i]->type)){
            return i;
        }
    }
}

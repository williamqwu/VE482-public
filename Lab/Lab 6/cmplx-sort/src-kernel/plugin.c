/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief plugin manager
 * @cdate 2020-11-08
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>  // load libraries
#include <dirent.h> // read files from a given directory

int pluginInit(Handle **handle){
  int pluginCnt = 0;
  DIR *dir;
  struct dirent *ent;
  if((dir = opendir("./plugins/")) != NULL){
    while((ent = readdir(dir)) != NULL){
      handle[pluginCnt] = (Handle *) malloc(sizeof(Handle));
      void *hd;
      // open the library
      char tmpStr[256];
      strcpy(tmpStr,"./plugins/");
      strcat(tmpStr,ent->d_name);
      strcpy(ent->d_name,tmpStr);
      if((hd = dlopen(ent->d_name, RTLD_LAZY)) == NULL){
        if(!strcmp(ent->d_name,"./plugins/.") || !strcmp(ent->d_name,"./plugins/..")) continue;
        printf("Error: plugin file \"%s\" not found.\n",ent->d_name);
        continue;
        exit(-1);
      }
      if(!strcmp(ent->d_name,"./plugins/.") || !strcmp(ent->d_name,"./plugins/..")) continue;
      // printf("Info: %s\n",ent->d_name);
      // extract the symbol
      *(void **) (&(handle[pluginCnt]->ui_cmd)) = dlsym(hd, "ui_cmd");
      if(dlerror() != NULL){
        printf("Error: extracting ui_cmd fails.\n");
        exit(-1);
      }
      char tmpArg[36];
      strcpy(tmpArg, ent->d_name);
      char *token = strtok(tmpArg, ".");
      token = strtok(token, "/");
      token = strtok(NULL, "/");
      strcpy(handle[pluginCnt]->type,token);
      handle[pluginCnt]->handle = hd;
      pluginCnt++;
    }
  }
  else{
    printf("Error: plugin directory not found.\n");
    exit(-1);
  }
  return pluginCnt;
}

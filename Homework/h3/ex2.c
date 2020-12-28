/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief VE482 h3 ex2
 * @date 2020-10-18
 */
#include "list.h"
#include <string.h>

#define SORT_RAND 0
#define SORT_INC  1
#define SORT_DEC  2
#define SORT_NULL -1

#define DATA_CHAR   0
#define DATA_INT    1
#define DATA_DOUBLE 2
#define DATA_NULL   -1

#define DELIM "_."

#define MAX_KEY 1024
#define MAX_STR 1024

typedef struct _type{
    int sortType;
    int dataType;
} Type;

/**
 * @brief initialize the list
 * 
 * @param list 
 */
void listConstructor(List *list){
    list->head = NULL;
    list->length = 0;
}

static void getType(Type *type, char *argv[]){
    type->dataType = DATA_NULL;
    type->sortType = SORT_NULL;
    char tmpArg[MAX_STR];
    memset(tmpArg,0,MAX_STR);
    strcpy(tmpArg, argv[1]);
    char *token = strtok(tmpArg, DELIM);
    token = strtok(NULL, DELIM);
    if(!strcmp(token, "int")) type->dataType = DATA_INT;
    else if(!strcmp(token, "char")) type->dataType = DATA_CHAR;
    else if(!strcmp(token, "double")) type->dataType = DATA_DOUBLE;
    
    if(!strcmp(argv[2], "rand")) type->sortType = SORT_RAND;
    else if(!strcmp(argv[2], "inc")) type->sortType = SORT_INC;
    else if(!strcmp(argv[2], "dec")) type->sortType = SORT_DEC;

    assert(type->dataType!=DATA_NULL && type->sortType!=SORT_NULL);
}

/**
 * @brief append a new node at the end of the list
 * 
 * @param list 
 * @param node 
 */
void listAdd(List *list, Node *node){
    if(list->head==NULL) list->head=node;
    else{
        Node *indexNode = list->head;
        while(indexNode->next!=NULL) indexNode=indexNode->next;
        indexNode->next=node;
    }
    list->length++;
}

static void insert(List *list, char *key, void *data){
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->data = data;
    node->next = NULL;
    listAdd(list, node);
}

/**
 * @brief search for an element in the list with key;
 *      return NULL if the list is empty or key doesn't match
 * 
 * @param list 
 * @param key 
 * @return void* 
 */
void *listSearch(List *list, const char *key){
    if(list->length==0) return NULL;
    Node *nodeIndex;
    for(nodeIndex=list->head; nodeIndex!=NULL; nodeIndex=nodeIndex->next){
        if(!strcmp(key, nodeIndex->key)) return nodeIndex->data;
    }
    return NULL;
}

static int rnd(const void *a, const void *b){
    assert(a!=NULL);
    assert(b!=NULL);
    return (((rand())%2)*2-1);
}

static int int_inc(const void *a, const void *b){
    return *((int *)(((Node*)a)->data)) - *((int *)(((Node*)b)->data));
}
static int int_dec(const void *a, const void *b){
    return *(int *)(((Node*)b)->data) - *(int *)(((Node*)a)->data);
}
static int double_inc(const void *a, const void *b){
    return *(double *)(((Node*)a)->data) - *(double *)(((Node*)b)->data);
}
static int double_dec(const void *a, const void *b){
    return *(double *)(((Node*)b)->data) - *(double *)(((Node*)a)->data);
}
static int char_inc(const void *a, const void *b){
    return strcmp((char *)(((Node*)a)->data), (char *)(((Node*)b)->data));
}
static int char_dec(const void *a, const void *b){
    return strcmp((char *)(((Node*)b)->data), (char *)(((Node*)a)->data));
}

int (*cmp[3][3]) (const void *, const void *) = {
    {rnd, char_inc, char_dec},
    {rnd, int_inc, int_dec},
    {rnd, double_inc, double_dec}
};

static void sort(Type *type, List *list){
    if(list->length==0) return;
    Node *src[list->length];
    Node *nodeIndex = list->head;
    int index = 0;
    while(nodeIndex!=NULL){
        src[index++]=nodeIndex;
        nodeIndex=nodeIndex->next;
    }
    // qsort(src, list->length, sizeof(Node *), (cmp[type->dataType][type->sortType]));
    Node *flag;
    int j;
    for(int i=1;i<list->length;i++){
        flag = src[i];
        j = i-1;
        while(j>=0 && (cmp[type->dataType][type->sortType](src[j], flag) >= 0)){
            src[j+1]=src[j];
            j--;
        }
        src[j+1]=flag;
    }
    list->head = src[0];
    for(int i=0;i<list->length-1;i++) src[i]->next=src[i+1];
    src[list->length-1]->next = NULL;
}

/**
 * @brief print the list in order
 * 
 * @param type 
 * @param list 
 */
void print(Type *type, const List *list){
    char outFileName[100];
    memset(outFileName,0,100);
    if(type->sortType==SORT_INC) strcat(outFileName,"inc_");
    else if(type->sortType==SORT_DEC) strcat(outFileName, "dec_");
    else strcat(outFileName, "rand_");
    if(type->dataType==DATA_INT) strcat(outFileName, "int.txt");
    else if(type->dataType==DATA_DOUBLE) strcat(outFileName, "double.txt");
    else strcat(outFileName, "char.txt");
    FILE *fout = fopen(outFileName, "w");
    printf("writing %s\n",outFileName);
    if(list->length==0) return;
    Node *nodeIndex;
    for(nodeIndex=list->head;nodeIndex!=NULL;nodeIndex=nodeIndex->next){
        if(type->dataType==DATA_INT) fprintf(fout, "%s=%d\n",nodeIndex->key, *(int*)(nodeIndex->data));
        else if(type->dataType==DATA_DOUBLE) fprintf(fout, "%s=%lf\n",nodeIndex->key, *(double*)(nodeIndex->data));
        else fprintf(fout, "%s=%s\n",nodeIndex->key, (char*)(nodeIndex->data));
    }
}

/**
 * @brief free the list
 * 
 * @param list 
 */
void listDestructor(List *list){
    if(list->length==0) return;
    Node *nodeIndex;
    for(nodeIndex=list->head; nodeIndex!=NULL;){
        Node *nodeOld = nodeIndex;
        free(nodeIndex->key);
        free(nodeIndex->data);
        nodeIndex=nodeIndex->next;
        free(nodeOld);
    }
}

static void driver(Type *type, char *argv[]){
    List list;
    listConstructor(&list);
    printf("reading %s\n",argv[1]);
    char *raw = malloc(sizeof(char)*MAX_STR);
    memset(raw,0,MAX_STR);
    char *tmpKey, *tmpData;
    
    FILE *fin = fopen(argv[1], "r");
    while(fgets(raw,MAX_STR,fin)!=NULL){
        // printf("%s|%d|",raw,(int)sizeof(raw));
        tmpKey = strtok(raw,"=");
        tmpData = strtok(NULL, " \n");
        void *data;
        if(type->dataType==DATA_INT){
            int value = atoi(tmpData);
            // printf("%s|%s|%d\n",tmpKey,tmpData,value);
            // fscanf(fin, "%d", &value);
            data = (void *)malloc(sizeof(int));
            *(int *)data = value;
        }
        else if(type->dataType==DATA_DOUBLE){
            double value = atof(tmpData);
            // fscanf(fin, "%lf", &value);
            data = (void *)malloc(sizeof(double));
            *(double *)data = value;
        }
        else{
            // char value[MAX_STR];
            // fscanf(fin, "%s", value);
            data = (void *)malloc(sizeof(char)*MAX_STR);
            memset(data,0,MAX_STR);
            strcpy((char *)data, tmpData);
        }
        char *key = (char *)malloc(sizeof(char)*MAX_KEY);
        memset(key,0,MAX_KEY);
        strcpy(key,tmpKey);
        insert(&list, key, data);
        memset(raw,0,MAX_STR);
    }
    free(raw);
    // Node*tmpnode=list.head;
    // for(int i=0;i<3;i++){
    //     printf("%d ",*(int *)(tmpnode->data));
    //     tmpnode=tmpnode->next;
    // }
    printf("sorting elements\n");
    sort(type, &list);
    print(type, &list);
    // NOTE: test function for listSearch
    // void *tmpp=NULL;
    // tmpp = listSearch(&list,"a");
    // if(tmpp!=NULL) printf("%d\n",*(int*)tmpp);

    listDestructor(&list);
    fclose(fin);
}

/**
 * @param argv[1]: input file name
 * @param argv[2]: sorting type
 */
int main(int argc, char *argv[]){
    srand(time(NULL));
    if (argc != 3){
        printf("Error: arguments not match!\n");
        return -1;
    }
    Type type;
    getType(&type, argv);
    driver(&type, argv);
    
    return 0;
}

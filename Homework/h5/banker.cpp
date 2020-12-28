/**
 * @author William Wu
 * @brief demo program for banker's algorithm with multiple resources
 * @date 2020-11-01
 */
#include <iostream>

using namespace std;

#define PS_CNT 5 // proccess count
#define RS_CNT 4 // resource count

int M[PS_CNT][RS_CNT] = { 
    {7,5,2,4},
    {3,3,2,3},
    {6,2,3,4},
    {2,9,4,2}
}; // demo maximum matrix

int R[PS_CNT][RS_CNT] = { 
    {6,4,1,3},
    {1,3,0,2},
    {3,1,1,2},
    {1,7,0,0}
}; // demo request matrix

int A[RS_CNT] = {3,2,1,3}; // demo available matrix

#define S_RUNNING 1
#define S_SUCCESS 2
#define S_FAIL 0

#define UNREACHABLE -1

int main(){
    int remainingPs = PS_CNT;
    // int isVisit[PS_CNT];
    int exitStatus = S_RUNNING;
    while(exitStatus==S_RUNNING){
        bool isFound = false;
        for(int i=0;i<PS_CNT;i++){
            if(R[i][0]==UNREACHABLE) continue;
            bool isSatisfy = true;
            for(int j=0;j<RS_CNT;j++){
                if(A[j]<R[i][j]) isSatisfy=false;
            }
            if(isSatisfy){
                isFound = true;
                for(int j=0;j<RS_CNT;j++) A[j]-=R[i][j];
                /* execute the process, allocating its requested resources */
                for(int j=0;j<RS_CNT;j++) A[j]+=M[i][j];
                /* exiting the process */
                R[i][0]=UNREACHABLE;
                remainingPs--;
            }
        }
        if(!isFound) exitStatus=S_FAIL;
        if(remainingPs<=0){
            exitStatus=S_SUCCESS;
        }
    }
    if(exitStatus==S_SUCCESS){
        cout << "all recources released, processes terminated safely.\n";
    }
    else if(exitStatus==S_FAIL){
        cout << "the state is unsafe, and the request should be denied.\n";
    }
    else{
        cerr << "system error.\n";
        exit(-1);
    }

    return 0;
}

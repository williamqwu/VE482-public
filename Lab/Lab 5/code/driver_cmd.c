/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief VE482 l5 ex1
 * @cdate 2020-10-27
 */

#include "ui.h"
#include "sort.h"

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
    ui_cmd(&type, argv);

    return 0;
}

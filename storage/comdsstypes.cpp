# include "comdsstypes.h"
# include "../dsstypes.h"
# include "iostream"

using namespace std;

/**
 * typedef struct {
    int dataType;       // 原本数据类型：1表示不定长字符串，2表示定长字符串，3表示整型，4表示double
    int compType;        // 压缩类型： 0表示没有压缩，1表示scale缩放，2表示字典表压缩
    int scale;          // 缩放倍数， comType==1时，scale才有效
    Dict *  dict;  // 字符串字典表，每个元素是一个指向字符串的指针
    void* data;         // 数据指针，根据不同的数据类型使用不同的指针类型
} comp_column;

typedef struct {
    comp_column* columns;    // 列数据数组
    int columnCount;    // 列的数量
    int rowCount;       // 行的数量
} comp_table;

*/

void init_customer_table(comp_table comp_customer){
    comp_customer.columnCount = 8;
    comp_customer.rowCount = 150000; // SF = 1
    comp_customer.columns = new comp_column[comp_customer.columnCount];

    // custkey
    comp_customer.columns[0].columnName = "custkey";
    comp_customer.columns[0].dataType = 3;
    comp_customer.columns[0].compType = 0;
    comp_customer.columns[0].data = new int[comp_customer.rowCount];

    // name
    comp_customer.columns[1].columnName = "name";
    comp_customer.columns[1].dataType = 2;
    comp_customer.columns[1].compType = 0;
    comp_customer.columns[1].data = new string[comp_customer.rowCount];

    //address
    comp_customer.columns[2].columnName = "address";
    comp_customer.columns[2].dataType = 2;
    comp_customer.columns[2].compType = 0;
    comp_customer.columns[2].data = new string[comp_customer.rowCount];

    //nationkey
    comp_customer.columns[3].columnName = "nationkey";
    comp_customer.columns[3].dataType = 3;
    comp_customer.columns[3].compType = 0;
    comp_customer.columns[3].data = new int[comp_customer.rowCount];

    //phone
    comp_customer.columns[4].columnName = "phone";
    comp_customer.columns[4].dataType = 2;
    comp_customer.columns[4].compType = 0;
    comp_customer.columns[4].data = new string[comp_customer.rowCount];

    //acctbal
    comp_customer.columns[5].columnName = "acctbal";
    comp_customer.columns[5].dataType = 4;
    comp_customer.columns[5].compType = 1;
    comp_customer.columns[5].scale = 100;
    comp_customer.columns[5].data = new int[comp_customer.rowCount];

    //mktsegment
    comp_customer.columns[6].columnName = "mktsegment";
    comp_customer.columns[6].dataType = 2;
    comp_customer.columns[6].compType = 2;
    comp_customer.columns[6].data = new int[comp_customer.rowCount];
    comp_customer.columns[6].dict = C_MKSEGEMNT;





}


int mk_comp_customer( comp_table comp_customer,  customer_t *customer, int mode){
    init_customer_table(comp_customer);
    


}
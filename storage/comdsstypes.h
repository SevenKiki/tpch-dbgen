
 /* 
 * general definitions and control information for the compressed DSS data types
 * and function prototypes
 */

# include "string.h"
# include "stdio.h"
# include "../dict/dict.hpp"

// typedef struct {
//     string *  dictData;   
//     int dictSize;
// }Dict;

// typedef struct {
//     string columnName;
//     int dataType;       // 原本数据类型：1表示不定长字符串，2表示定长字符串，3表示整型，4表示浮点型
//     int compType;        // 压缩类型： 0表示没有压缩，1表示scale缩放，2表示字典表压缩
//     int scale;          // 缩放倍数， comType==1时，scale才有效
//     Dict *  dict;  // 字符串字典表，每个元素是一个指向字符串的指针
//     void* data;         // 数据指针，根据不同的数据类型使用不同的指针类型
// } comp_column;

// typedef struct {
//     comp_column* columns;    // 列数据数组
//     int columnCount;    // 列的数量
//     int rowCount;       // 行的数量
// } comp_table;



 /* 
 * general definitions and control information for the compressed DSS data types
 * and function prototypes
 */

# include "string.h"
# include "stdio.h"
# include "../dict/dict.hpp"

typedef struct {
    string *  dictData;   
    int dictSize;
}Dict;

typedef struct {
    string columnName;
    int dataType;       // 原本数据类型：1表示不定长字符串，2表示定长字符串，3表示整型，4表示浮点型
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


/*
 * typedefs
 */
typedef struct
{
    DSS_HUGE            custkey;
    char            name[C_NAME_LEN + 3];
    char            address[C_ADDR_MAX + 1];
    int             alen;
    DSS_HUGE            nation_code;
    char            phone[PHONE_LEN + 1];
    DSS_HUGE            acctbal;
    char            mktsegment[MAXAGG_LEN + 1];
    char            comment[C_CMNT_MAX + 1];
    int             clen;
}               customer_t;

typedef struct 
{
    DSS_HUGE            custkey;
    char            name[C_NAME_LEN + 3];
    char            address[C_ADDR_MAX + 1];
    int             alen;
    DSS_HUGE            nationkey;
    char            phone[PHONE_LEN + 1];
    DSS_HUGE            acctbal; /* compression type: scale ｜ 100x */
    int             mktsegment;  /* compression type: dict | NDV = 5*/
    char            comment[C_CMNT_MAX + 1];
    int             clen;
}           customer_tuple;

typedef struct 
{
    customer_tuple * tuples;
    
}           customer_table;

/* customers.c */
long mk_cust   PROTO((DSS_HUGE n_cust, customer_t * c));
int pr_cust    PROTO((customer_t * c, int mode));
int ld_cust    PROTO((customer_t * c, int mode));


typedef struct/*Primary Key:ORDERKEY, LINENUMBER*/
{
    DSS_HUGE	    okey; 
    DSS_HUGE            partkey;
    DSS_HUGE            suppkey;
    DSS_HUGE            lcnt; /*LineNumber*/
    DSS_HUGE            quantity;
    DSS_HUGE            eprice;/*extendprice*/
    DSS_HUGE            discount;
    DSS_HUGE            tax;
    DSS_HUGE            pskey; /*新增字段*/
    char            rflag[1];/*returnflag*/
    char            lstatus[1];/*linestatus*/
    char            cdate[DATE_LEN];/*commitDate*/
    char            sdate[DATE_LEN];/*shipDate*/
    char            rdate[DATE_LEN];/*receiptDate*/
    char           shipinstruct[MAXAGG_LEN + 1];
    char           shipmode[MAXAGG_LEN + 1];
    char           comment[L_CMNT_MAX + 1];
    int            clen;/*length of comment*/

}               line_t;

typedef struct
{
    DSS_HUGE	    okey;
    DSS_HUGE        custkey;
    char            orderstatus;
    DSS_HUGE            totalprice;
    char            odate[DATE_LEN];
    char            opriority[MAXAGG_LEN + 1];
    char            clerk[O_CLRK_LEN + 1];
    long            spriority; /*ship-priority*/
    DSS_HUGE            lines;  /*该order包含多少个lines 最少1个，最多7个*/
    char            comment[O_CMNT_MAX + 1];
    int            clen; /* length of comment*/

    line_t          l[O_LCNT_MAX]; /* 每个order最多包含7个lineitem*/
}               order_t;

/* order.c */
long	mk_order	PROTO((DSS_HUGE index, order_t * o, long upd_num));
int		pr_order	PROTO((order_t * o, int mode));
int		ld_order	PROTO((order_t * o, int mode));
void	mk_sparse	PROTO((DSS_HUGE index, DSS_HUGE *ok, long seq));

typedef struct
{
    DSS_HUGE             pskey; /*新增字段*/
    DSS_HUGE            partkey;
    DSS_HUGE            suppkey;
    DSS_HUGE            qty;
    DSS_HUGE            scost; /*supplycost*/
    char           comment[PS_CMNT_MAX + 1];
    int            clen;
}               partsupp_t;

typedef struct
{
    DSS_HUGE           partkey;
    char           name[P_NAME_LEN + 1];
    int            nlen;
    char           mfgr[P_MFG_LEN + 1];
    char           brand[P_BRND_LEN + 1];
    char           type[P_TYPE_LEN + 1];
    int            tlen; /* length of type*/
    DSS_HUGE           size;
    char           container[P_CNTR_LEN + 1];
    DSS_HUGE           retailprice;
    char           comment[P_CMNT_MAX + 1];
    int            clen;
    DSS_HUGE        psbegin;/* 新增字段，ps_key 的开始值*/
    
    partsupp_t     s[SUPP_PER_PART];
}               part_t;

/* parts.c */
// long mk_part   PROTO((DSS_HUGE index, part_t * p));
long mk_part   PROTO((DSS_HUGE index, DSS_HUGE psnum, part_t * p)); /*make part表的同时加入ps_key字段*/
int pr_part    PROTO((part_t * part, int mode));
int ld_part    PROTO((part_t * part, int mode));

typedef struct
{
    DSS_HUGE            suppkey;
    char            name[S_NAME_LEN + 1];
    char            address[S_ADDR_MAX + 1];
    int             alen;/*length of address*/
    DSS_HUGE            nation_code;
    char            phone[PHONE_LEN + 1];
    DSS_HUGE            acctbal;
    char            comment[S_CMNT_MAX + 1];
    int             clen;
}               supplier_t;
/* supplier.c */
long mk_supp   PROTO((DSS_HUGE index, supplier_t * s));
int pr_supp    PROTO((supplier_t * supp, int mode));
int ld_supp    PROTO((supplier_t * supp, int mode));

typedef struct
{
    DSS_HUGE            timekey;
    char            alpha[DATE_LEN];
    long            year;
    long            month;
    long            week;
    long            day;
} dss_time_t;               

/* time.c */
long mk_time   PROTO((DSS_HUGE h, dss_time_t * t));

/*
 * this assumes that N_CMNT_LEN >= R_CMNT_LEN 
 */
typedef struct
{
    DSS_HUGE            code;
    char            *text;
    long            join;
    char            comment[N_CMNT_MAX + 1];
    int             clen;
}               code_t;

/* code table */
int mk_nation   PROTO((DSS_HUGE i, code_t * c));
int pr_nation    PROTO((code_t * c, int mode));
int ld_nation    PROTO((code_t * c, int mode));
int mk_region   PROTO((DSS_HUGE i, code_t * c));
int pr_region    PROTO((code_t * c, int mode));
int ld_region    PROTO((code_t * c, int mode));


/** make compressed table*/
int mk_comp_customer( comp_table comp_customer,  customer_t *customer, int mode);
int mk_comp_lineitem( comp_table comp_lineitem, line_t lineitem);
int mk_comp_part (comp_table comp_part,part_t part);
int mk_comp_supplier(comp_table comp_supplier, supplier_t supplier);
int mk_comp_partsupp(comp_table comp_partsupp, partsupp_t partsupp);
int mk_comp_orders(comp_table comp_orders, order_t order );
int mk_comp_nation(comp_table comp_nation, code_t nation);
int mk_comp_region(comp_table comp_region, code_t region);



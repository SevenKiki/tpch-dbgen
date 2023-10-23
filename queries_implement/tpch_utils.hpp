#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <map>
#include <vector>
#include <set>
using namespace std;

#define SF 1

#define BASE_PATH "/home/heyuqi/HYQ/multicore-hashjoin/tpchdata/"

#if SF == 1
#define DATA_DIR BASE_PATH "s1_columnar/"
#define L_LEN 6001215
#define PS_LEN 800000
#define O_LEN 1500000 
// #define O_LEN 6000000
#define N_LEN 25
#define R_LEN 5
#define C_LEN 150000
#define P_LEN 200000
#define S_LEN 10000
#elif SF == 10
#define DATA_DIR BASE_PATH "s10_columnar/"
#define L_LEN 59986052

#elif  SF == 30
#define DATA_DIR BASE_PATH "s30_columnar/"
#define L_LEN 179998372

#else //SF==100
#define DATA_DIR BASE_PATH "s100_columnar/"
#define L_LEN   600037902
#define PS_LEN  80000000
#define O_LEN   150000000
#define N_LEN   25
#define R_LEN   5
#define C_LEN   150000000
#define P_LEN   20000000
#define S_LEN   1000000
#endif


// define the num of each table fieldsx
# define C_FIELD_NUM 8
# define L_FIELD_NUM 19
# define N_FIELD_NUM 4 
# define O_FIELD_NUM 10
# define P_FIELF_NUM 9
# define PS_FIELF_NUM 6
# define R_FIELD_NUM 3 
# define S_FIELD_NUM 7

//define the size of char *
// # define P_TYPE_SIZE 25

int index_of(string * arr, int len, string val) {
  for (int i=0; i<len; i++)
    if (arr[i] == val)
      return i;
  return -1;
}

string lookup(string col_name) {
  string lineitem[] = {"l_orderkey", "l_partkey", "l_suppkey", "l_linenumber", "l_quantity", "l_extendedprice", "l_discount", "l_tax", "l_returnflag", "l_linestatus", "l_shipdate", "l_commitdate","l_receiptdate","l_shipinstruct","l_shipmode","l_comment","l_key", "l_supplycost", "l_orderkey2"};
  string part[]= {"p_partkey", "p_name","p_mfgr", "p_brand", "p_type", "p_size", "p_container"};
  string orders[]= {"o_orderkey", "o_custkey","o_orderstatus","o_totalprice",  "o_orderdate","o_orderpriority", "o_clerk","o_shippriority", "o_comment", "o_key"};
  string customer[]= {"c_custkey", "c_name","c_address","c_nationkey","c_phone", "c_acctbal", "c_mktsegment", "c_comment"};
  string partsupp[]= {"ps_partkey", "ps_suppkey", "ps_availqty", "ps_supplycost", "ps_comment","ps_key"};
  string supplier[]= {"s_suppkey", "s_name","s_address","s_nationkey", "s_phone", "s_acctbal", "s_comment"};
  string nation[]= {"n_nationkey", "n_name", "n_regionkey", "n_comment"};
  string region[]= {"r_regionkey", "r_name", "r_comment"};


  if (col_name[0] == 'l') {
    int index = index_of(lineitem, L_FIELD_NUM, col_name);
    return "LINEITEM" + to_string(index);
  } 
  else if (col_name[0] == 'o') {
    int index = index_of(orders, O_FIELD_NUM, col_name);
    return "ORDERS" + to_string(index);
  } 
  else if (col_name[0] == 's') {
    int index = index_of(supplier, S_FIELD_NUM, col_name);
    return "SUPPLIER" + to_string(index);
  } 
  else if (col_name[0] == 'c') {
    int index = index_of(customer, C_FIELD_NUM, col_name);
    return "CUSTOMER" + to_string(index);
  } 
  else if (col_name[0] == 'p' && col_name[1] == 's') {
    int index = index_of(partsupp, PS_FIELF_NUM, col_name);
    return "PARTSUPP" + to_string(index);
  }
  else if (col_name[0] == 'p') {
    int index = index_of(part, P_FIELF_NUM, col_name);
    return "PART" + to_string(index);
  } 
  else if (col_name[0] == 'n'){
    int index = index_of(nation, N_FIELD_NUM, col_name);
    return "NATION" + to_string(index);
  }
  else if (col_name[0] == 'r'){
    int index = index_of(region, R_FIELD_NUM, col_name);
    return "REGION" + to_string(index);
  }
  return "error";
}


template<typename T>
T* loadColumn(string col_name, int num_entries) {
    T* h_col = new T[num_entries];
    string filename = DATA_DIR + lookup(col_name);
    ifstream colData (filename.c_str(), ios::in | ios::binary);
    
    if (!colData) {
      return NULL;
    }
    colData.read((char*)h_col, num_entries * sizeof(T));

    printf("%s: %d * %d\n",col_name.data(),num_entries, sizeof(T));
    return h_col;
}

template<typename T>
int storeColumn(string col_name, int num_entries, int* h_col) {
  string filename = DATA_DIR + lookup(col_name);
  ofstream colData (filename.c_str(), ios::out | ios::binary);
  if (!colData) {
    return -1;
  }

  colData.write((char*)h_col, num_entries * sizeof(T));
  return 0;
}

template<typename T>
int  setBitmap1(T * col_name, T col_value, int * bitmap, int col_num){
  for(int i = 0; i< col_num; i++){
    if(col_name[i] == col_value){
      bitmap[i] = 1;
    }
  }
  return 0;
}

template<typename T>
int  show10(T * col_name){
  cout <<"*************"<<col_name<<"*************"<<endl;
  for(int i = 0; i< 10; i++){
    cout << i<< " "<<col_name[i] <<endl;
  }
  cout <<endl;
  return 0;
}

template<typename T>
int showLast10(T * col_name, int LEN){
  cout <<"*************"<<col_name<<"*************"<<endl;
  for(int i = LEN-11; i< LEN; i++){
    cout << i<< " "<<col_name[i] <<endl;
  }
  cout <<endl;
}

/*int main() {*/
  //int *h_col = new int[10];
  //for (int i=0; i<10; i++) h_col[i] = i;
  //storeColumn<int>("test", 10, h_col);
  //int *l_col = loadColumn<int>("test", 10);
  //for (int i=0; i<10; i++) cout << l_col[i] << " ";
  //cout << endl;
  //return 0;
/*}*/

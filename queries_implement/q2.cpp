# include <iostream>
# include <stdio.h>
# include <chrono>
# include "tpch_util.hpp"
using namespace std;

const float EPS = 0.000000001;
struct result{
  int n_nationkey;
  float revenue;
};

int * setBitmapRegion(int* r_regionkey, int * bitmap_r, int r_num, char* compare, int predicate){
  if(!strcmp(compare, "=")){
    for ( int i = 0; i < r_num; i++){
      int tmp = r_regionkey[i];
      if (r_regionkey[i] == predicate){
        bitmap_r[i] = 1;
      }
    }
  }
}

int * setBitmapOrderdate(int * o_orderdate,int * bitmap_o, int o_num, char * compare1, int predicate1, char * compare2, int predicate2){
  if(!strcmp(compare1, ">=") && !strcmp(compare2, "<")){
      for ( int i = 0; i < o_num; i++){
        int tmp = o_orderdate[i];
        if (o_orderdate[i] >= predicate1 && o_orderdate[i] < predicate2){
            bitmap_o[i] =1;
        }
      }
  }
}

int * setBitmapS(int * s_nationkey, int * n_nationkey, int * n_regionkey, int * r_regionkey,int * bitmap_s, int * bitmap_r, int s_num, int n_num, int r_num){
  int nationkey = -1;
  int regionkey = -1;
  for( int i = 0 ; i< s_num; i ++){
    nationkey = s_nationkey[i];
    regionkey = n_regionkey[nationkey];
    if (bitmap_r[regionkey] == 1){
      bitmap_s[i] = nationkey;
    }
    }
  }

int * setbitmapL(int * l_orderkey, float * l_extendedprice, float * l_discount,int * l_suppkey,  
  int * s_nationkey, int * c_custkey, int * c_nationkey, int * o_custkey,
  int * bitmap_s, int * bitmap_c,int * bitmap_n, int * bitmap_o, float * res,
  int l_num, int s_num, int c_num, int o_num
  ){
  /*step4: lineitem表记录扫描，
    按l_orderkey映射到order表位图，
    满足条件时映射到supplier向量，非空时映射到customer向量，
    判断supplier与customer向量对应位置的nationkey值是否相等，
    相等时计算sum(l_extendedprice * (1 - l_discount))表达式的值，
    映射到s_nationkey向量进行累加。*/
  int orderkey = -1;
  int suppkey = -1;
  int custkey = -1;
  int nationkey = -1;

  for( int i = 0; i< l_num; i++){
    orderkey = l_orderkey[i];
    if(bitmap_o[orderkey - 1] == 1){
      suppkey = l_suppkey[i];
      float tmp = bitmap_s[suppkey -1];
      if(bitmap_s[suppkey -1] != -1){
        custkey = o_custkey[orderkey-1];
        nationkey = c_nationkey[custkey-1];
        bitmap_c[custkey-1] = c_nationkey[custkey-1];
      }

      int bitmapc = bitmap_c[custkey - 1];
      int bitmaps = bitmap_s[suppkey -1];

      if(bitmap_c[custkey - 1] != -1 && 
      bitmap_c[custkey - 1] == bitmap_s[suppkey -1]) {
        res[bitmapc] += l_extendedprice[i] * (1- l_discount[i]);
      }
    }
  }
}

float runQuery(int * r_regionkey, int * s_nationkey,int * s_suppkey, 
    int * c_custkey, int * c_nationkey, 
    int * o_orderdate,int * o_custkey,int *  o_orderkey, 
    int * n_regionkey, int * n_nationkey, 
    float * l_extendedprice, float * l_discount, int * l_orderkey,int *  l_suppkey, 
    int  r_num, int s_num , int c_num, int o_num, int n_num, int l_num) {

    float time_query;
    chrono::high_resolution_clock::time_point st, finish;
    st = chrono::high_resolution_clock::now();

    int bitmap_r[r_num];
    memset(bitmap_r, -1, sizeof(int) * r_num);
    int bitmap_s[s_num] = {-1}; 
    memset(bitmap_s, -1, sizeof(int) * s_num);
    float res[n_num] = {0};

    int bitmap_c[c_num] = {-1};
    memset(bitmap_c, -1, sizeof(int) * c_num);

    int bitmap_n[n_num] = {-1};
    memset(bitmap_n, -1, sizeof(int) * n_num);
    int * bitmap_o = (int *) malloc(sizeof(int) * o_num);
    memset(bitmap_o,-1, sizeof(int) * o_num);

    /*step1: region表按r_name = '[REGION]'生成位图；*/
    setBitmapRegion(r_regionkey, bitmap_r, r_num, "=",2);

    /*step2: order表按照表达式生成位图*/
    setBitmapOrderdate(o_orderdate, bitmap_o, o_num, ">=", 19940101, "<", 19950101);

    /*step3: supplier创建向量，表按s_nationkey映射的r_regionkey映射region位图进行连接过滤，
    满足条件记录生成supplier表向量，存放指定region记录的s_nationkey*/
    setBitmapS(s_nationkey, n_nationkey, n_regionkey, r_regionkey,bitmap_s, bitmap_r,s_num, n_num, r_num);

    /*step4: lineitem表记录扫描，
    按l_orderkey映射到order表位图，
    满足条件时映射到supplier向量，非空时映射到customer向量，
    判断supplier与customer向量对应位置的nationkey值是否相等，
    相等时计算sum(l_extendedprice * (1 - l_discount))表达式的值，
    映射到s_nationkey向量进行累加。*/
    setbitmapL(l_orderkey, l_extendedprice, l_discount, l_suppkey,  
    s_nationkey,c_custkey, c_nationkey, o_custkey,
    bitmap_s, bitmap_c, bitmap_n, bitmap_o, res,
    l_num, s_num, c_num, o_num);
  
  finish = chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = finish - st;

  for(int i = 0; i < n_num ; i++){
    if(abs(res[i])> EPS)
      printf("%d : %f \n", i, res[i]);
  }

  cout << "Time Taken Total: " << diff.count() * 1000 << endl;


  return time_query;
}

/**
 * Main
 */
int main(int argc, char** argv)
{
  int num_trials          = 3;
//   int * r_name = loadColumn<int>("r_name", R_LEN);
  int * r_regionkey = loadColumn<int>("r_regionkey", R_LEN);

  int * s_nationkey = loadColumn<int>("s_nationkey", S_LEN);
  int * s_suppkey = loadColumn<int>("s_suppkkey", S_LEN);

  int * c_custkey = loadColumn<int>("c_custkey", C_LEN);
  int * c_nationkey = loadColumn<int>("c_nationkey", C_LEN);

  int * o_orderdate = loadColumn<int>("o_orderdate", O_LEN);
  int * o_custkey = loadColumn<int>("o_custkey", O_LEN);
  int * o_orderkey = loadColumn<int>("o_orderkey2", O_LEN);
  
//   int * n_name = loadColumn<int>("n_name", N_LEN);
  int * n_regionkey = loadColumn<int>("n_regionkey", N_LEN);
  int * n_nationkey = loadColumn<int>("n_nationkey", N_LEN);

  float * l_extendedprice = loadColumn<float>("l_extendedprice", L_LEN);
  float * l_discount = loadColumn<float>("l_discount", L_LEN);
  int * l_orderkey = loadColumn<int>("l_orderkey2", L_LEN);
  int * l_suppkey = loadColumn<int>("l_suppkey", L_LEN);

  cout << "** LOADED DATA **" << endl;

  for (int t = 0; t < num_trials; t++) {
    float time_query;
    time_query = runQuery(r_regionkey, s_nationkey,s_suppkey,c_custkey, c_nationkey,
    o_orderdate,o_custkey, o_orderkey, 
    n_regionkey, n_nationkey, 
    l_extendedprice, l_discount, l_orderkey, l_suppkey, 
    R_LEN, S_LEN, C_LEN, O_LEN, N_LEN, L_LEN);
    cout<< "{"
        << "\"query\":5" 
        << ",\"time_query\":" << time_query
        << "}" << endl;
  }

  return 0;
}


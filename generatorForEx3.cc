#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>
#include <iostream>
#define N 100000
/*
  Please, compile with -std=c++11 like below.
  % g++ -std=c++11 generatorForEx3.cc

  #Index
  in3-1: 昇順に出力
  in3-2: 順番に関係なく出力、限界値(ユーザ数=100000、書籍数=100000、評価数=100000)
  in3-3: 特定の1冊を避けて出力
  in3-4: 特定の1人を避けて出力
 */

//管理用クラス
class User{
public:
  User(){num=0;}
  User(int n){num=n;}
  void set_num(int n){ num=n; }
  void addData(int n){ evalist.push_back(n); }
  bool searchList(int);
  int getSize(){return evalist.size();}
  int getData(int n){
    /*std::cout << "[" << num << "] ";
    for(int i=0; i<evalist.size(); i++)
      std::cout << evalist[i] << " ";
      std::cout<<std::endl;*/
    return evalist[n];}
  void mSort(){mergeSort(evalist.begin(),evalist.end(),0,evalist.size());}
  void mergeSort(std::vector<int>::iterator, std::vector<int>::iterator, int, int);
  void merge(std::vector<int>::iterator, std::vector<int>::iterator, int, int, int);
private:
  int num;
  std::vector<int> evalist;
};

bool User::searchList(int n){
  for(int i=0; i<evalist.size(); i++)
    if(evalist[i]==n)return true;
  return false;
}

void User::mergeSort(std::vector<int>::iterator first, std::vector<int>::iterator last, int left, int right){
  int mid;

  if( left+1 < right ){
    mid = (left + right)/2;
    mergeSort(first, last, left, mid);
    mergeSort(first, last, mid+1, right);
    merge(first, last, left, mid, right);
  }
}

void User::merge(std::vector<int>::iterator first, std::vector<int>::iterator last, int L, int M, int R){
  int i, j, k;
  int n1 = M - L + 1;
  int n2 = R - M;
  std::vector<int> Left;
  std::vector<int> Right;

  for( i=0 ; i<n1 ; i++ ){
    Left.push_back( *(first+(L+i)) );
  }
  for( i=0 ; i<n2 ; i++){
    Right.push_back( *(first+(M+i+1)) );
  }

  i=0;
  j=0;

  for( k=L ; k<R ; k++ ){
    if( Left[i] <=  Right[j] ){
      *(first+k) = Left[i];
      i++;
    }
    else{
      *(first+k) = Right[j];
      j++;
    }
  } 
}
//管理用クラスおわり

//====================================
//空の配列を作成
std::vector<User> list;
void Makelist(int n){
  for(int i=0; i<n; i++){
    User temp(i+1);
    list.push_back(temp);
  }
}

//実数で範囲指定した乱数生成
double RangedRand( const double& a, const double& b ) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_real_distribution<double> rangedGen( a, b );
  return rangedGen(mt);
}

//昇順に出力する生成器
void AscendingOrder( const char* filename, int n, int m, int e ) {
  int u, b; // userNo. & bookNo.
  double eva;
  list.clear();
  Makelist(n);
  FILE* fpOut = fopen( filename, "w" );
  fprintf( fpOut, "%d %d %d\n", n, m, e);
  for ( int i=0; i<e; ++i ) {
    u = RangedRand(1.0, (double)n+1);
    b = RangedRand(1.0, (double)m+1);
    //std::cout << u << ", " << b << std::endl;
    if(!list[u-1].searchList(b)){
      list[u-1].addData(b);
    }else {
      i--;
      //std::cout << "continue" << std::endl;
      continue;
    }
  }
  // ソート作り直し
  // for( int i=0; i<n; i++){
  //    list[i].mSort();
  //  }
  for( int i=0; i<n; i++){
    //std::cout << "debug1: " << i << ", getSize: " << list[i].getSize() << std::endl;
    for( int j=0; j<list[i].getSize(); j++)
      fprintf( fpOut, "%d %d %2.1lf\n",i+1,list[i].getData(j),RangedRand( 0.0, 5.0 ));
  }
  fclose( fpOut );
}

//ランダムに出力する生成器
void NoOrder( const char* filename, int n, int m, int e ) {
  int u, b; // userNo. & bookNo.
  list.clear();
  Makelist(n);
  FILE* fpOut = fopen( filename, "w" );
  fprintf( fpOut, "%d %d %d\n", n, m, e);
  for ( int i=0; i<e; ++i ) {
    u = RangedRand(1.0, (double)n+1);
    b = RangedRand(1.0, (double)m+1);
    if(!list[u-1].searchList(b)){
      list[u-1].addData(b);
    }else {
      i--;
      continue;
    }
    fprintf( fpOut, "%d %d %2.1lf\n", u, b, RangedRand( 0.0, 5.0 ));
  }
  fclose( fpOut );
}

/*
//col番目だけ全員未評価のデータ生成器
void UnevaluatedColumn( const char* filename, int n, int m, int col ) {
  FILE* fpOut = fopen( filename, "w" );
  fprintf( fpOut, "%d %d\n", n, m );
  for ( int i=0; i<n; ++i ) {
    for ( int j=0; j<m; ++j ) {
      if ( j == col )
	fprintf( fpOut, "-1.0%c", j == m-1 ? '\n' : ' ' );
      else
	fprintf( fpOut, "%2.1lf%c",
		 RangedRand( 0.0, 5.0 ), j == m-1 ? '\n' : ' ' );
    }
  }
  fclose( fpOut );
}

//i番目の人だけ全て未評価のデータ生成器
void UnevaluatedUser( const char* filename, int n, int m, int user ) {
  FILE* fpOut = fopen( filename, "w" );
  fprintf( fpOut, "%d %d\n", n, m );
  for ( int i=0; i<n; ++i ) {
    for ( int j=0; j<m; ++j ) {
      if ( i == user )
	fprintf( fpOut, "-1.0%c", j == m-1 ? '\n' : ' ' );
      else
	fprintf( fpOut, "%2.1lf%c",
		 RangedRand( 0.0, 5.0 ), j == m-1 ? '\n' : ' ' );
    }
  }
  fclose( fpOut );
}
*/
//mainから生成器を呼び出して、まとめてデータを作る
int main() {
  AscendingOrder( "in3-1", 10, 15, 100 );
  NoOrder( "in3-2", 10000, 10000, 10000 );
  //UnevaluatedColumn( "in3-3", 10, 15, 3 );
  //UnevaluatedUser( "in3-4", 10, 15, 1);
  //EvaluatedOnly( "", 2, 1 );
  //Mixed( "03_min_02.in", 2, 1, 2 );
  //Mixed( "03_min_03.in", 2, 1, 2 );
  //EvaluatedOnly( "04_max_01.in", 100, 100 );
  //Mixed( "04_max_02.in", 100, 100, 4 );
  //UnevaluatedColumn( "04_max_03.in", 100, 100, 0 );
  return 0;
}

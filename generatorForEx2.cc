#include <cstdio>
#include <cstdlib>
#include <random>

/*
  Please, compile with -std=c++11 like below.
  % g++ -std=c++1 generatorForEx2.cc

  This code is completery same as the Ex1.

  #Index
  in2-1: 全て評価済、限界値(ユーザ数=100, 書籍数=100)
  in2-2: 一定の割合で-1を出力
  in2-3: ある1冊だけ全て未評価
  in2-4: ある1人だけ全て未評価
 */



//実数で範囲指定した乱数生成
double RangedRand( const double& a, const double& b ) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_real_distribution<double> rangedGen( a, b );
  return rangedGen(mt);
}

//評価済みのものしか出力しない生成器
void EvaluatedOnly( const char* filename, int n, int m ) {
  FILE* fpOut = fopen( filename, "w" );
  fprintf( fpOut, "%d %d\n", n, m );
  for ( int i=0; i<n; ++i ) {
    for ( int j=0; j<m; ++j ) {
      fprintf( fpOut, "%2.1lf%c",
	       RangedRand( 0.0, 5.0 ), j == m-1 ? '\n' : ' ' );
    }
  }
  fclose( fpOut );
}

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

//freq回に1回の割合で-1を出力する生成器
void Mixed( const char* filename, int n, int m, int freq ) {
  FILE* fpOut = fopen( filename, "w" );
  fprintf( fpOut, "%d %d\n", n, m );
  for ( int i=0; i<n; ++i ) {
    for ( int j=0; j<m; ++j ) {
      if ( rand()%freq == 0 )
	fprintf( fpOut, "-1.0%c", j == m-1 ? '\n' : ' ' );
      else
	fprintf( fpOut, "%2.1lf%c",
		 RangedRand( 0.0, 5.0 ), j == m-1 ? '\n' : ' ' );
    }
  }
  fclose( fpOut );
}

//mainから生成器を呼び出して、まとめてデータを作る
int main() {
  EvaluatedOnly( "in2-1", 100, 100 );
  Mixed( "in2-2", 10, 15, 10 );
  UnevaluatedColumn( "in2-3", 10, 15, 3 );
  UnevaluatedUser( "in2-4", 10, 15, 1);
  //EvaluatedOnly( "", 2, 1 );
  //Mixed( "03_min_02.in", 2, 1, 2 );
  //Mixed( "03_min_03.in", 2, 1, 2 );
  //EvaluatedOnly( "04_max_01.in", 100, 100 );
  //Mixed( "04_max_02.in", 100, 100, 4 );
  //UnevaluatedColumn( "04_max_03.in", 100, 100, 0 );
  return 0;
}

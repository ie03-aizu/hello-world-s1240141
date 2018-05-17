#include<iostream>
#include<vector>
#include<complex>

int N;  //User数
int M;  //書籍数

class Userinfo{
public:
  Userinfo();                                       // デフォルトコンストラクタ
  Userinfo( std::istream& );
  std::istream& read( std::istream& );         // データ設定用
  void set_num( int );                         // 何人目のユーザーかの設定
  void Evaluate(Userinfo);                   // 類似性評価用
  
  double getevaluate(){return evaluate;}                  // 類似性のデータ取得
  double getBooks(int x){return Booksinfo[x];}           // j番目の本に対する評価を得る
  int get_num(){return num;}                             // numのデータ取得


private:
  std::vector<double> Booksinfo;  // 書籍の情報
  int num;                        // 何番目の人物か
  double evaluate;                // ユーザー1との類似性の評価
};


int main(){
  std::vector<Userinfo> User;
  
  std::cin >> N >> M;  //User数と書籍数の登録

  for( int i=0 ; i<N ; ++i ){
    Userinfo userinfo;
    
    userinfo.read( std::cin );  // Userinfoの型にデータを登録
    userinfo.set_num(i+1);      // 何人目のユーザーかの設定    
    User.push_back( userinfo );    // Userinfo型の配列に格納
  }

  // 全ユーザーの全書籍に対する評価計算
  for( int i=1 ; i < N ; ++i ){
    User[i].Evaluate(User[0]); 
  }
  
  // バブルソート
  for( int i=1 ; i < N-1 ; i++ ){
    for( int j = i ; j < N-2 ; j++ ){
      if( User[j].getevaluate() < User[j+1].getevaluate() ){
	Userinfo tmp = User[j];

	User[j] = User[j+1];
	User[j+1] = tmp;
      }
    }
  }

  // 類似性評価出力
  for( int i = 1 ; i < N ; i++ ){
    std::cout << User[i].get_num() << " " << User[i].getevaluate() << std::endl;
  }
  
  return 0;
}

//デフォルトコンストラクタ
Userinfo::Userinfo(){
  Booksinfo.push_back(0);
  num = 0;
}

//コンストラクタ1
Userinfo::Userinfo( std::istream& is ){ read( is ); }

//データの読込用関数
std::istream& Userinfo::read( std::istream& is ){
  double score;
  Booksinfo.clear();

  for( int i=0 ; i < M ; ++i ){
    is >> score;
    Booksinfo.push_back(score);
  }

  return is;
}

void Userinfo::set_num( int x ){
  num = x;
}

// 類似性の評価関数
void Userinfo::Evaluate(Userinfo u1){

  double sum=0;
  double x;
 
  for( int j=0 ; j<M ; ++j ){
    if(Booksinfo[j] == -1 || u1.getBooks(j) == -1) continue;
    
    x = (Booksinfo[j] - u1.getBooks(j));
    sum += x*x; 
  }

  evaluate =  1/(std::sqrt(sum)+1);
}

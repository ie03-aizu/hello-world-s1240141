#include<iostream>
#include<vector>
#include<complex>
#include<iterator>

int N;  //User数
int M;  //書籍数
int E;  //評価数

class Userinfo{
public:
  Userinfo();                                  // デフォルトコンストラクタ
  Userinfo( std::istream& );                   // コンストラクタ1
  Userinfo(int, int);                          // コンストラクタ2
  std::istream& read( std::istream& );         // データ設定用
  void set_num( int );                         // 何人目のユーザーかの設定
  void set_Books( int, double );               // int 番目の本の評価の設定
  void set_Evaluate( double x){ evaluate = x;}  // 手動での評価設定
  void Evaluate(Userinfo);                   // 類似性評価用
  
  double getevaluate(){return evaluate;}                  // 類似性のデータ取得
  double getBooks(int x){return Booksinfo[x];}           // x番目の本に対する評価を得る
  int get_num(){return num;}                             // numのデータ取得


private:
  std::vector<double> Booksinfo;  // 書籍の情報
  int num;                        // 何番目の人物か
  double evaluate;                // ユーザー1との類似性の評価
};


class Recitem{
public:
  Recitem(){num = 0; rec = 0;}            //デフォルトコンストラクタ
  Recitem(int x, double j){ num = x; rec = j; }         //コンストラクタ1
  void set_num( int x ){ num = x; }      
  void Recommendation( std::vector<Userinfo> );
  int get_num(){return num;}
  double get_rec(){return rec;}

private:
  int num;
  double rec;
};

// User数と書籍数から、それに応じた初期状態のリストを作る
std::vector<Userinfo> MakeList(int, int);

// マージソート
void mergeSort(std::vector<Recitem>::iterator, std::vector<Recitem>::iterator, int ,int);
void merge(std::vector<Recitem>::iterator, std::vector<Recitem>::iterator, int, int, int);

std::vector<Userinfo> User;
std::vector<Recitem> Items;

int main(){

  int num, book;
  double eval;
  
  std::cin >> N >> M >> E;  //User数と書籍数と評価数の登録

  User = MakeList(N, M);

  for( int i=0 ; i<E ; ++i ){
    std::cin >> num >> book >> eval;

    User[num-1].set_Books( book-1, eval );
  }

    /*
  for( int i=0 ; i<E ; ++i ){
    Userinfo userinfo;
    
    userinfo.read( std::cin );  // Userinfoの型にデータを登録
    userinfo.set_num(i+1);      // 何人目のユーザーかの設定    
    User.push_back( userinfo );    // Userinfo型の配列に格納
  }
    */

  // 全ユーザーの全書籍に対する評価計算
  for( int i=1 ; i < N ; ++i ){
    User[i].Evaluate(User[0]); 
  }

  /*
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
*/
  // アイテムの推薦
  for( int i = 0 ; i < M ; i++ ){
    if( User[0].getBooks(i) == -1 ){
      Recitem item;
      
      item.set_num(i+1);
      item.Recommendation( User );

      
      Items.push_back(item); 
    }
  }
      
  // 推薦度のソート

  mergeSort(Items.begin(), Items.end(), 0, Items.size());  

/*
  for( int i=0 ; i < Items.size() ; i++ ){
    for( int j = Items.size()-1 ; j > i ; j-- ){
      if(Items[j-1].get_rec() < Items[j].get_rec() ){
	Recitem tmp = Items[j-1];
	Items[j-1] = Items[j];
	Items[j] = tmp;
      }
    }
  }
*/
  //推薦度の出力
  
  for( int i=0 ; i < Items.size() ; i++ ){
    std::cout << Items[i].get_num() << " " << Items[i].get_rec() << std::endl;
  }

  

  /* 類似性評価出力
  for( int i = 1 ; i < N ; i++ ){
    std::cout << User[i].get_num() << " " << User[i].getevaluate() << std::endl;
    }
  */
  return 0;
}

//デフォルトコンストラクタ
Userinfo::Userinfo(){
  Booksinfo.push_back(-1);
  num = 0;
  evaluate = 0;
}


// コンストラクタ1
Userinfo::Userinfo( std::istream& is ){ read( is ); }

// コンストラクタ2
Userinfo::Userinfo( int x, int y ){
  num = x;
  
  for( int i=0 ; i < y ; ++i ){
    Booksinfo.push_back(-1);  
  }
}

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

// 何人目のユーザーかの設定
void Userinfo::set_num( int x ){
  num = x;
}

// int 番目の本の評価の設定
void Userinfo::set_Books( int book, double eval ){
  Booksinfo[book] = eval;
}


// リストの作成
std::vector<Userinfo> MakeList(int x, int y){
  std::vector<Userinfo> List;

  for( int i=0 ; i < x ; ++i ){
    Userinfo userinfo(i+1, y);
    List.push_back(userinfo);
  }

  return List;
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


// 推薦度の算出
void Recitem::Recommendation( std::vector<Userinfo> U ){
  double S=0;
  double A=0;

  for( int i = 1 ; i < N ; i++ ){
    if( U[i].getBooks(num-1) != -1 ){
      S += U[i].getevaluate() * U[i].getBooks(num-1);
      A += U[i].getevaluate();
    }
  }

  rec = S/A;
}

// マージソート
void mergeSort(std::vector<Recitem>::iterator first, std::vector<Recitem>::iterator last, int left, int right){
  int mid;

  if( left+1 < right ){
    mid = (left + right)/2;
    mergeSort(first, last, left, mid);
    mergeSort(first, last, mid, right);
    merge(first, last, left, mid, right);
  }
}
void merge(std::vector<Recitem>::iterator first, std::vector<Recitem>::iterator last, int L, int M, int R){
  int i, j, k;
  int n1 = M - L;
  int n2 = R - M;
  std::vector<Recitem> Left;
  std::vector<Recitem> Right;

  for( i=0 ; i<n1 ; i++ ){
    Left.push_back( *(first+(L+i)) );
  }
  for( i=0 ; i<n2 ; i++){
    Right.push_back( *(first+(M+i)) );
  }

  Recitem item = Recitem();
  Left.push_back(item);
  Right.push_back(item);

  i=0;
  j=0;

  for( k=L ; k<R ; k++ ){
    if( Left[i].get_rec() > Right[j].get_rec() ){
      *(first+k) = Left[i];
      i++;
    }
    else{
      *(first+k) = Right[j];
      j++;
    }
  } 
}













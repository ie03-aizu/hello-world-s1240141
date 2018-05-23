#include<iostream>
#include<vector>
#include<complex>
#include<iterator>

int N;  //User数
int M;  //書籍数
int E;  //評価数
int R;  //友好関係数

class Userinfo{
public:
  Userinfo();                                             // デフォルトコンストラクタ
  Userinfo(int, int, int);                                // コンストラクタ
  void set_num( int x ){ num = x;}                        // 何人目のユーザーかの設定
  void set_Books( int x, double y ){ Booksinfo[x] = y; }  // int 番目の本の評価の設定
  void set_Evaluate( double x){ evaluate = x;}            // 手動での評価設定
  void set_Friends(int x){Friends[x] = 1;}                // x番目のユーザーと友人関係であることの設定
  void Evaluate(Userinfo);                                // 類似性評価用
  
  double getevaluate(){return evaluate;}                  // 類似性のデータ取得
  double getBooks(int x){return Booksinfo[x];}            // x番目の本に対する評価の取得
  int get_num(){return num;}                              // numのデータ取得
  int get_Friends(int x){return Friends[x];}              // x番目のユーザーとの友好関係の取得
 
private:
  std::vector<double> Booksinfo;  // 書籍の情報
  std::vector<int> Friends;       // 有効関係の登録 1ならtrue, 0ならfalse
  int num;                        // 何番目の人物か
  double evaluate;                // ユーザー1との類似性の評価
};

class Recitem{
public:
  Recitem(){num = 0; rec = 0;}                          // デフォルトコンストラクタ
  Recitem(int x, double j){ num = x; rec = j; }         // コンストラクタ
  void set_num( int x ){ num = x; }                     // int番目の本と設定
  void F_Recommendation( std::vector<Userinfo>,int  x );// 友人関係を考慮したint番目のユーザーに対するオススメ度の計算
  
  int get_num(){return num;}
  double get_rec(){return rec;}

private:
  int num;
  double rec;
};

// User数と書籍数から、それに応じた初期状態のリストを作る
std::vector<Userinfo> MakeList(int, int);

// 推薦度のマージソート
void mergeSort(std::vector<Recitem>::iterator, int ,int);
void merge(std::vector<Recitem>::iterator, int, int, int);


std::vector<Userinfo> User;
std::vector<Recitem> Items;

int main(){

  int num, book, friend_num;
  double eval;
  
  std::cin >> N >> M >> E;  //User数と書籍数と評価数の登録

  User = MakeList(N, M);    //初期状態のリストの作成

  
  // データの設定
  for( int i=0 ; i<E ; ++i ){
    std::cin >> num >> book >> eval;

    User[num-1].set_Books( book-1, eval );
  }

  std::cin >> R;            //友好関係数の登録

  for( int i=0 ; i<R ; ++i ){
    std::cin >> num >> friend_num;
    
    User[num-1].set_Friends(friend_num-1);
    User[friend_num-1].set_Friends(num-1);
  }  
  
   // ユーザーnumに対する全ユーザーの全書籍に対する評価計算
  num = 1;
  for( int i=0 ; i < N ; ++i ){
    if( i == num-1 ) continue;
    User[i].Evaluate(User[num-1]); 
  }
  
  std::cout << "user2" << " : " << User[1].getevaluate() << std::endl;
  std::cout << "user4" << " : " << User[3].getevaluate() << std::endl;
  std::cout << "user6" << " : " << User[5].getevaluate() << std::endl;
  
  // ユーザーnumに対するアイテムの推薦
  num = 1;
  for( int i=0 ; i < M ; ++i ){
    if( User[num-1].getBooks(i) == -1 ){
      Recitem item;
      
      item.set_num(i+1);
      item.F_Recommendation( User , num-1);
      
      Items.push_back(item); 
    }
  }


  
  // 推薦度のソート
  mergeSort(Items.begin(), 0, Items.size());  

  
  //推薦度の出力
  for( int i=0 ; i < Items.size() ; i++ ){
    std::cout << Items[i].get_num() << " " << Items[i].get_rec() << std::endl;
  }
  
  return 0;
}

// デフォルトコンストラクタ
Userinfo::Userinfo(){
  Booksinfo.push_back(-1);
  Friends.push_back(0);
  num = 0;
  evaluate = 0;
}

// コンストラクタ
Userinfo::Userinfo( int x, int y, int z ){
  num = x;

  // 本の情報の初期化
  for( int i=0 ; i < y ; ++i ){
    Booksinfo.push_back(-1);  
  }

  // 友人関係の初期化
  for( int i=0 ; i < z ; ++i ){
    Friends.push_back(0);
  }
}

// u1との類似性評価関数
void Userinfo::Evaluate(Userinfo u1){

  double sum=0;
  double x;
 
  for( int j=0 ; j<M ; ++j ){
    if(Booksinfo[j] == -1 || u1.getBooks(j) == -1) continue;
    
    x = (Booksinfo[j] - u1.getBooks(j));
    sum += x*x; 
  }

  if(sum == 0) evaluate = 0;
  else evaluate =  1/(std::sqrt(sum)+1);
}


// 推薦度の算出
void Recitem::F_Recommendation( std::vector<Userinfo> U, int x ){
  double S=0;
  double A=0;
  
  for( int i = 0 ; i < N ; i++ ){
    if( i == x )continue;
    
    if( U[i].getBooks(num-1) != -1 && U[x].get_Friends(i) == 1){
      
      //std::cout << num << ":" << i+1 << std::endl;
      S += U[i].getevaluate() * U[i].getBooks(num-1);
      A += U[i].getevaluate();
    }
  }

  rec = S/A;
}


// 初期状態のリストの作成
std::vector<Userinfo> MakeList(int x, int y){
  std::vector<Userinfo> List;

  for( int i=0 ; i < x ; ++i ){
    Userinfo userinfo(i+1, y, x);
    List.push_back(userinfo);
  }

  return List;
}


// マージソート
void mergeSort(std::vector<Recitem>::iterator first, int left, int right){
  int mid;

  if( left+1 < right ){
    mid = (left + right)/2;
    mergeSort(first, left, mid);
    mergeSort(first, mid, right);
    merge(first, left, mid, right);
  }
}
void merge(std::vector<Recitem>::iterator first, int L, int M, int R){
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

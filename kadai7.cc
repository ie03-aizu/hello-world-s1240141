#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>

#define K 20   // スコアの計算回数

int N;  // ドキュメント数

class HTML{
public:
  HTML(){name = ""; score = -1;}                     // デフォルトコンストラクタ
  HTML(std::string Name){name = Name; score = -1;}   // コンストラクタ
  
  void add_link(HTML html){link.push_back(html);}   // linksに変数の追加
  void score_Reset(){score = 1;}                           // スコアのリセット
  void PageRank(std::vector<HTML>);                        // スコアの計算
  
  std::string get_name(){return name;}                     // 名前の取得
  std::vector<HTML> get_links(){return link;}              // リンクの配列の取得
  double get_score(){return score;}                        // スコアの取得
private:
  std::string name;                            // htmlページの名前
  std::vector<HTML> link;                      // リンクしているhtmlページの配列
  double score;                                // 重要度のスコア
};


// スコアを用いたHTMLのマージソート
void mergeSort(std::vector<HTML>::iterator, int ,int);
void merge(std::vector<HTML>::iterator, int, int, int);

int main(){
  std::vector<HTML> HTMLs;
  std::string name;
  
  // htmlファイルのリストの作成
  std::cin  >> N;  // ドキュメント数の登録

  if( N > 100000 || N < 0 ){
    printf("Error! N is lower than 100001 and higher than 0.");
    return 0;
  }
  
  for( int i=0 ; i<N ; i++ ){
    std::cin >> name;

    HTML html(name + ".html"); 
    HTMLs.push_back(html);  // 名前のみのリストの作成
  }
  //======================
  
  // リストにデータを反映

  std::vector< std::vector<std::string> > arr; //リンク先の名前を一時的に保管する
  arr.resize(N);
  int count;     // カウント変数
  
  for( int i=0 ; i<N ; i++ ){
    arr[i].resize(10);
    count=0;
    // 対象のファイルの読み出し
    std::ifstream file;
    std::string S = "/home/student/s1240141/Software1/phase2/source/" + HTMLs[i].get_name();
    const char *place = S.c_str();
  
    file.open(place);
    if(!file){
      std::cout << "Error! Can't open file " + HTMLs[i].get_name() << std::endl;
    }

    // リンク先の名前の保存
    std::string target = ".html";
    std::string str;
       // 1文ずつ取得し、取得できなくなるまでループ
    while(file >> str){
      // 取得した文の末尾が".html"の場合、リンク先として登録するために名前の保存
      if(str.find(target, str.size() - target.size()) != std::string::npos){
	arr[i][count] = str;
	count++;
      }
    }
  }

  // リンク先の登録
  for( int i=0 ; i<N ; i++ ){
    for( int j=0 ; j<arr[i].size() ; j++ ){
      for( int k=0 ; k<N ; k++ ){
	if( arr[i][j].compare(HTMLs[k].get_name()) == 0 ){
	  HTMLs[i].add_link(HTMLs[k]);  // 先に保存した名前と一致するHTMLページをリンク先として登録
	}   
      }
    }
  }
  

  //====================

  // スコアの計算

  for( int i=0 ; i<N ; i++ ){   // 全htmlページのスコアのリセット
    HTMLs[i].score_Reset();
  }

  for( int i=0 ; i<K ; i++ ){
    for( int j=0 ; j<N ; j++)
      HTMLs[j].PageRank(HTMLs);
  }

  //===================


  // マージソート
  mergeSort(HTMLs.begin(), 0, HTMLs.size());  

  // スコアの出力
  for( int i=0 ; i<N ; i++ ){
    std::cout << std::fixed;
    std::cout << std::setprecision(2) <<HTMLs[i].get_score() << " " << HTMLs[i].get_name() << std::endl;
  }

  
  /*
  std::vector<HTML> links;

  
  for( int i=0 ; i<N ; i++ ){
    links = HTMLs[i].get_links();
    std::cout << HTMLs[i].get_name() << ": ";
    for( int j=0 ; j<links.size() ; j++ ){
      std::cout << links[j].get_name() << "  ";
    }
    std::cout << std::endl;
  }
  */
  return 0;
}

// スコア計算用関数
void HTML::PageRank(std::vector<HTML> HTMLs){
  double link_sum=0;
  std::vector<HTML> links;

  // リンクされているページの捜索及びスコアの計算
  for( int i=0 ; i<N ; i++ ){
    if(HTMLs[i].get_name().compare(name) == 0) continue;
    
    links = HTMLs[i].get_links();
    for( int j=0 ; j<links.size() ; j++ ){
      if(links[j].get_name().compare(name) == 0){
	link_sum += HTMLs[i].get_score()/links.size(); 
      }
    }
  }

  score = 0.15 + 0.85 * link_sum;
}


// マージソート
void mergeSort(std::vector<HTML>::iterator first, int left, int right){
  int mid;

  if( left+1 < right ){
    mid = (left + right)/2;
    mergeSort(first, left, mid);
    mergeSort(first, mid, right);
    merge(first, left, mid, right);
  }
}
void merge(std::vector<HTML>::iterator first, int L, int M, int R){
  int i, j, k;
  int n1 = M - L;
  int n2 = R - M;
  std::vector<HTML> Left;
  std::vector<HTML> Right;

  for( i=0 ; i<n1 ; i++ ){
    Left.push_back( *(first+(L+i)) );
  }
  for( i=0 ; i<n2 ; i++){
    Right.push_back( *(first+(M+i)) );
  }

  HTML html = HTML();
  Left.push_back(html);
  Right.push_back(html);

  i=0;
  j=0;

  for( k=L ; k<R ; k++ ){
    if( Left[i].get_score() > Right[j].get_score() ){
      *(first+k) = Left[i];
      i++;
    }
    else{
      *(first+k) = Right[j];
      j++;
    }
  } 
}

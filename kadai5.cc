#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>

class FriendData{
public:
  FriendData(int n){ usernum = n; group = 0;}
  int get_group(){return group;}
  void set_group(int n){group = n;}
  int get_usernum(){return usernum;}
private:
  int group;
  int usernum;
};

static const int MAX = 100000;
std::vector<FriendData> Friend_data;
std::vector<int> G[MAX];

std::vector<FriendData> MakeList(int);
void dfs(int, int);
void assignGroup(int);


int main(){

  // ===========
  // 友達情報入力
  int n, r; // ユーザの数、友達関係の数
  std::cin >> n >> r;

  Friend_data = MakeList(n);
  
  for(int i=0; i<r; i++){
    int s,t; // (仮)ユーザs、(仮)ユーザt
    std::cin >> s >> t;
    G[s-1].push_back(t);
    G[t-1].push_back(s);    
  }// 友達情報入力おわり

  // グループ化
  assignGroup(n);
  
  // ============
  // 友達問い合わせ
  int q;
  std::cin >> q;
  for(int i=0; i < q; i++){
    int s, t; // (仮)ユーザs、(仮)ユーザt
    std::cin >> s >> t;
    if((Friend_data[s-1].get_group() != 0 && Friend_data[t-1].get_group() != 0) &&
       (Friend_data[s-1].get_group() == Friend_data[t-1].get_group())){
      std::cout << "yes" << std::endl;
    }
    else std::cout << "no" << std::endl;
  }// 友達問い合わせおわり

  return 0;
}

// 空の配列を作成
std::vector<FriendData> MakeList(int n){
  std::vector<FriendData> list;

  for(int i=1; i<=n; i++){
    FriendData frienddata(i);
    list.push_back(frienddata);
  }
  return  list;
}

// 深さ優先探索
void dfs(int n, int c){
  std::stack<int> s;
  s.push(n);
  Friend_data[n].set_group(c);
  while(!s.empty()){
    int u = s.top();
    s.pop();
    for(int i=0; i<G[u].size(); i++){
      int v = G[u][i];
      if(Friend_data[v-1].get_group() == 0){
	Friend_data[v-1].set_group(c);
	s.push(v-1);
      }
    }
  }
}


void assignGroup(int n){
  int id = 1;
  for(int i=0; i<n; i++)
    if(Friend_data[i].get_group() == 0)dfs(i, id++);
}

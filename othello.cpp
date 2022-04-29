#include <iostream>
#include <ios>
#include <iomanip>
#include <map>

const int Board_size = 8;
const int Empty = 0;
const int Black = 1;
const int White = -1;
const int Wall = 2;
const int Wall_num[] = {1,2,3,4,5,6,7,8};

const int Left = 1;
const int Left_Upper = 2;
const int Upper = 4;
const int Right_Upper = 8;
const int Right = 16;
const int Right_Down = 32;
const int Down = 64;
const int Left_Down = 128;

class Othello{
  public:
    //盤。
    int board[Board_size+2][Board_size+2];
    //裏返せる方向。
    int moveable_direction[Board_size+2][Board_size+2];
    //置ける位置。
    int moveable_position[Board_size+2][Board_size+2];
    int turns;
    int pass_count;
    int current_color;

    //constructor
    Othello();

    //石を置くメソッド。
    bool move(int x,int y);

    //石を裏返すメソッド。
    void flip(int x,int y);

    //どの方向に裏返せるかを返すメソッド。
    int check_moveable(int x,int y,int color);

    //moveable_directionとmoveable_positionを更新するメソッド。
    void init_moveable();

    //石と盤を表示するメソッド。
    void display_board();

    //終了したかを判定するメソッド。
    bool identify_end();

    //パスするメソッド。
    void player_pass();

};


//constructor
Othello::Othello(){
  for(int i=0; i<Board_size+2; i++){
    for(int j=0; j<Board_size+2; j++){
      this->board[i][j] = 0;
      this->moveable_direction[i][j] = 0;
      this->moveable_position[i][j] = 0;
    }
  }

  for(int i=0; i<Board_size+2; i++){
    this->board[0][i] = Wall;
    this->board[i][0] = Wall;
    this->board[Board_size+1][i] = Wall;
    this->board[i][Board_size+1] = Wall;
  }

  this->board[4][4] = White;
  this->board[5][5] = White;
  this->board[4][5] = Black;
  this->board[5][4] = Black;

  this->turns = 0;
  this->current_color = Black;
  this->pass_count = 0;

  //moveable_directionとmoveable_positionの初期化。
  this->init_moveable();

}

//石を置くメソッド。
bool Othello::move(int x,int y){
  if(x < 1 || Board_size < x){
    std::cout << "Horizontal axis is out of size" << std::endl;
    return false;
  }
  if(y < 1 || Board_size < y){
    std::cout << "Vertical axis is out of size" << std::endl;
    return false;
  }
  //その座標に石を置けるかどうかの確認。
  if(this->moveable_position[y][x] == false){
    std::cout << "(" << x << "," << y << ")" << " can not be placed" << std::endl;
    return false;
  }

  //石を裏返すメソッド。
  this->flip(x,y);

  this->turns += 1;

  this->current_color = -this->current_color;

  //moveable_directionとmoveable_positionの更新。
  this->init_moveable();

  this->display_board();

  return true;
}


//石を裏返すメソッド。
void Othello::flip(int x,int y){
  this->board[y][x] = this->current_color;
  int direction = this->moveable_direction[y][x];

  if(direction & Left){
    int y_tmp = y;
    int x_tmp = x - 1;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      x_tmp -= 1;
    }
  }

  if(direction & Left_Upper){
    int y_tmp = y - 1;
    int x_tmp = x - 1;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      y_tmp -= 1;
      x_tmp -= 1;
    }
  }

  if(direction & Upper){
    int y_tmp = y - 1;
    int x_tmp = x;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      y_tmp -= 1;
    }
  }

  if(direction & Right_Upper){
    int y_tmp = y - 1;
    int x_tmp = x + 1;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      y_tmp -= 1;
      x_tmp += 1;
    }
  }

  if(direction & Right){
    int y_tmp = y;
    int x_tmp = x + 1;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      x_tmp += 1;
    }
  }

  if(direction & Right_Down){
    int y_tmp = y + 1;
    int x_tmp = x + 1;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      y_tmp += 1;
      x_tmp += 1;
    }
  }

  if(direction & Down){
    int y_tmp = y + 1;
    int x_tmp = x;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      y_tmp += 1;
    }
  }

  if(direction & Left_Down){
    int y_tmp = y + 1;
    int x_tmp = x - 1;
    while(this->board[y_tmp][x_tmp] == -this->current_color){
      this->board[y_tmp][x_tmp] = this->current_color;
      y_tmp += 1;
      x_tmp -= 1;
    }
  }

}


//どの方向に裏返せるかを返すメソッド。
int Othello::check_moveable(int x,int y,int color){
  int direction = 0;

  if(this->board[y][x] != Empty){
    return 0; //falseにしたかったが、c++の仕様上無理なので０とした。
  }

  //左。
  if(this->board[y][x-1] == -color){
    int y_tmp = y;
    int x_tmp = x - 2;
    while(this->board[y_tmp][x_tmp] == -color){
      x_tmp -= 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Left;
    }
  }

  //左上。
  if(this->board[y-1][x-1] == -color){
    int y_tmp = y - 2;
    int x_tmp = x - 2;
    while(this->board[y_tmp][x_tmp] == -color){
      y_tmp -= 1;
      x_tmp -= 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Left_Upper;
    }
  }

  //上。
  if(this->board[y-1][x] == -color){
    int y_tmp = y - 2;
    int x_tmp = x;
    while(this->board[y_tmp][x_tmp] == -color){
      y_tmp -= 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Upper;
    }
  }

  //右上。
  if(this->board[y-1][x+1] == -color){
    int y_tmp = y - 2;
    int x_tmp = x + 2;
    while(this->board[y_tmp][x_tmp] == -color){
      y_tmp -= 1;
      x_tmp += 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Right_Upper;
    }
  }

  //右。
  if(this->board[y][x+1] == -color){
    int y_tmp = y;
    int x_tmp = x + 2;
    while(this->board[y_tmp][x_tmp] == -color){
      x_tmp += 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Right;
    }
  }

  //右下。
  if(this->board[y+1][x+1] == -color){
    int y_tmp = y + 2;
    int x_tmp = x + 2;
    while(this->board[y_tmp][x_tmp] == -color){
      y_tmp += 1;
      x_tmp += 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Right_Down;
    }
  }

  //下。
  if(this->board[y+1][x] == -color){
    int y_tmp = y + 2;
    int x_tmp = x;
    while(this->board[y_tmp][x_tmp] == -color){
      y_tmp += 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Down;
    }
  }

  //左下。
  if(this->board[y+1][x-1] == -color){
    int y_tmp = y + 2;
    int x_tmp = x - 2;
    while(this->board[y_tmp][x_tmp] == -color){
      y_tmp += 1;
      x_tmp -= 1;
    }
    if(this->board[y_tmp][x_tmp] == color){
      direction |= Left_Down;
    }
  }

  return direction;

}


//moveable_directionとmoveable_positionの更新。
void Othello::init_moveable(){
  for(int i=0; i<Board_size+2; i++){
    for(int j=0; j<Board_size+2; j++){
      this->moveable_position[i][j] = 0;
    }
  }

  for(int x=1; x<Board_size+1; x++){
    for(int y=1; y<Board_size+1; y++){
      int dir = this->check_moveable(x,y,this->current_color);
      this->moveable_direction[y][x] = dir;
      if(dir != 0){
        this->moveable_position[y][x] = 1;
      }
    }
  }
}


void Othello::display_board(){
  std::cout << "  "; //微調整用。
  for(int i=0; i<Board_size; i++){
    std::cout << std::right << std::setw(2) << Wall_num[i] << " ";
  }
  std::cout << "\n";

  int n = 0;
  for(int i=1; i<Board_size+1; i++){
    std::cout << Wall_num[n];
    n += 1;
    for(int j=1; j<Board_size+1; j++){
      if(this->board[i][j] == Empty){
        std::cout << std::right << std::setw(5) << "□";
      } else if(this->board[i][j] == Black){
        std::cout << std::right << std::setw(4) << "〇";
      } else {
        std::cout << std::right << std::setw(4) << "●";
      }
    }
    std::cout << "\n";
  }
}


//終了したかどうかを判定するメソッド。
bool Othello::identify_end(){
  int count_black = 0;
  int count_white = 0;
  int count_empty = 0;

  if(this->pass_count == 2){
    for(int i=1; i<Board_size+1; i++){
      for(int j=1; j<Board_size+1; j++){
        if(this->board[i][j] == White){
          count_white += 1;
        } else if(this->board[i][j] == Black){
          count_black += 1;
        }
      }
    }
    std::cout << "Black = " << count_black << " White = " << count_white << std::endl;
    std::cout << "Game End!" << std::endl;;
    return true;
  }

  for(int i=1; i<Board_size+1; i++){
    for(int j=1; j<Board_size+1; j++){
      if(this->board[i][j] == Black){
        count_black += 1;
      } else if(this->board[i][j] == White){
        count_white += 1;
      } else {
        count_empty += 1;
      }
    }
  }

  if(count_empty == 0 || count_black == 0 || count_white == 0){
    std::cout << "Black = " << count_black << " White = " << count_white << std::endl;
    std::cout << "Game End!" << std::endl;
    return true;
  } else {
    return false;
  }
}


//パスするメソッド。
void Othello::player_pass(){
  //ここエラー
  std::map<int,std::string> player_color = {{-1,"White"},{1,"Black"}};
  std::cout << player_color[this->current_color] << " passes" << std::endl;

  this->current_color = -this->current_color;
  this->turns += 1;
  this->init_moveable();
}


int main(void){
  std::map<int,std::string> player_color = {{-1,"White"},{1,"Black"}};
  int select;

  Othello game;
  game.display_board();

  while(!game.identify_end()){
    std::cout << "Now player is " << player_color[game.current_color] << std::endl;
    std::cout << "Enter Number 0:Pass 1:Put Disc> ";
    std::cin >> select;

    if(select == 1){
      int x,y;
      std::cout << "Enter coordinate (x,y)> ";
      std::cin >> x >> y;
      game.move(x,y);
      game.pass_count = 0;
    } else if(select == 0){
      game.pass_count += 1;
      game.player_pass();
    }
  }


  return 0;
}

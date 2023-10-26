#include <iostream> 
#include <vector> 
#include <algorithm> 
#include <random> 

class correctInput{
private:
  void fixWrongCin(){
    std::cout << "Wrong input, try again> ";
    std::cin.clear();
    std::cin.ignore(100000000,'\n');
  }
public:
  char correctXOChoice(){
    char moveChoice; 
    while (!(std::cin >> moveChoice) || (moveChoice != 'X' && moveChoice != 'O')) fixWrongCin();
    return moveChoice;
  } 

  char correctContinueChoice(){
    char continueChoice;
    while (!(std::cin >> continueChoice) || (continueChoice != 'Y' && continueChoice != 'N')) fixWrongCin();
    return continueChoice;
  }

  int correctRowColChocie(){
    int rowcol; 
    while (!(std::cin >> rowcol) || rowcol > 3 || rowcol < 1 || std::cin.get() != '\n') fixWrongCin();
    return rowcol;
  }
};

class operationsOnVectors{
public:
  void removeNestedVector(std::vector<int>& vec, std::vector<std::vector<int>>& vectorOfVectors) {
    auto element = find(vectorOfVectors.begin(), vectorOfVectors.end(), vec);
    if (element != vectorOfVectors.end()) vectorOfVectors.erase(element);
  }

  bool checkNestedVecExists(const std::vector<int>& vec, const std::vector<std::vector<int>>& vectorOfVectors) {
    return find(vectorOfVectors.begin(), vectorOfVectors.end(), vec) != vectorOfVectors.end();
  }

  std::vector<int> vectorShuffle(std::vector<std::vector<int>> vectorOfVectors){
    std::random_device rd; 
    std::mt19937 generator(rd());
  
    shuffle(vectorOfVectors.begin(), vectorOfVectors.end(), generator);
    return vectorOfVectors[0];
  }
}; 


class screenOutput{
public:
  void printGameMap(std::vector<std::vector<char>> gameMap){
    std::cout << std::endl << "Current state: " << std::endl;

    for (int i = 0; i < 3; ++i){
      for (int j = 0; j < 3; ++j){
        std::cout << gameMap[i][j] << " ";
      }
      std::cout << std::endl;
      }
  }
};

class gameLogic{
private:
  correctInput input;
  operationsOnVectors vectorOperations;
  screenOutput output;

public:
  char whoseMove(char userXO){
    //if the user chose X - user first, if O - bot first
    return (userXO == 'X') ? 'u' : 'b';
  }

  void userTurnLogic(std::vector<std::vector<char>> &gameMap, std::vector<std::vector<int>> &potentialMoves, char userXO){
    bool checkMoveCorrect = true;
    while (checkMoveCorrect){
      std::cout << "Input the number of the row> "; int row = input.correctRowColChocie();
      std::cout << "Input the number of the column> "; int column = input.correctRowColChocie();
      
      std::vector<int> currentUserMove = {row, column};
      checkMoveCorrect = !(vectorOperations.checkNestedVecExists(currentUserMove, potentialMoves));
      if (!checkMoveCorrect){
        gameMap[row-1][column-1] = userXO;
        vectorOperations.removeNestedVector(currentUserMove, potentialMoves);
      }
      else{
        std::cout << std::endl << "There is already a symbol in this place, try again" << std::endl << std::endl;
      }
    }
    output.printGameMap(gameMap);
  }

  void botTurnLogic(std::vector<std::vector<char>> &gameMap, std::vector<std::vector<int>> &potentialMoves, char botXO){
    std::vector<int> currentBotMove = vectorOperations.vectorShuffle(potentialMoves);
    int row = currentBotMove[0];
    int column = currentBotMove[1];
    
    std::cout << "Bot sets '" << botXO << "' at the position (" << row << ", " << column << ")" << std::endl;
    gameMap[row-1][column-1] = botXO;
    
    vectorOperations.removeNestedVector(currentBotMove, potentialMoves);
    
    output.printGameMap(gameMap);
  }
  
  char checkWinner(const std::vector<std::vector<char>>& gameMap) {
    for (int i = 0; i < 3; ++i) {
        if (gameMap[i][0] == gameMap[i][1] && gameMap[i][0] == gameMap[i][2] && gameMap[i][0] != '.') return gameMap[i][0];
        if (gameMap[0][i] == gameMap[1][i] && gameMap[0][i] == gameMap[2][i] && gameMap[0][i] != '.') return gameMap[0][i];
    }
    if (gameMap[0][0] == gameMap[1][1] && gameMap[0][0] == gameMap[2][2] && gameMap[0][0] != '.') return gameMap[0][0];
    if (gameMap[0][2] == gameMap[1][1] && gameMap[0][2] == gameMap[2][0] && gameMap[0][2] != '.') return gameMap[0][2];
    return '.';
  }

  void whoWon(const std::vector<std::vector<char>>& gameMap, char userXO, char botXO){
    char resultOfGame = checkWinner(gameMap);
    if (userXO == resultOfGame){
      std::cout << "Game over. You win!" << std::endl;
    }
    if (botXO == resultOfGame){
      std::cout << "Game over. You lose(" << std::endl; 
    }
    if (resultOfGame == '.'){
      std::cout << "Game over. Draw!" << std::endl;
    }
  }
  
  void mainGameCycled(std::vector<std::vector<char>> &gameMap, std::vector<std::vector<int>> &potentialMoves, char botXO, char userXO){
    char currentMove = this -> whoseMove(userXO);
    output.printGameMap(gameMap);
    bool someOneWin = false; 
    for (int i = 0; (i < 3 && !someOneWin); ++i){
      for (int j = 0; (j < 3 && !someOneWin); ++j){
        if (currentMove == 'u'){
          std::cout << std::endl << "Your turn: " << std::endl;
          userTurnLogic(gameMap, potentialMoves, userXO);
          currentMove = 'b';
        }
        else{
          std::cout << std::endl << "Bot turn: " << std::endl;
          this -> botTurnLogic(gameMap, potentialMoves, botXO);
          currentMove = 'u';
        }

        char resultOfCurrentMove = this -> checkWinner(gameMap);
        if (resultOfCurrentMove != '.'){
          someOneWin = true;
        }
      }
    }
    this -> whoWon(gameMap, userXO, botXO);
  }

};



int main(){
  std::cout << "Tik-tac-toe" << std::endl;

  correctInput input;
  operationsOnVectors vectorOperations;
  screenOutput output;
  gameLogic logic;


  char userChoice = 'Y';
  while (userChoice == 'Y'){
    std::cout << "Choose your sign (X/O)> ";
    char userXO = input.correctXOChoice();
    char botXO = (userXO == 'X') ? 'O' : 'X';

    std::vector<std::vector<char>> gameMap = {{'.','.','.'}, {'.','.','.'}, {'.','.','.'}};
    std::vector<std::vector<int>> potentialMoves = {{1,1}, {1,2}, {1,3}, {2,1}, {2,2}, {2,3}, {3,1}, {3,2}, {3,3}};

    logic.mainGameCycled(gameMap, potentialMoves, botXO, userXO);

    std::cout << "Continue (Y/N)> ";
    userChoice = input.correctContinueChoice();
  }
}

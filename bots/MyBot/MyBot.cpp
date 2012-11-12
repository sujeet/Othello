/*
 * @file botTemplate.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @date 2010-02-04
 * Template for users to create their own bots
 */

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>

#define MIN -1
#define MAX 1
#define OPPOSITE(X) (-X)
#define get_extreme(X) ((X)*1073741824) // 1073741824 is 2^30
// #define print_move(move) cout << (move).x << " " << (move).y << endl;
#define print_move(move) 0;

#define MAX_DEPTH 5

using namespace std;
using namespace Desdemona;

int const weights [8][8] = 
{{120, -20, 20,  5,  5, 20, -20, 120},
 {-20, -40, -5, -5, -5, -5, -40, -20},
 { 20,  -5, 15,  3,  3, 15,  -5,  20},
 {  5,  -5,  3,  3,  3,  3,  -5,   5},
 {  5,  -5,  3,  3,  3,  3,  -5,   5},
 { 20,  -5, 15,  3,  3, 15,  -5,  20},
 {-20, -40, -5, -5, -5, -5, -40, -20},
 {120, -20, 20,  5,  5, 20, -20, 120}};

class MinimaxResult 
{
public :
     Move move;
     int value;
     MinimaxResult (const Move & move, int value) : move (0,0)
          {
               this->move = move;
               this->value = value;
          }
};

class MyBot: public OthelloPlayer
{
public:
     /**
      * Initialisation routines here
      * This could do anything from open up a cache of "best moves" to
      * spawning a background processing thread. 
      */
     MyBot( Turn turn );

     /**
      * Play something 
      */
     virtual Move play( const OthelloBoard& board );
private:
     int evaluate (const OthelloBoard & board) ;
     MinimaxResult mini_max (const OthelloBoard & board,
                             int min_or_max,
                             int depth,
                             Turn turn,
                             int best_so_far);
};

MyBot::MyBot( Turn turn )
     : OthelloPlayer( turn )
{
}

int MyBot::evaluate (const OthelloBoard & board) 
{
     Coin my_color = this->turn;
     Coin opp_color = other (this->turn);
     int i,j;
     int score = 0;
     for (i = 0; i < 8; ++i) {
          for (j = 0; j < 8; ++j) {
               if (board.get (i,j) == my_color) {
                    score += weights [i][j];
               }
               else if (board.get (i,j) == opp_color) {
                    score -= weights [i][j];
               }
          }
     }
     
     Coin coin;
     if (board.get (0,0) != EMPTY) {
          if ((coin = board.get (0,1)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (1,0)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (1,1)) != EMPTY) {
               if (coin == my_color) score += 35;
               else score -= 35;
          }
     }
     if (board.get (7,0) != EMPTY) {
          if ((coin = board.get (6,0)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (7,1)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (6,1)) != EMPTY) {
               if (coin == my_color) score += 35;
               else score -= 35;
          }
     }
     if (board.get (0,7) != EMPTY) {
          if ((coin = board.get (0,6)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (1,7)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (1,6)) != EMPTY) {
               if (coin == my_color) score += 35;
               else score -= 35;
          }
     }
     if (board.get (7,7) != EMPTY) {
          if ((coin = board.get (6,7)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (7,6)) != EMPTY) {
               if (coin == my_color) score += 15;
               else score -= 15;
          }
          if ((coin = board.get (6,6)) != EMPTY) {
               if (coin == my_color) score += 35;
               else score -= 35;
          }
     }
     return score;
     
               
     // int score = (board.getBlackCount ()
     //              - board.getRedCount ());
     // if (this->turn == RED) {
     //      score = -score;
     // }
     // return score;
}

// min_or_max : can be either MIN or MAX
//              MIN -> From the child minmax values choose min
//              MAX -> From the child minmax values choose max
// depth      : When 0, return the evaluation function value for board
//              instead of returning 'best' minimax value for the children.
// turn       : Note that this might be different from this->turn
//              this parameter denotes at this position of the board,
//              who is going to make next turn.
// best_so_far: By now, many of minimaxes at this depth would have finished.
//              best_so_far is the best of these. (minimum in case of MIN
//              and maximum in case of MAX).
MinimaxResult MyBot::mini_max (const OthelloBoard & board,
                               int min_or_max,
                               int depth,
                               Turn turn,
                               int best_so_far) 
{
     // Leaf node due to depth.
     if (depth == 0) {
          Move move (0, 0); // has no consequences.
          return MinimaxResult (move, this->evaluate (board));
     }


     list<Move> moves = board.getValidMoves (turn);
     list<Move>::iterator it = moves.begin();

     // Leaf node due to unavailability of moves.
     if (moves.size () == 0) {
          Move move (0, 0); // has no consequences.
          return MinimaxResult (move, this->evaluate (board));
     }
     
     OthelloBoard test_board (board);
     test_board.makeMove (turn, *it);
     int best_minimax_value_so_far = 
          this->mini_max (test_board,
                          OPPOSITE (min_or_max),
                          depth - 1,
                          other (turn),
                          get_extreme (OPPOSITE (min_or_max))).value;
     Move best_move = *it;
     ++ it;
    
     for (; it != moves.end(); ++it) {
          OthelloBoard test_board (board);
          test_board.makeMove (turn, *it);
          int minimax_value = this->mini_max (
               test_board,
               OPPOSITE (min_or_max),
               depth - 1,
               other (turn),
               best_minimax_value_so_far
          ).value;
 
          switch (min_or_max) {
          case MIN :
               if (minimax_value <= best_so_far) {
                    return MinimaxResult (*it,
                                          minimax_value);
               }
               if (minimax_value < best_minimax_value_so_far) {
                    best_minimax_value_so_far = minimax_value;
                    best_move = *it;
               }
               break;
          case MAX :
               if (minimax_value >= best_so_far) {
                    return MinimaxResult (*it,
                                          minimax_value);
               }
               if (minimax_value > best_minimax_value_so_far) {
                    best_minimax_value_so_far = minimax_value;
                    best_move = *it;
               }
               break;
          }
     }
     return MinimaxResult (best_move, best_minimax_value_so_far);
}

Move MyBot::play( const OthelloBoard& board )
{
     MinimaxResult result = this->mini_max (board,
                                            MAX,
                                            MAX_DEPTH,
                                            this->turn,
                                            get_extreme (MAX));
     return result.move;
}


// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
     OthelloPlayer* createBot( Turn turn )
     {
          return new MyBot( turn );
     }

     void destroyBot( OthelloPlayer* bot )
     {
          delete bot;
     }
}



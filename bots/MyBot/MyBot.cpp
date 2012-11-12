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
#define OPPOSITE(X) -X
// #define print_move(move) cout << (move).x << " " << (move).y << endl;
#define print_move(move) 0;                     \

#define MAX_DEPTH 5

using namespace std;
using namespace Desdemona;

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
                             Turn turn);
};

MyBot::MyBot( Turn turn )
     : OthelloPlayer( turn )
{
}

int MyBot::evaluate (const OthelloBoard & board) 
{
     int score = (board.getBlackCount ()
                  - board.getRedCount ());
     if (this->turn == RED) {
          score = -score;
     }
     return score;
}

// min_or_max : can be either MIN or MAX
//              MIN -> From the child minmax values choose min
//              MAX -> From the child minmax values choose max
// depth      : When 0, return the evaluation function value for board
//              instead of returning 'best' minimax value for the children.
// turn       : Note that this might be different from this->turn
//              this parameter denotes at this position of the board,
//              who is going to make next turn.
MinimaxResult MyBot::mini_max (const OthelloBoard & board,
                               int min_or_max,
                               int depth,
                               Turn turn) 
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
                          other (turn)).value;
     Move best_move = *it;
     ++ it;
    
     for (; it != moves.end(); ++it) {
          OthelloBoard test_board (board);
          test_board.makeMove (turn, *it);
          int minimax_value = this->mini_max (test_board,
                                              OPPOSITE (min_or_max),
                                              depth - 1,
                                              other (turn)).value;
 
          switch (min_or_max) {
          case MIN :
               if (minimax_value < best_minimax_value_so_far) {
                    best_minimax_value_so_far = minimax_value;
                    best_move = *it;
               }
               break;
          case MAX :
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
                                            this->turn);
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



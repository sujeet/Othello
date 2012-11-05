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
using namespace std;
using namespace Desdemona;

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
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

Move MyBot::play( const OthelloBoard& board )
{
    list<Move> moves = board.getValidMoves( turn );
    list<Move>::iterator it = moves.begin();
    if (this->turn == BLACK) cout << "black\n";
    if (this->turn == RED) cout << "red\n";
    
    Move final_move = *it;
    int max_coin_count = 0;
    for (; it != moves.end(); ++it) {
         // Simulate a move, and choose which maximizes your coin count.
         OthelloBoard test_board (board);
         test_board.makeMove (this->turn, *it);

         int count = 0;
         if (this->turn == BLACK) count = test_board.getBlackCount ();
         if (this->turn == RED) count = test_board.getRedCount ();

         if (count > max_coin_count) {
              max_coin_count = count;
              final_move = *it;
         }
    }
    

    return final_move;
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



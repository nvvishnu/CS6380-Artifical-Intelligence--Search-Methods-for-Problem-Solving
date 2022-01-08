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

// Using Chrono library to measure the execution time - Should be commented before submission
#include <chrono>
#include <climits>

using namespace std;
using namespace Desdemona;

#define pii pair<int,int>
#define pb push_back
#define BOARD_SIZE 8

Move best_move(Move::empty()); 
//Used to store the best move for the player

int Value[BOARD_SIZE][BOARD_SIZE] = 
{
    {100, -20, 10, 5, 5, 10, -20, 100}, 
    {-20, -50, -2, -2, -2, -2, -50, -20},
    {10, -2, -1, -1, -1, -1, -2, 10},
    {5, -2, -1, -1, -1, -1, -2, 5},
    {5, -2, -1, -1, -1, -1, -2, 5},
    {10, -2, -1, -1, -1, -1, -2, 10},
    {-20, -50, -2, -2, -2, -2, -50, -20},
    {100, -20, 10, 5, 5, 10, -20, 100},
};


/*{ 
                                        { 20, -3, 11, 8, 8, 11, -3, 20 },
    				                    { -3, -7, -4, 1, 1, -4, -7, -3 },
         				                { 11, -4, 2, 2, 2, 2, -4, 11 },
    				                    { 8, 1, 2, -3, -3, 2, 1, 8 },
    				                    { 8, 1, 2, -3, -3, 2, 1, 8 },
    				                    { 11, -4, 2, 2, 2, 2, -4, 11 },
    				                    { -3, -7, -4, 1, 1, -4, -7, -3 },
    				                    { 20, -3, 11, 8, 8, 11, -3, 20 } 
                                    };*/

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

        Turn my_turn; //Marks the turn of the player
        int look_ahead_depth = 6; // The maximum look-ahead depth.
        // Game_Tree_Node root; //Root of the game tree
        double alpha; //Weight assigned to corner_closeness in the evaluation function 
        double beta; //Weight assigned to legal_moves in the evaluation function
        double gamma; //Weight assigned to corner_pieces in the evaluation function 
        double mu; //Weight assigned to positional_value in the evaluation function    
        const int MAX = INT_MAX; //Initial value of Alpha
        const int MIN = INT_MIN; //Initial value of Beta


        int disc_count(const OthelloBoard& board,Turn turn); 
        int legal_moves(const OthelloBoard& board,Turn turn);
        double evaluation(const OthelloBoard& board,Turn turn); 
        int corner_pieces(const OthelloBoard& board,Turn turn);
        int cornercloseness(const OthelloBoard&Board, Turn turn);
        int positionalvalue(const OthelloBoard& board, Turn turn);
        double alpha_beta(int depth, OthelloBoard board,
			Turn turn, int alpha,
			int beta);
        
        
        
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
    //cout<<"Constructor called"<<endl;
    fflush(stdout);
    my_turn=turn;    
    beta  = 100;
    gamma = 1000;
    alpha = 250;
    mu=10;
}

// Returns the optimal value for the player
double MyBot::alpha_beta(int depth, OthelloBoard board,
			Turn turn, int alpha,
			int beta)
{
    list<Move> moves = board.getValidMoves(turn);
    //cout<<"Depth: "<<depth<<" Number of moves: "<<moves.size()<<endl;
    // Terminating condition. i.e. leaf node is reached or there are no valid moves
	if (depth == look_ahead_depth || moves.size()==0)
		return evaluation(board,turn);

	if (turn==BLACK)
	{	
		for (auto move:moves)
		{			
            OthelloBoard child = board;
            child.makeMove(turn,move);
			int val = alpha_beta(depth + 1, child,
							other(turn), alpha, beta);
            if(depth == 0 && val>alpha)
            {
                best_move = move;                
            }
			alpha = max(alpha, val);

			// Alpha Beta Pruning
			if (beta <= alpha)
				break;
		}
		return alpha;
	}
	else
	{
        for (auto move:moves)
		{			
            OthelloBoard child = board;
            child.makeMove(turn,move);
			int val = alpha_beta(depth + 1, child,
							other(turn), alpha, beta);			
			beta = min(beta, val);

			// Alpha Beta Pruning
			if (beta <= alpha)
				break;
		}
		return beta;
	}
}


Move MyBot::play( const OthelloBoard& board )
{
    //cout<<"Play called"<<endl;
    auto start = std::chrono::high_resolution_clock::now();

    //Generation of the entire tree is against the spirit of alpha-beta
    list<Move> moves = board.getValidMoves(my_turn);
    list<Move>::iterator it = moves.begin();
    alpha_beta(0,board,my_turn,MIN,MAX);
    if(!board.validateMove(turn,best_move))
    {
        //cout<<best_move.x<<","<<best_move.y<<" is not a valid move"<<endl;
        best_move=*it;
    }    

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  
   // cout << "Time taken to make a move is " << duration.count() << " microseconds" << endl;
    //cout<<" Number of valid moves is: "<<num_valid<<endl;
    
    return best_move;
}

//Returns the disc count of the player in the board
int MyBot::disc_count(const OthelloBoard& board,Turn turn)
{    
    switch(turn)
    {
        case BLACK: return board.getBlackCount(); break;
        case RED:   return board.getRedCount(); break;
        default: return -1;    
    }
    return -1;
}

// Returns the number of legal moves that can be made 
int MyBot::legal_moves(const OthelloBoard& board,Turn turn)
{
    return board.getValidMoves(turn).size();
}

// Returns the number of legal moves that can be made 
int MyBot::corner_pieces(const OthelloBoard& board,Turn turn)
{
    int ans=0;
    if(board.get(0,0)==turn)
    ans++;
    if(board.get(0,BOARD_SIZE-1)==turn)
    ans++;
    if(board.get(BOARD_SIZE-1,0)==turn)
    ans++;
    if(board.get(BOARD_SIZE-1,BOARD_SIZE-1)==turn)
    ans++;
    return ans;
}

//Returns the aggregate score of the board
double MyBot::evaluation(const OthelloBoard& board,Turn turn)
{
    double player_value = disc_count(board,turn) + beta*legal_moves(board,turn) + gamma*corner_pieces(board,turn) - alpha*cornercloseness(board,turn) + mu*positionalvalue(board,turn);
    double opponent_value = disc_count(board,other(turn)) + beta*legal_moves(board,other(turn)) + gamma*corner_pieces(board,other(turn)) - alpha*cornercloseness(board,turn) + mu*positionalvalue(board,turn) ;
    return (player_value - opponent_value);
}

//Returns the number of pieces of the player close to the corner.
// Having pieces which enable the opponent to place a piece in the corner is disadvantageous 
// as corner pieces are very powerful.

int MyBot::cornercloseness(const OthelloBoard& board, Turn turn)
{
    int ans = 0;
    if(board.get(0,0)==EMPTY) 
    {
        if(board.get(0,1)==turn)
        ans++;
        if(board.get(1,0)==turn)
        ans++;
        if(board.get(1,1)==turn)
        ans++;
    }
    if(board.get(0,BOARD_SIZE-1)==EMPTY) 
    {
        if(board.get(0,BOARD_SIZE-2)==turn)
        ans++;
        if(board.get(1,BOARD_SIZE-1)==turn)
        ans++;
        if(board.get(1,BOARD_SIZE-2)==turn)
        ans++;
    }
    if(board.get(BOARD_SIZE-1,0)==EMPTY) 
    {
        if(board.get(BOARD_SIZE-1,1)==turn)
        ans++;
        if(board.get(BOARD_SIZE-2,0)==turn)
        ans++;
        if(board.get(BOARD_SIZE-2,1)==turn)
        ans++;
    }
    if(board.get(BOARD_SIZE-1,BOARD_SIZE-1)==EMPTY) 
    {
        if(board.get(BOARD_SIZE-1,BOARD_SIZE-2)==turn)
        ans++;
        if(board.get(BOARD_SIZE-2,BOARD_SIZE-1)==turn)
        ans++;
        if(board.get(BOARD_SIZE-2,BOARD_SIZE-2)==turn)
        ans++;
    }
   return ans;
}

int MyBot::positionalvalue(const OthelloBoard& board, Turn turn)
{
    int ans=0;
    for(int i=0;i<BOARD_SIZE;i++)
    {
        for (int j=0;j<BOARD_SIZE;j++)
        {
            if(board.get(i,j)==turn)
            ans+=Value[i][j];
        }        
    }
    return ans;
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



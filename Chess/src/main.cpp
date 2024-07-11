// Chess 
#include "../include/Chess.h"
#include "../include/Board.h"

int main()
{
    //string board = "RNBQKBNR"
                   "PPPPPPPP"
                   "########"
                   "########"
                   "########"
                   "########"
                   "pppppppp"
                   "rnbqkbnr";
    string board = "RNBQ#K#R"
                   "PPPP###q"
                   "####PP##"
                   "######P#"
                   "########"
                   "####p###"
                   "pppp#ppp"
                   "rnb#kbnr";
    //string board = "R###K##RPPPPPPPP################################ppppppppr###k##r";
    //string board = "RNBQKBNR################################################rnbqkbnr";
	Chess a(board);
    shared_ptr<Board> engine_board(make_shared<Board>(board));
    int codeResponse = 0;
	string res = a.getInput();
	while (res != "exit")
	{
		/* 
		codeResponse value : 
		Illegal movements : 
		11 - there is not piece at the source  
		12 - the piece in the source is piece of your opponent
		13 - there one of your pieces at the destination 
		21 - illegal movement of that piece 
		31 - this movement will cause you checkmate

		legal movements : 
		41 - the last movement was legal and cause check 
		42 - the last movement was legal, next turn
        43 - the last move was Castling
        44 - the last move was checkmate
		*/

		/**/ 
		{ // put your code here instead that code
            codeResponse=engine_board->move(res);
		}
		/**/

		a.setCodeResponse(codeResponse);
		res = a.getInput();
	}

	cout << endl << "Exiting " << endl; 
	return 0;
}
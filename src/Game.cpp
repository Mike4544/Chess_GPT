#include "Game.h"

#include "Helpers/HelperMacros.h"

SDL_Renderer* Game::renderer = nullptr;
bool Game::currentTurn = true;

//  The UI
UI* ui = nullptr;

//  The game board
Board* board = nullptr;

//  The player
Player* player = nullptr;
Player* enemy = nullptr;

Game::Game() {

    //  Init the game
    Init("Chess GPT",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        8 * Chess::Tiles::TILE_HEIGHT,
        8 * Chess::Tiles::TILE_WIDTH + 250, /* We will have a player view on the side, basically */
        SDL_WINDOW_SHOWN
    );
}

Game::~Game() {

    //  Clean up
    CleanUp();

}

void Game::Init(const char* windowTitle, int x, int y, int height, int width, int flags) {
    //  Ensure everything is initialized
    __SDL_ASSERT_FAILURE(SDL_Init(SDL_INIT_EVERYTHING) == 0, "SDL_Init failed: %s\n", VOID_RETURN);

    //  Init the texture subsystem
    __SDL_ASSERT_FAILURE(TextureManager::Init() == true, "Texture init failed: %s\n", VOID_RETURN);

    //  Init the sound subsystem
    __SDL_ASSERT_FAILURE(SoundManager::Init() == true, "Sound init failed: %s\n", VOID_RETURN);

    //  Init the font subsystem
    __SDL_ASSERT_FAILURE(FontManager::Init() == true, "Font init failed: %s\n", VOID_RETURN);


    //  Create the window
    _window = SDL_CreateWindow(windowTitle, x, y, width, height, flags);
    __SDL_ASSERT_FAILURE(_window != NULL, "Window failed to initialize: %s\n", VOID_RETURN);

    //  Create the renderer
    renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    __SDL_ASSERT_FAILURE(renderer != NULL, "Renderer failed to initialize: %s\n", VOID_RETURN);

    //  Init the UI
    ui = new UI("Test");


    //  Init the player
    player = new Player(Chess::Pieces::WHITE);
    enemy = new Player(Chess::Pieces::BLACK);

    //  Init the board
    board = Board::getInstance();
    board->InitBoard(player, enemy);



    _isRunning = true; 
}

void Game::HandleEvents() {

    SDL_Event event;

    //  Wait for the event, as there is no need to poll
    SDL_WaitEvent(&event);

    switch(event.type) {
        //  Ensure the game loop stops
        case SDL_QUIT: {
            _isRunning = false;
            break;
        }
        
        case SDL_MOUSEBUTTONDOWN: {

            //  Handle mouse button down
            Vector2D mousePos = {event.button.x / Chess::Tiles::TILE_WIDTH, event.button.y / Chess::Tiles::TILE_HEIGHT};
            //  Log the mouse position @DEBUG
            SDL_Log("Mouse button down at (%d, %d), tile (%d, %d)\n", event.button.x, event.button.y, mousePos.x, mousePos.y);
            
            //  Check if it's the player's turn or not
            Player*& curr = currentTurn ? player : enemy;
            // Get the other player
            Player*& other = currentTurn ? enemy : player;

            //  Take the piece
            MoveHandler::TakePiece(curr, other, mousePos);
            //  Useless since the player's piece will already be 0 if otherwise
            //  _mouseHasPiece = player->tempPiece.piece != 0;

            break;
        }

        case SDL_MOUSEBUTTONUP: {
            //  Handle mouse button up
            //  Log the mouse position @DEBUG
            SDL_Log("Mouse button up at (%d, %d)", event.button.x, event.button.y);

            //  Check the current player
            Player*& curr = currentTurn ? player : enemy;

            //  Break out if the mouse doesn't have a piece
            if(!curr->Piece().piece) {
                SDL_Log("Player didn't have any piece\n");
                break;
            }

            Vector2D mousePos = {
                event.button.x / Chess::Tiles::TILE_WIDTH,
                event.button.y / Chess::Tiles::TILE_HEIGHT
            };
            
            Player*& other = currentTurn ? enemy : player;


            MoveHandler::HandleMove(curr, other, mousePos);

            break;
        }

        //  @DEBUG: set the turn to white on A pressed
        #ifdef DEBUG
        case SDL_KEYDOWN: {
            const Uint8* state = SDL_GetKeyboardState(nullptr);

            if(state[SDL_SCANCODE_A]) {
                SDL_Log("A pressed. Turn changed to WHITE\n");
                currentTurn = true;
            }


            break;
        }


        #endif

        default:
            break;
    }
}

void Game::Update() {
    //  Calculate the currently attacked tiles for THIS turn
    if(_lastTurn != currentTurn) {
        // SDL_Log("Before calculations: \n");
        // MoveHandler::PrintBoard();

        //  Get the other player's attacked tiles
        auto& other = currentTurn ? enemy : player;

        
        CheckingService::CalculateAttackedTiles(other);
    
        // SDL_Log("After:\n");
        // MoveHandler::PrintBoard();



        CheckingService::CheckingCheck();

        //  Check if the player is in check
        if((CheckingService::isBlackKingChecked && !currentTurn) || (CheckingService::isWhiteKingChecked && currentTurn)) {
            SoundManager::PlaySound(Chess::Sounds::CHECK);
        }

        //  Check if the player is in checkmate
        if(CheckingService::isWhiteKingChecked) {
            auto isCheckmate = CheckingService::CheckMateCheck(player, other);

            SDL_Log("White is MATE: %d\n", isCheckmate);            
        
            if(isCheckmate) {
                SDL_Log("White is in checkmate\n");
                SDL_Delay(3000);

                _isRunning = false;
            }
        
        }
        else if(CheckingService::isBlackKingChecked) {
            auto isCheckmate = CheckingService::CheckMateCheck(enemy, other);

            SDL_Log("Black is MATE: %d\n", isCheckmate);
        
            if(isCheckmate) {
                SDL_Log("Black is in checkmate\n");
                SDL_Delay(3000);

                _isRunning = false;
            }
        
        }


    }


    //  Set the last turn to this turn
    _lastTurn = currentTurn;
}

void Game::Render() {
    //  Clear the screen
     SDL_RenderClear(renderer);

    //  Render stuff on the screen

    //  Draw the board
    board->DrawBoard();

    //  Check if the _tempPiece is not null

    //  Get the current player
    Player*& curr = currentTurn ? player : enemy;

    if(curr->Piece().piece) {
       Board::MovePieceToMouse(curr->Piece().piece);

       board->DrawPossibleMoves(curr->LegalMoves());
    }

    #ifdef DEBUG

        board->DrawAttackedTiles();

    #endif

    //  Draw the UI
    ui->RenderPlayerWindow();
    ui->RenderEatenPieces(player, enemy);

    //  Show the stuff on the screen
     SDL_RenderPresent(renderer);
}


void Game::CleanUp() {
    //  Free the player
    delete player;

    //  Free the board
    delete board;

    //  Free the UI
    delete ui;


    //  Free the subsystems
    TextureManager::CleanUp();
    


    //  Destroy the window
    SDL_DestroyWindow(_window);

    //  Destroy the renderer
    SDL_DestroyRenderer(renderer);

    //  SDL_Quit
    SDL_Quit();


}

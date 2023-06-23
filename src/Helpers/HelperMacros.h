#define VOID_RETURN

#define ERROR_TEXTURE(width, height)                        \
(SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height))

#define __SDL_ASSERT_FAILURE(condition, message, returnVal) \
if(!(condition)) {                                          \
    SDL_Log(message, SDL_GetError());                       \
    return returnVal;                                       \
}

#define __CHESS_NOT_COLOR(piece, color)                     \
((piece & color) == 0)

#define UID_MASK            (0b11111111)
#define UID_MASK_SHIFTED    (UID_MASK << 24)
#define PIECE_MASK  (0b11111111 | UID_MASK_SHIFTED)
//  I am masking all the bits defining pieces
#define __CHESS_EMPTY_TILE(tile)                            \
((tile & PIECE_MASK) == 0)

//  Same as before, but I am getting the color as well
#define __CHESS_GET_PIECE_TILE(tile)                        \
((tile & PIECE_MASK))



#define __CHESS_ATTACK_TILE(tile, player_color)             \
(tile |= (player_color == Chess::Pieces::WHITE ? Chess::Tiles::WHITE_ATTACKED : Chess::Tiles::BLACK_ATTACKED))           

#define __CHESS_CLEAR_ATTACK(tile, player_color)            \
(tile &= ~(player_color == Chess::Pieces::WHITE ? Chess::Tiles::WHITE_ATTACKED : Chess::Tiles::BLACK_ATTACKED))



#define __CHESS_TILE_ATTACKED(tile, player_color)           \
((tile & player_color))

#define __CHESS_TILE_NOT_ATTACKED(tile, player_color)       \
((tile & (player_color == Chess::Pieces::WHITE ? Chess::Tiles::BLACK_ATTACKED : Chess::Tiles::WHITE_ATTACKED)) == 0)

 

#include "game_logic.h"

void fsm(GameState_t* state, UserAction_t* action) {
  GameInfo_t* game = game_info();
  switch (*state) {
    case START:
      initialize_game(game);
      initialize_game_start(game);
      select_next_figure(game);
      if (*action == Start) {
        *state = SPAWN;
      }
      break;
    case SPAWN:
      add_next_figure(game);
      select_next_figure(game);
      *state = MOVING;
      break;
    case MOVING:
      if (*action == Action) {
        *state = SHIFTING;
      } else {
        userInput(*action, *action == Down);
        if (!can_move_down(game)) *state = ATTACHING;
      }
      break;
    case SHIFTING:
      rotate_figure(game);
      *state = MOVING;
      break;
    case ATTACHING:
      fix_figure(game);
      if (check_gameover(game)) {
        *state = GAMEOVER;
      } else {
        *state = SPAWN;
      }
      break;
    case GAMEOVER:
      *state = START;
      break;
  }
}

/**
 * @mainpage Tetris Game Documentation
 *
 * Welcome to the documentation of the Tetris game, part of the C7 Brick Game
 * Collection.
 *
 * @section sec_controls Controls
 * - **Left Arrow**: Move block left
 * - **Right Arrow**: Move block right
 * - **Up Arrow**: Rotate block clockwise
 * - **Down Arrow**: Speed up block descent
 * - **Space**: Drop block instantly
 * - **P**: Pause/unpause the game
 */

#include "../../gui/cli/cli.h"
#include "game_logic.h"

int main() {
  init_gui();
  GameInfo_t *gameInfo = game_info();
  GameState_t state = START;
  showWelcomeScreen();

  while (true) {
    int ch = getch();
    UserAction_t action = getUserAction(ch);
    flushinp();
    if (action == Terminate) {
      break;
    }
    if (action == Pause) {
      pauseGame();  // Вызываем функцию паузы
      continue;  // Возвращаемся в начало цикла после паузы
    }

    fsm(&state, &action);
    updateInfoPanel(gameInfo);
    draw_field(gameInfo);

    if (state == GAMEOVER) {
      showGameOverScreen();
    }
    unsigned int delay =
        get_speed_delay(gameInfo);  // получаем задержку в наносекундах

    // Создаем структуру timespec для задержки
    struct timespec req = {0, delay};
    nanosleep(&req, NULL);  // Используем nanosleep вместо usleep
  }
  end_gui();
  return 0;
}
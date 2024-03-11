#include "ui.h"
#include <ncurses.h>
#include <stdlib.h>

static void on_exit(void);
static bool init_colors(void);
static bool init_windows(void);
static void init_info_text(void);

constexpr int BORDER_WIDTH = 1;
constexpr int BORDER_HEIGHT = 1;
constexpr int ACTION_WIN_WIDTH = 10 * 2;
constexpr int ACTION_WIN_HEIGHT = 18;
constexpr int PREVIEW_WIN_WIDTH = 5 * 2 + 2 * BORDER_WIDTH;
constexpr int PREVIEW_WIN_HEIGHT = 4 + 2 * BORDER_HEIGHT;
constexpr int INFO_WIN_WIDTH = 5 * 2 + 2 * BORDER_WIDTH;
constexpr int INFO_WIN_HEIGHT = 8 + 2 * BORDER_HEIGHT;
constexpr int SIDEBAR_WIDTH = 7 * 2;
constexpr int GAME_WIDTH = ACTION_WIN_WIDTH + SIDEBAR_WIDTH + 3 * BORDER_WIDTH;
constexpr int GAME_HEIGHT = ACTION_WIN_HEIGHT + 2 * BORDER_HEIGHT;

typedef enum color {
  COLOR_GAME_WIN = 1,
  COLOR_ACTION_WIN,
  COLOR_INFO_WIN,
  COLOR_PREVIEW_WIN,
  COLOR_SEPARATOR,
  COLOR_EMPTY,
  COLOR_Z,
  COLOR_S,
  COLOR_T,
  COLOR_L,
  COLOR_J,
  COLOR_I,
  COLOR_O
} color_e;

static const color_e piece_color_map[NUM_PIECES] = {
  [Z] = COLOR_Z,
  [S] = COLOR_S,
  [T] = COLOR_T,
  [L] = COLOR_L,
  [J] = COLOR_J,
  [I] = COLOR_I,
  [O] = COLOR_O,
};

static WINDOW *game_win = NULL, *action_win = NULL,
              *info_win = NULL, *preview_win = NULL;

bool init_ui(void)
{
  initscr();
  atexit(on_exit);

  if(LINES < GAME_HEIGHT) {
    mvprintw(
      LINES / 2, 0,
      "Window min height: %d, current height: %d\n", GAME_HEIGHT, LINES
    );
    return false;
  }
  if(COLS < GAME_WIDTH) {
    mvprintw(
      LINES / 2, 0,
      "Window min width: %d, current width: %d\n", GAME_WIDTH, COLS
    );
    return false;
  }

  if(!init_colors())
    return false;
  if(!init_windows())
    return false;
  init_info_text();

  refresh();
  wrefresh(game_win);
  wrefresh(action_win);
  wrefresh(info_win);
  wrefresh(preview_win);

  return true;
}

void destroy_ui(void)
{
  delwin(preview_win);
  delwin(info_win);
  delwin(action_win);
  delwin(game_win);
}

static void on_exit(void)
{
  endwin();
}

static bool init_colors(void)
{
  if(!has_colors()) {
    mvaddstr(LINES / 2, 0, "No color support");
    return false;
  }
  if(start_color() != OK) {
    mvaddstr(LINES / 2, 0, "Unable to start colors");
    return false;
  }

  init_pair(COLOR_GAME_WIN, COLOR_BLACK, COLOR_YELLOW);
  init_pair(COLOR_ACTION_WIN, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_INFO_WIN, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_PREVIEW_WIN, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_SEPARATOR, COLOR_BLACK, COLOR_YELLOW);
  init_pair(COLOR_EMPTY, COLOR_BLACK, COLOR_BLACK);

  init_pair(COLOR_Z, COLOR_WHITE, COLOR_RED);
  init_pair(COLOR_S, COLOR_WHITE, COLOR_GREEN);
  init_pair(COLOR_T, COLOR_WHITE, COLOR_WHITE);
  init_pair(COLOR_L, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(COLOR_J, COLOR_WHITE, COLOR_CYAN);
  init_pair(COLOR_I, COLOR_WHITE, COLOR_YELLOW);
  init_pair(COLOR_O, COLOR_WHITE, COLOR_BLUE);

  return true;
}

static bool init_windows(void)
{
  const int game_win_offset_y = (LINES - GAME_HEIGHT) / 2;
  const int game_win_offset_x = (COLS - GAME_WIDTH) / 2;

  game_win = newwin(
    GAME_HEIGHT, GAME_WIDTH,
    game_win_offset_y,
    game_win_offset_x
  );

  if(!game_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    return false;
  }

  action_win = newwin(
    ACTION_WIN_HEIGHT, ACTION_WIN_WIDTH,
    game_win_offset_y + BORDER_HEIGHT,
    game_win_offset_x + BORDER_WIDTH
  );

  if(!action_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    delwin(game_win);
    return false;
  }

  info_win = newwin(
    INFO_WIN_HEIGHT, PREVIEW_WIN_WIDTH,
    game_win_offset_y + BORDER_HEIGHT,
    game_win_offset_x + ACTION_WIN_WIDTH + 2 * BORDER_WIDTH +
      (SIDEBAR_WIDTH - INFO_WIN_WIDTH) / 2
  );

  if(!info_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    delwin(action_win);
    delwin(game_win);
    return false;
  }

  preview_win = newwin(
    PREVIEW_WIN_HEIGHT, PREVIEW_WIN_WIDTH,
    game_win_offset_y + GAME_HEIGHT - (BORDER_HEIGHT + PREVIEW_WIN_HEIGHT),
    game_win_offset_x + ACTION_WIN_WIDTH + 2 * BORDER_WIDTH +
      (SIDEBAR_WIDTH - PREVIEW_WIN_WIDTH) / 2
  );

  if(!preview_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    delwin(info_win);
    delwin(action_win);
    delwin(game_win);
    return false;
  }

  wbkgd(game_win, COLOR_PAIR(COLOR_GAME_WIN));
  box(game_win, 0, 0);
  wattrset(game_win, COLOR_PAIR(COLOR_SEPARATOR));
  mvwvline(
    game_win,
    BORDER_HEIGHT, BORDER_WIDTH + ACTION_WIN_WIDTH,
    0, GAME_HEIGHT - 2 * BORDER_HEIGHT
  );

  wbkgd(action_win, COLOR_PAIR(COLOR_ACTION_WIN));

  wbkgd(info_win, COLOR_PAIR(COLOR_INFO_WIN));
  box(info_win, 0, 0);

  wbkgd(preview_win, COLOR_PAIR(COLOR_PREVIEW_WIN));
  box(preview_win, 0, 0);

  return true;
}

static void init_info_text(void)
{
  wattrset(info_win, COLOR_PAIR(COLOR_INFO_WIN) | A_BOLD);
  const int center_x = INFO_WIN_WIDTH / 2 - 1;
  mvwaddstr(info_win, 1, 3, "Score");
  mvwprintw(info_win, 2, center_x, "%d", 0);
  mvwaddstr(info_win, 3, 3, "Level");
  mvwprintw(info_win, 4, center_x, "%d", 0);
  mvwaddstr(info_win, 5, 3, "Lines");
  mvwprintw(info_win, 6, center_x, "%d", 0);
  mvwaddstr(info_win, 7, 3, "Tetris");
  mvwprintw(info_win, 8, center_x, "%d", 0);
}

void show_start_text(void)
{
  wattrset(action_win, COLOR_PAIR(COLOR_ACTION_WIN) | A_BOLD);
  const int center_x = ACTION_WIN_WIDTH / 2;
  mvwaddstr(action_win, 4, center_x - 4, "TERMINAL");
  mvwaddstr(action_win, 5, center_x - 3, "TETRIS");
  mvwaddstr(action_win, 7, center_x - 1, "--");
  mvwaddstr(action_win, 9, center_x - 3, "Press:");
  mvwaddstr(action_win, 11, center_x - 6, "'s' to start");
  mvwaddstr(action_win, 12, center_x - 6, "'q' to quit");
  wrefresh(action_win);
}

void show_game_over_text(void)
{
  wclear(action_win);
  wattrset(action_win, COLOR_PAIR(COLOR_ACTION_WIN) | A_BOLD);
  const int center_x = ACTION_WIN_WIDTH / 2;
  mvwaddstr(action_win, 2, center_x - 5, "GAME OVER");
  mvwaddstr(action_win, 4, center_x - 1, "--");
  mvwaddstr(action_win, 6, center_x - 6, "Your score:");
  mvwaddstr(action_win, 8, center_x - 2, "1234");
  mvwaddstr(action_win, 10, center_x - 1, "--");
  mvwaddstr(action_win, 12, center_x - 3, "Press:");
  mvwaddstr(action_win, 14, center_x - 9, "'s' to play again");
  mvwaddstr(action_win, 15, center_x - 6, "'q' to quit");
  wrefresh(action_win);
}

void draw_action(
  const piece_s piece[const static 1],
  uint8_t *const lines[static FIELD_HEIGHT]
)
{
  wclear(action_win);

  /* draw placed pieces */
  for(uint8_t y = 0; y < FIELD_HEIGHT; y++) {
    for(uint8_t x = 0; x < FIELD_WIDTH; x++) {
      wattrset(action_win, COLOR_PAIR(piece_color_map[lines[y][x]]));
      mvwaddstr(action_win, y, x * 2, "  ");
    }
  }

  /* draw current piece */
  wattrset(action_win, COLOR_PAIR(piece_color_map[piece->type]));

  const int8_t y = piece->pos.y;
  const int8_t x = piece->pos.x * 2;

  // center piece coord always the same
  mvwaddstr(action_win, y + 1, x + 2, "  ");

  for(int i = 0; i < NUM_PIECE_TILES - 1; i++)
    mvwaddstr(
      action_win,
      y + piece->coords[i].y,
      x + piece->coords[i].x * 2,
      "  "
    );

  wrefresh(action_win);
}

void draw_preview(const piece_s piece[const static 1])
{
  wattrset(preview_win, COLOR_PAIR(COLOR_PREVIEW_WIN));
  mvwprintw(preview_win, 2, 1, "%9c", ' ');
  mvwprintw(preview_win, 3, 1, "%9c", ' ');

  const color_e color = piece->type == T ?
    COLOR_EMPTY : piece_color_map[piece->type];

  wattrset(preview_win, COLOR_PAIR(color));

  const int8_t y = 1;
  const int8_t x = piece->type == I || piece->type == O ? 2 : 3;

  // center piece coord always the same
  mvwaddstr(preview_win, y + 1, x + 2, "  ");

  for(int i = 0; i < NUM_PIECE_TILES - 1; i++)
    mvwaddstr(
      preview_win,
      y + piece->coords[i].y,
      x + piece->coords[i].x * 2,
      "  "
    );

  wrefresh(preview_win);
}

void animate_line_removal(
  uint8_t *const lines[static FIELD_HEIGHT],
  const int8_t lines_completed[static MAX_REMOVABLE_LINES]
)
{
  constexpr uint8_t FLASH_MS = 100;
  constexpr uint8_t NUM_FLASHES = 3;

  for(uint8_t flash = 0; flash < NUM_FLASHES; flash++) {
    for(uint8_t i = 0; i < MAX_REMOVABLE_LINES; i++) {
      const int8_t line_to_animate = lines_completed[i];
      if(line_to_animate == 0)
        continue;

      wattrset(action_win, COLOR_PAIR(COLOR_ACTION_WIN));
      mvwprintw(action_win, line_to_animate, 0, "%20c", ' ');
    }

    wrefresh(action_win);
    napms(FLASH_MS);

    for(uint8_t i = 0; i < MAX_REMOVABLE_LINES; i++) {
      const int8_t line_to_animate = lines_completed[i];
      if(line_to_animate == 0)
        continue;

      for(uint8_t col = 0; col < FIELD_WIDTH; col++) {
        const piece_type_e type = lines[line_to_animate][col];
        wattrset(action_win, COLOR_PAIR(piece_color_map[type]));
        mvwaddstr(action_win, line_to_animate, col * 2, "  ");
      }
    }

    wrefresh(action_win);
    napms(FLASH_MS);
  }
}

#include <ncurses.h>
#include <stdlib.h>

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

enum {
  COLOR_GAME_WIN = 1,
  COLOR_ACTION_WIN,
  COLOR_INFO_WIN,
  COLOR_PREVIEW_WIN,
  COLOR_SEPARATOR
};

void on_exit(void)
{
  refresh();
  getch();
  endwin();
}

void init_colors(void)
{
  if(!has_colors()) {
    mvaddstr(LINES / 2, 0, "No color support");
    exit(EXIT_FAILURE);
  }
  if(start_color() != OK) {
    mvaddstr(LINES / 2, 0, "Unable to start colors");
    exit(EXIT_FAILURE);
  }

  init_pair(COLOR_GAME_WIN, COLOR_BLACK, COLOR_YELLOW);
  init_pair(COLOR_ACTION_WIN, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_INFO_WIN, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_PREVIEW_WIN, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_SEPARATOR, COLOR_BLACK, COLOR_YELLOW);
}

void init_windows(
  WINDOW **game_win, WINDOW **action_win,
  WINDOW **info_win, WINDOW **preview_win
)
{
  if(LINES < GAME_HEIGHT) {
    mvprintw(
      LINES / 2, 0,
      "Window min height: %d, current height: %d\n", GAME_HEIGHT, LINES
    );
    exit(EXIT_FAILURE);
  }
  if(COLS < GAME_WIDTH) {
    mvprintw(
      LINES / 2, 0,
      "Window min width: %d, current width: %d\n", GAME_WIDTH, COLS
    );
    exit(EXIT_FAILURE);
  }

  const int game_win_offset_y = (LINES - GAME_HEIGHT) / 2;
  const int game_win_offset_x = (COLS - GAME_WIDTH) / 2;

  *game_win = newwin(
    GAME_HEIGHT, GAME_WIDTH,
    game_win_offset_y,
    game_win_offset_x
  );

  if(!*game_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    exit(EXIT_FAILURE);
  }

  *action_win = newwin(
    ACTION_WIN_HEIGHT, ACTION_WIN_WIDTH,
    game_win_offset_y + BORDER_HEIGHT,
    game_win_offset_x + BORDER_WIDTH
  );

  if(!*action_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    delwin(*game_win);
    exit(EXIT_FAILURE);
  }

  *info_win = newwin(
    INFO_WIN_HEIGHT, PREVIEW_WIN_WIDTH,
    game_win_offset_y + BORDER_HEIGHT,
    game_win_offset_x + ACTION_WIN_WIDTH + 2 * BORDER_WIDTH +
      (SIDEBAR_WIDTH - INFO_WIN_WIDTH) / 2
  );

  if(!*info_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    delwin(*action_win);
    delwin(*game_win);
    exit(EXIT_FAILURE);
  }

  *preview_win = newwin(
    PREVIEW_WIN_HEIGHT, PREVIEW_WIN_WIDTH,
    game_win_offset_y + GAME_HEIGHT - (BORDER_HEIGHT + PREVIEW_WIN_HEIGHT),
    game_win_offset_x + ACTION_WIN_WIDTH + 2 * BORDER_WIDTH +
      (SIDEBAR_WIDTH - PREVIEW_WIN_WIDTH) / 2
  );

  if(!*preview_win) {
    mvaddstr(LINES / 2, 0, "Could not initialize game window");
    delwin(*info_win);
    delwin(*action_win);
    delwin(*game_win);
    exit(EXIT_FAILURE);
  }
}

void init_info_text(WINDOW *const info_win)
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

int main(void)
{
  initscr();
  atexit(on_exit);

  cbreak();
  noecho();
  curs_set(0);

  init_colors();

  WINDOW *game_win = NULL, *action_win = NULL,
         *info_win = NULL, *preview_win = NULL;
  init_windows(&game_win, &action_win, &info_win, &preview_win);

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

  init_info_text(info_win);

  refresh();

  wrefresh(game_win);
  wrefresh(action_win);
  wrefresh(info_win);
  wrefresh(preview_win);

  delwin(preview_win);
  delwin(info_win);
  delwin(action_win);
  delwin(game_win);

  return EXIT_SUCCESS;
}


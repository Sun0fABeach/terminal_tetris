#include <stdlib.h>
#include <pthread.h>
#include "input.h"
#include "game.h"

typedef enum game_state {
  START_SCREEN,
  PLAYING,
} game_state_e;

void init_threading(void);
void tear_down_threading(void);
void input_loop(void);
bool handle_start_screen_input(int key);
bool handle_play_input(int key);
void do_push_down(void);
void *tick_loop(void *dummy_param);

game_state_e game_state = START_SCREEN;

struct {
  pthread_mutex_t mutex;
  pthread_t tick_thread;
  bool end_tick_thread;
} thread_data = {
  .mutex = PTHREAD_MUTEX_INITIALIZER,
  .end_tick_thread = false,
};


int main(void)
{
  if(!init_game()) {
    getch();
    return EXIT_FAILURE;
  }

  init_input();
  greet_player();
  init_threading();
  input_loop();
  tear_down_threading();
  tear_down_game();

  return EXIT_SUCCESS;
}

void init_threading(void)
{
  pthread_create(&thread_data.tick_thread, NULL, tick_loop, NULL);
}

void tear_down_threading(void)
{
  pthread_join(thread_data.tick_thread, NULL);
  pthread_mutex_destroy(&thread_data.mutex);
}

void input_loop(void)
{
  while(true) {
    const int key = read_key();
    bool exit = false;

    pthread_mutex_lock(&thread_data.mutex);

    switch(game_state) {
      case START_SCREEN:
        if(!handle_start_screen_input(key)) {
          thread_data.end_tick_thread = true;
          exit = true;
        }
        break;
      case PLAYING:
        if(!handle_play_input(key)) {
          thread_data.end_tick_thread = true;
          exit = true;
        }
        break;
    }

    pthread_mutex_unlock(&thread_data.mutex);

    flush_input();

    if(exit)
      return;
  }
}

bool handle_start_screen_input(const int key)
{
  switch(key) {
    case KEY_START:
      setup_play();
      set_new_piece();
      game_state = PLAYING;
      return true;
    case KEY_QUIT:
      return false;
    default:
      return true;
  }
}

bool handle_play_input(const int key)
{
  switch(key) {
    case KEY_ROTATE_LEFT:
      rotate_piece_left();
      return true;

    case KEY_ROTATE_RIGHT:
      rotate_piece_right();
      return true;

    case KEY_MOVE_LEFT:
      move_piece_left();
      return true;

    case KEY_MOVE_RIGHT:
      move_piece_right();
      return true;

    case KEY_PUSH_DOWN:
      do_push_down();
      return true;

    case KEY_QUIT:
      return false;

    default:
      return true;
  }
}

void do_push_down(void)
{
  const move_result_e move_res = push_piece_down();

  if(move_res == MOVE_DONE)
    return;

  if(move_res == PIECE_STUCK && game_is_lost()) {
    game_over();
    game_state = START_SCREEN;
    return;
  }

  set_new_piece();
}

/* return type & param just here to satisfy func signature of pthread_create */
void *tick_loop(void *dummy_param)
{
  constexpr int TICK_MS = 500;

  while(true) {
    pthread_mutex_lock(&thread_data.mutex);

    if(thread_data.end_tick_thread) {
      pthread_mutex_unlock(&thread_data.mutex);
      return NULL;
    }

    if(game_state == PLAYING)
      do_push_down();

    pthread_mutex_unlock(&thread_data.mutex);
    napms(TICK_MS);
  }

  return dummy_param; // this is only here to not get unused param warning
}

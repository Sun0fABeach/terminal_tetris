#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "input.h"
#include "game.h"

typedef enum game_state {
  START_SCREEN,
  PLAYING,
} game_state_e;

static void init_threading(void);
static void tear_down_threading(void);
static void input_loop(void);
static bool handle_start_screen_input(int key);
static bool handle_play_input(int key);
static void do_push_down(void);
static void *tick_loop(void *dummy_param);
static bool lock_mutex(void);

static game_state_e game_state = START_SCREEN;
static bool discard_concurrent_action = false;

static struct {
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

static void init_threading(void)
{
  pthread_create(&thread_data.tick_thread, NULL, tick_loop, NULL);
}

static void tear_down_threading(void)
{
  pthread_join(thread_data.tick_thread, NULL);
  pthread_mutex_destroy(&thread_data.mutex);
}

static void input_loop(void)
{
  while(true) {
    const int key = read_key();
    bool exit = false;

    if(!lock_mutex()) {
      flush_input();
      continue;
    }

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

static bool handle_start_screen_input(const int key)
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

static bool handle_play_input(const int key)
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

static void do_push_down(void)
{
  const move_result_e move_res = push_piece_down();

  switch(move_res) {
    case PIECE_STUCK:
      if(game_is_lost()) {
        game_over();
        game_state = START_SCREEN;
        discard_concurrent_action = true;
        break;
      }
      set_new_piece();
      break;
    case LINES_CLEARED:
      discard_concurrent_action = true;
      // fall through
    case PIECE_PLACED:
      set_new_piece();
      break;
    case MOVE_DONE:
    default:
      break;
  }
}

/* return type & param just here to satisfy func signature of pthread_create */
static void *tick_loop(void *dummy_param)
{
  constexpr int TICK_MS = 500;

  while(true) {
    napms(TICK_MS);

    if(!lock_mutex())
      continue;

    if(thread_data.end_tick_thread) {
      pthread_mutex_unlock(&thread_data.mutex);
      return NULL;
    }

    if(game_state == PLAYING)
      do_push_down();

    pthread_mutex_unlock(&thread_data.mutex);
  }

  return dummy_param; // this is only here to not get unused param warning
}

static bool lock_mutex(void)
{
  if(pthread_mutex_trylock(&thread_data.mutex) == EBUSY) {
    pthread_mutex_lock(&thread_data.mutex);
    // we waited for the other thread. it may have told us to back off
    if(discard_concurrent_action) {
      pthread_mutex_unlock(&thread_data.mutex);
      return false;
    }
  } else {
    discard_concurrent_action = false;
  }

  return true;
}

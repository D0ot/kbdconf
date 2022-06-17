#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "linux/input.h"

#define TEST_AND_OUTPUT(E, T) \
  do { \
    if(E.type == T) { \
      std::cerr << #T; \
    } \
  }while(0)

void send_key(uint16_t code, uint32_t value) {
  struct input_event event;
  gettimeofday(&event.time, NULL);
  event.type = EV_KEY;
  event.code = code;
  event.value = value;
  fwrite(&event, sizeof(input_event), 1, stdout);
}

void log_input(struct input_event &event) {
    TEST_AND_OUTPUT(event, EV_KEY);
    TEST_AND_OUTPUT(event, EV_REL);
    TEST_AND_OUTPUT(event, EV_LED);
    TEST_AND_OUTPUT(event, EV_ABS);
    TEST_AND_OUTPUT(event, EV_CNT);
    TEST_AND_OUTPUT(event, EV_FF);
    TEST_AND_OUTPUT(event, EV_ABS);
    TEST_AND_OUTPUT(event, EV_FF_STATUS);
    TEST_AND_OUTPUT(event, EV_MAX);
    TEST_AND_OUTPUT(event, EV_REP);
    TEST_AND_OUTPUT(event, EV_SND);
    TEST_AND_OUTPUT(event, EV_PWR);
    TEST_AND_OUTPUT(event, EV_SYN);
    TEST_AND_OUTPUT(event, EV_MSC);
    
    std::cerr << std::endl;
    std::cerr << "event.type: " << event.type<< std::endl;
    std::cerr << "event.code : " << event.code << std::endl;
    std::cerr << "event.value : " << event.value << std::endl;
    std::cerr << "event.time.sec: " << event.time.tv_sec<< std::endl;
    std::cerr << "event.time.usec: " << event.time.tv_usec << std::endl;

    struct timeval cur;
    gettimeofday(&cur, NULL);

    std::cerr << "cur.sec: " << cur.tv_sec << std::endl;
    std::cerr << "cur.usec: " << cur.tv_usec << std::endl;

    std::cerr << std::endl;
}

int main(void) {
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);

  struct input_event event;

  bool pending_caps = false;
  bool left_ctrl = false;

  while(fread(&event, sizeof(event), 1, stdin) == 1) {
    if(event.type == EV_KEY) {

      if(event.code == KEY_CAPSLOCK) {

        if(event.value == 1) {
          pending_caps = true;
          continue;
        } else if(event.value == 0) {
          if(pending_caps) {
            send_key(KEY_ESC, 1);
            send_key(KEY_ESC, 0);
            pending_caps = false;
            continue;
          } else {
            event.code = KEY_RIGHTCTRL;
            goto over;
          }
        } else if(event.value == 2) {
          // one click function timeout
          if(pending_caps) {
            pending_caps = false;
            send_key(KEY_RIGHTCTRL, 1);
            continue;
          } else {
            event.code = KEY_RIGHTCTRL;
            goto over;
          }
        }
      }

      if(pending_caps) {
        pending_caps = false;
        send_key(KEY_RIGHTCTRL, 1);
      }

      if(event.code == KEY_LEFTCTRL) {

        if(event.value == 0) {
          left_ctrl = false;
        } else {
          left_ctrl = true;
        }
        continue;
      }

      if(event.code == KEY_J) {
        if(left_ctrl) {
          event.code = KEY_DOWN;
        } 
        goto over;
      }

      if(event.code == KEY_K) {
        if(left_ctrl) {
          event.code = KEY_UP;
        }
        goto over;
      }


      if(event.code == KEY_H) {
        if(left_ctrl) {
          event.code = KEY_LEFT;
        }
        goto over;
      }

      if(event.code == KEY_L) {
        if(left_ctrl) {
          event.code = KEY_RIGHT;
        }
        goto over;
      }


    }


over:
     fwrite(&event, sizeof(event), 1, stdout);
  }
}

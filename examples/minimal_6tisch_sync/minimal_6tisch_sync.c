/*
 * \author Apostolos Karalis <akaralis@unipi.gr>
 */
#include "contiki.h"
#include "net/routing/routing.h"
#include "dev/leds.h"
#include "dev/button-hal.h"
#include "helper_functions.h"
#include "net/mac/tsch/tsch.h"
/*---------------------------------------------------------------------------*/
PROCESS(minimal_6tisch_sync, "Minimal 6TiSCH Sync Experiment");
AUTOSTART_PROCESSES(&minimal_6tisch_sync);

static struct etimer et;

void update_led_state(){
    static int led_on = 0;

    if(led_on){
        leds_off(LEDS_ALL);
        led_on = 0;
        etimer_set(&et, 5 * CLOCK_SECOND);
        return;
    }

    switch (node_state){  // node_state is declared in helper_functions.h
#   if NODE_TYPE == SAMPLER
        case SAMPLING_NOT_STARTED:
            leds_set(LEDS_CONF_RED | LEDS_CONF_BLUE); // magenta
            break;

        case SAMPLING_STARTED:
            leds_set(LEDS_CONF_BLUE);
            break;

        case SAMPLING_COMPLETED:
            leds_set(LEDS_CONF_GREEN);
            break;
#   else // NODE_TYPE == PAN_COORDINATOR || NODE_TYPE == ADVERTISER
        case ADVERTISING_NOT_STARTED:
            leds_set(LEDS_CONF_RED | LEDS_CONF_GREEN); // yellow
            break;

        case ADVERTISING_STARTED:
            leds_set(LEDS_CONF_BLUE | LEDS_CONF_GREEN); // cyan
            break;

        case ADVERTISING_STOPPED:
            leds_set(LEDS_CONF_RED | LEDS_CONF_GREEN); // yellow
            break;
#   endif
        case ERROR:
            leds_set(LEDS_CONF_RED);
    }

    // Note that the led is kept open for a very short time in order to save energy
    etimer_set(&et, 1);
    led_on = 1;
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(minimal_6tisch_sync, ev, data)
{

  PROCESS_BEGIN();

#if NODE_TYPE == PAN_COORDINATOR
  NETSTACK_ROUTING.root_start();
#endif

  update_led_state();
  while(1){
    PROCESS_WAIT_EVENT();

    if(ev == PROCESS_EVENT_TIMER && data == &et){ // led state update event
        update_led_state();
    } else if(ev == button_hal_periodic_event && ((button_hal_button_t *)data)->press_duration_seconds > 4){
        terminate_stats_recording();
        break;
        }
    }

  // after termination request
  static int flag = 0;
  while(1){
   flag++;
   switch(flag){
       case 1:
           leds_set(LEDS_CONF_RED);
           break;
       case 2:
           leds_set(LEDS_CONF_BLUE | LEDS_CONF_GREEN);
           break;
       case 3:
           flag = 0;
           leds_set(LEDS_CONF_GREEN);
      }

      etimer_set(&et, 1 * CLOCK_SECOND);
      PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &et);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
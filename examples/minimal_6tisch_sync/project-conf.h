/*
 * \author Apostolos Karalis <akaralis@unipi.gr>
 */
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#include "default_chs.h"

#define SYNC_TIME_EXPERIMENT // Enables the code of the experiment

/* Define if the SAMPLERS do not start the sampling process automatically, but the pressing of the user button is required. */
#define START_SAMPLING_WITH_USER_BUTTON 1

/* Define if the statistics will be saved in the sd card or will be printed to the console */
#define STORING_STATS_IN_SD 1

// the total number of channels available to the network
#define NUM_CHANNELS 16

/* Define the number of slots in the slotframe. It is noted that Contiki runs the minimal 6TiSCH schedule by default. */
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 101

/* TSCH slotframe duration in clock seconds/ticks. Note that, by default Contiki uses the default timeslot template,
 * which means that the duration of a timelslot is 10ms. */
#define SLOTFRAME_DURATION_CS ((clock_time_t)(TSCH_SCHEDULE_DEFAULT_LENGTH * 0.01 * CLOCK_SECOND))

#if NODE_TYPE == SAMPLER

    /* Specify the values of scan period for which samples will be collected. The values are expressed in slotframes. */
    #define SCAN_PERIODS (double[]){0.5, 2, 3.5, 5, 6.5, 8, 9.5, 11, 12.5, 14, 15.5, 16, 17, 18.5, 20, \
                                     (double) (CLOCK_SECOND) / SLOTFRAME_DURATION_CS  /* Contiki's Generic Default Scan Period */ \
                                     (double) (1.6 * CLOCK_SECOND) / SLOTFRAME_DURATION_CS /* OpenWSN Generic Default Scan Period */ \
                                     }

    #define NUM_SCAN_PERIODS (sizeof(SCAN_PERIODS) / (sizeof(SCAN_PERIODS[0])))

    // Define the number of samples that will be collected for each scan period
    #define NUM_SAMPLES 750

    /* Defines the file where the samples about the synchronization time will be saved if the storing in the sd card has
     * been selected, otherwise does not taken into account */
    #define STATS_FILE "syncsamp.csv"

#else // NODE_TYPE == PAN_COORDINATOR || NODE_TYPE == ADVERTISER

    /* Define the EB period. It has effect only for the advertisers (including the PAN coordinator) */
    #define TSCH_CONF_EB_PERIOD (4 * SLOTFRAME_DURATION_CS)
    #define TSCH_CONF_MAX_EB_PERIOD TSCH_CONF_EB_PERIOD

    /* Defines the file where the EB transmissions will be recorded if the storing in the sd card has been selected,
     * otherwise does not taken into account */
    #define STATS_FILE "EBtrans.csv"

    // Define the scan period (for the procedure of initial synchronization).
    #define TSCH_CONF_CHANNEL_SCAN_DURATION (NUM_CHANNELS * SLOTFRAME_DURATION_CS)

#endif

#define RPL_CONF_SUPPORTED_OFS {&rpl_of0}
#define RPL_CONF_OF_OCP RPL_OCP_OF0

// Enables the Energest module
#define ENERGEST_CONF_ON 1

// Defines the PAN ID
#define IEEE802154_CONF_PANID 0x83a7

#define LLSEC802154_CONF_ENABLED 1
#define TSCH_SECURITY_CONF_K1 {0x37, 0x15, 0xfb, 0x31, 0, 0x64, 0x85, 0x16, 0x57, 0xa5, 0xc4, 0x18, 0xcd, 0x8b, 0x20, 0x19}
#define TSCH_SECURITY_CONF_K2 {0x58, 0x45, 0xe, 0x19, 0x69, 0xfe, 0xb, 0x31, 0xc8, 0xc, 0x7a, 0x3, 0x2f, 0x9a, 0xb2, 0xd8}

#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE DEFAULT_TSCH_HOPPING_SEQUENCE(NUM_CHANNELS)

#define LOG_CONF_LEVEL_HELPER_FUNCTIONS              LOG_LEVEL_DBG

// Some optimizations to save memory when STORING_STATS_IN_SD is 1
#define  _FS_EXFAT 0 // 0disable the support for EXFAT - use FAT32
#define _USE_LFN 0 // disable the support for Long File Names - only 8.3 filenames are supported

////////
#define TSCH_CONF_MAC_MAX_FRAME_RETRIES 3 // retransmission attempts
#endif //PROJECT_CONF_H_
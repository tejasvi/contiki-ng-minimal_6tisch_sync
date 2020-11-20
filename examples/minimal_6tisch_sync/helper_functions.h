/*
 * \author Apostolos Karalis <akaralis@unipi.gr>
 */
#ifndef MINIMAL_6TISCH_SYNC_TESTBED_HELPER_FUNCTIONS_H
#define MINIMAL_6TISCH_SYNC_TESTBED_HELPER_FUNCTIONS_H

#include <stdint.h>
#include "project-conf.h"

extern int stats_recording_terminated;
typedef enum {
#if NODE_TYPE == SAMPLER
    SAMPLING_NOT_STARTED,
    SAMPLING_STARTED,
    SAMPLING_COMPLETED,
#else // NODE_TYPE == PAN_COORDINATOR || NODE_TYPE == ADVERTISER
    ADVERTISING_NOT_STARTED,
    ADVERTISING_STARTED,
    ADVERTISING_STOPPED,
#endif
    ERROR
} node_state_t;

extern node_state_t node_state;

#if STORING_STATS_IN_SD
#include "lib/fs/fat/ff.h"

extern FATFS sd_card;
extern FIL results_file;
/**
 * Prepare the sd card for the writing of statistics.
 * \return 1 if the preparation succeeded, otherwise 0
 * */
int prepare_sd();
#endif /* STORING_STATS_IN_SD */

#if NODE_TYPE == SAMPLER
/**
 * This function is called when the sampling starts.
 */
void sampling_started();

/**
 * This function is called when a (new) channel selection is made during the scan, and it records the channel.
 * If STORING_STATS_IN_SD is 1 the channel is recorded in STATS_FILE, otherwise in the console.
 * \param channel the selected channel
 * \return 1 in case of success or if the recording of statistics has been terminated, otherwise 0
 */
int record_channel_selection(int channel);

/**
 * Save the given sample in STATS_FILE if STORING_STATS_IN_SD is 1, otherwise the sample is printed to the console.
 * \param scan_period the scan period
 * \param sync_time the sync time, in seconds
 * \param cpu_active_time the time that the cpu was active, in seconds
 * \param lpm_time the time that the node was in Low Power Mode, in seconds
 * \param dlpm_time the time that the node was in Deep Low Power Mode, in seconds
 * \param asn the ASN when the synchronization done (i.e., the asn where the EB was received)
 * \return 1 in case of success or if the recording of statistics has been terminated, otherwise 0
 */
int
record_sync_sample(double scan_period, double sync_time, double cpu_active_time, double lpm_time, double dlpm_time, uint64_t asn, double time_elapsed_since_reception_slot_start);

/**
 * This function is called when the sampling finishes.
 * If STORING_STATS_IN_SD is 1, it also makes the SD card ready to be removed from the device.
 */
void sampling_completed();

#else // NODE_TYPE == PAN_COORDINATOR || NODE_TYPE == ADVERTISER

/**
 * This function is called when the advertising starts.
 */
void advertising_started();

/**
 * This function is called when the advertising is (temporarily) stopped in the case of a disconnection.
 */
void advertising_stopped();

/**
 * Record that an EB transmission occurred in the slot with the given ASN.
 * If STORING_STATS_IN_SD is 1 the event is recorded in STATS_FILE, otherwise in the console.
 * \param asn the ASN of the slot where the transmission occurred
 * \param channel the channel where the transmission happened
 * \return 1 in case of success or if the recording of statistics has been terminated, otherwise return 0
 */
int record_eb_transmission(uint64_t asn, int channel);

#endif  /* NODE_TYPE */

/**
 * This function is called when an error occurred during the experiment.
 * @param msg a message that describes the error
 */
void error(const char *msg);

/**
 * End the recording of statistics and if STORING_STATS_IN_SD is 1, it makes the SD card ready to be removed from
 * the device.
 */
void terminate_stats_recording();

#endif //MINIMAL_6TISCH_SYNC_TESTBED_HELPER_FUNCTIONS_H
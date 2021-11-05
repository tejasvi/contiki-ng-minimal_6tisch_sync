/*
 * \author Apostolos Karalis <akaralis@unipi.gr>
 */
#include "helper_functions.h"
#include "os/sys/log.h"

/* Log configuration */
#define LOG_MODULE "HELPER_FUNCTIONS"
#define LOG_LEVEL LOG_LEVEL_DBG

int stats_recording_terminated = 0;

#if STORING_STATS_IN_SD
#include "lib/fs/fat/ff.h"

FATFS sd_card;
FIL results_file;

int prepare_sd() {
    if (f_mount(&sd_card, "", 1) != FR_OK) {
        error("Error! No SD card found. Please insert an SD card and restart the device. \n");
        return 0;
    }

    //check if the lock file exists
    FILINFO fno;
    if (f_stat("~lock", &fno) == FR_OK){
        error("The statistics file of the previous completed experiment exists.If you want to start a new experiment,"
              "please restart the device.");
        f_unlink("~lock");
        return 0;
    }
    //

    if (f_open(&results_file, STATS_FILE, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
        error("Error creating statistic file. \n");
        return 0;
    }

    //add csv header
    if (f_printf(&results_file,
#   if NODE_TYPE == SAMPLER
            "scannedChannels,nodeID,channels,slots,scanPeriod,syncTime,cpuActiveTime,LPMTIme,DLPMTime,ASN,timeElapsedSinceReceptionSlotStartTime\n"
#   else
            "nodeID,channel,ASN\n"
#   endif
        ) == -1
        || f_sync(&results_file) != FR_OK) {
        error("Error writing to the statistic file. \n");
        return 0;
    }

    return 1;
}

static void createLockFile(){
    FIL lock_file;
    f_open(&lock_file, "~lock", FA_CREATE_ALWAYS);
    f_close(&lock_file);
}
#endif /* STORING_STATS_IN_SD */

#if NODE_TYPE == SAMPLER
node_state_t node_state = SAMPLING_NOT_STARTED; // initial state

void sampling_started() {
    LOG_INFO("Sampling started.\n");
    node_state = SAMPLING_STARTED;
}

int record_channel_selection(int channel) {
    if(stats_recording_terminated){
        return 1;
    }

#   if STORING_STATS_IN_SD
    if (f_printf(&results_file, "%d ", channel) == -1)
#   else
    if (printf("{ScannedChannel:%d}\n", channel) < 0)
#   endif /* STORING_STATS_IN_SD */
    {
        error("Error writing to the results file. \n");
        return 0;
    }

    return 1;
}

typedef struct {
    char str_array[26];
} number_as_string;

/**
 * Contiki does not support the printing of real numbers. This function supports the printing of real numbers by
 * converting them into strings.
 */
static number_as_string *real_to_str(double real_number, number_as_string *output) {
    long long int_part = (long long) real_number;
    double dec_part = real_number - int_part;
    snprintf(
            output->str_array, 26, "%lld.%06llu", int_part, (unsigned long long) (dec_part * 1000000)
    ); // six digits after the floating point
    return output;
}

int
record_sync_sample(double scan_period, double sync_time, double cpu_active_time, double lpm_time, double dlpm_time, uint64_t asn, double time_elapsed_since_reception_slot_start) {
    if(stats_recording_terminated){
        return 1;
    }

    number_as_string scan_period_as_string, sync_time_as_string, cpu_time_as_string, lpm_time_as_string, dlpm_time_as_string, time_elapsed_since_reception_slot_start_as_string;
    char asn_as_string[21];
    snprintf(asn_as_string, 21, "%llu", (unsigned long long) asn);

    if (
#   if STORING_STATS_IN_SD
f_printf(&results_file, ",%u,%d,%d,%s,%s,%s,%s,%s,%s,%s\n",
#   else
printf("sync_sample:{nodeID:%u channels:%d, slots:%d, scanPeriod:%s, syncTime:%s, cpuActiveTime:%s, LPMTIme:%s, DLPMTime:%s, ASN: %s}\n",
#   endif
       IEEE_ADDR_NODE_ID,
       sizeof(TSCH_CONF_DEFAULT_HOPPING_SEQUENCE),
       TSCH_SCHEDULE_CONF_DEFAULT_LENGTH,
       real_to_str(scan_period, &scan_period_as_string)->str_array,
       real_to_str(sync_time, &sync_time_as_string)->str_array,
       real_to_str(cpu_active_time, &cpu_time_as_string)->str_array,
       real_to_str(lpm_time, &lpm_time_as_string)->str_array,
       real_to_str(dlpm_time, &dlpm_time_as_string)->str_array,
       asn_as_string,
       real_to_str(time_elapsed_since_reception_slot_start, &time_elapsed_since_reception_slot_start_as_string)->str_array
) < 0) {
        error("Error writing to the results file. \n");
        return 0;
    }

#   if STORING_STATS_IN_SD
    if(f_sync(&results_file) != FR_OK){
        error("Error writing to the results file. \n");
        return 0;
    }
#   endif
    return 1;
}

void sampling_completed() {
    LOG_INFO("Sampling completed.\n");
    node_state = SAMPLING_COMPLETED;
# if STORING_STATS_IN_SD
    if(f_close(&results_file) != FR_OK){
        error("Error closing the results file. \n");
    } else {
        createLockFile();
    }
    f_mount(0, "", 0);
#   endif
}

#else // NODE_TYPE == PAN_COORDINATOR || NODE_TYPE == ADVERTISER
node_state_t node_state = ADVERTISING_NOT_STARTED; // initial state

void advertising_started() {
    LOG_INFO("The advertising started.\n");
    node_state = ADVERTISING_STARTED;
}

void advertising_stopped() {
    LOG_INFO("The advertising stopped.\n");
    node_state = ADVERTISING_STOPPED;
}

int record_eb_transmission(uint64_t asn, int channel) {

    if (stats_recording_terminated) {
        return 1;
    }

#   if STORING_STATS_IN_SD
    // convert asn to string because f_printf does not suport (unsigned) long long integers
    char asn_as_string[21];
    snprintf(asn_as_string, 21, "%llu", asn);

    if (f_printf(&results_file, "%u,%d,%s\n", IEEE_ADDR_NODE_ID, channel, asn_as_string) == -1)
#   else
    if (printf("EB_transmission:{nodeID:%u, channel:%d, ASN:%llu}\n", IEEE_ADDR_NODE_ID, channel, asn) < 0)
#   endif /* STORING_STATS_IN_SD */
    {
        error("Error writing to the results file. \n");
        return 0;
    }

    return 1;
}
#endif /* NODE_TYPE */

void error(const char *msg) {
    LOG_ERR(msg);
    node_state = ERROR;
}

void terminate_stats_recording() {
    stats_recording_terminated = 1;
#   if STORING_STATS_IN_SD
    if(f_close(&results_file) != FR_OK){
        error("Error closing the results file. \n");
    } else {
        createLockFile();
    }
    f_mount(0, "", 0);
#   endif
    LOG_INFO("Statistics Recording Terminated.\n");
}
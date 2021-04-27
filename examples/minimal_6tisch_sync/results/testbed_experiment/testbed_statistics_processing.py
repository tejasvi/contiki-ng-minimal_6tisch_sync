import math

import numpy as np
import pandas as pd

CLOCK_SECOND = 128  # in hardware clock ticks
SLOT_DURATION = 0.01  # in seconds
TX_OFFSET = 0.00212  # in seconds
OUTPUT_CSV_FILE = "./initial_experiment/16channels/testbed_final_statistics.csv"

NUMCHANNELS = 16

DEFAULT_CHS = {
    4: [11, 13, 14, 12],
    8: [16, 12, 15, 11, 14, 13, 17, 18],
    12: [16, 19, 15, 20, 13, 12, 21, 18, 22, 11, 14, 17],
    16: [16, 17, 23, 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21]
}

CHANNELS = DEFAULT_CHS[NUMCHANNELS]

advertisers_stat_files = [
    "./initial_experiment/16channels/NODE1/EBTRANS.CSV",
    "./initial_experiment/16channels/NODE2/EBTRANS.CSV",
    "./initial_experiment/16channels/NODE3/EBTRANS.CSV",
    "./initial_experiment/16channels/NODE4/EBTRANS.CSV",
]

samplers_stat_files = [
    "./initial_experiment/16channels/NODE5/SYNCSAMP.CSV",
    "./initial_experiment/16channels/NODE6/SYNCSAMP.CSV",
    "./initial_experiment/16channels/NODE7/SYNCSAMP.CSV",
    "./initial_experiment/16channels/NODE8/SYNCSAMP.CSV",
    "./initial_experiment/16channels/NODE9/SYNCSAMP.CSV",
    "./initial_experiment/16channels/NODE10/SYNCSAMP.CSV",
    "./initial_experiment/16channels/NODE11/SYNCSAMP.CSV",
    "./initial_experiment/16channels/NODE12/SYNCSAMP.CSV"
]


def energy_consumption(cpu_active_time, cpu_lpm_time, listen_time):
    """
    Calculates the power consumption during a synchronization attempt.
    Note that, in our experiments, we use Zolertia RE-Mote B devices (https://github.com/Zolertia/Resources/raw/master/RE-Mote/Hardware/Revision%20B/Datasheets/ZOL-RM0x-B%20-%20RE-Mote%20revision%20B%20Datasheet%20v.1.0.0.pdf)
    :param cpu_active_time the active cpu (microcontroller) time, in seconds
    :param cpu_lpm_time the time where the cpu (microcontroller) was in low power mode
    :param listen_time the time the radio was in listen mode, in seconds
    :return: the energy consumption, in Joules.
    """
    cpu_active_current_consumption = 0.02  # Amperes
    cpu_lpm_current_consumption = 0.0000013  # Amperes
    listen_current_consumption = 0.02  # Amperes
    volts = 3.7  # of the default battery
    return (cpu_active_time * cpu_active_current_consumption +
            cpu_lpm_time * cpu_lpm_current_consumption +
            listen_time * listen_current_consumption) * volts


def bootstrap(data, n=1000, func=np.mean):
    """
    Generate `n` bootstrap samples, evaluating `func`
    at each resampling. `bootstrap` returns a function,
    which can be called to obtain confidence intervals
    of interest.
    http://www.jtrive.com/the-empirical-bootstrap-for-confidence-intervals-in-python.html
    """
    simulations = list()
    sample_size = len(data)
    for c in range(n):
        itersample = np.random.choice(data, size=sample_size, replace=True)
        simulations.append(func(itersample))
    simulations.sort()

    def ci(p):
        """
        Return 2-sided symmetric confidence interval specified
        by p.
        """
        u_pval = (1 + p) / 2.
        l_pval = (1 - u_pval)
        l_indx = int(np.floor(n * l_pval))
        u_indx = int(np.floor(n * u_pval))
        return (simulations[l_indx], simulations[u_indx])

    return (ci)


# load EB transmissions
eb_transmissions = pd.concat(
    [pd.read_csv(file) for file in advertisers_stat_files],
    ignore_index=True, sort=False
)

sample_points = pd.concat(
    [pd.read_csv(file) for file in samplers_stat_files],
    ignore_index=True, sort=False
)

NUM_CHANNELS = sample_points.head(1)["channels"].iat[0]
NUM_SLOTS = sample_points.head(1)["slots"].iat[0]
SLOTFRAME_DURATION = NUM_SLOTS * SLOT_DURATION
CDF_MAX_STEP = math.ceil(sample_points["syncTime"].max() / SLOTFRAME_DURATION)

final_statistics = pd.DataFrame(
    {
        "channels": [],
        "slots": [],
        "scanPeriod": [],
        "pEB": [],
        "pSR": [],
        "avgSyncTime": [],
        "avgSyncTimeCILow": [],
        "avgSyncTimeCIHigh": [],
        "avgEnergyConsumption": [],
        "avgEnergyConsumptionCILow": [],
        "avgEnergyConsumptionCIHigh": [],
        **{"CDF(" + str(k) + ")": [] for k in range(1, CDF_MAX_STEP + 1)}
    }
)

scan_periods = sample_points["scanPeriod"].unique()
for sp in scan_periods:
    sp_sample = sample_points.query("scanPeriod == @sp")  # the sample of the current scan period
    sp_duration = math.ceil(sp * (SLOTFRAME_DURATION * CLOCK_SECOND)) / CLOCK_SECOND
    sp_channel_successes = {ch: 0 for ch in CHANNELS}

    # calculate pEB #
    first_sample_point = sp_sample.query("ASN == ASN.min()").query("syncTime == syncTime.max()")
    sp_sampling_start_asn = (
            first_sample_point["ASN"].iat[0] - math.ceil(first_sample_point["syncTime"].iat[0] / SLOT_DURATION)
    )
    sp_sampling_end_asn = sp_sample.query("ASN == ASN.max()")["ASN"].iat[0]

    num_eb_trans = (
        eb_transmissions.query("ASN >= @sp_sampling_start_asn and ASN <= @sp_sampling_end_asn")["ASN"].unique().size
    )

    num_minimal_cells = math.floor((sp_sampling_end_asn - sp_sampling_start_asn) / NUM_SLOTS) + 1
    pEB = num_eb_trans / num_minimal_cells
    #################

    # calculate pSR
    pSR = {}
    numtrans_in_scanned_channels = {ch: 0 for ch in CHANNELS}
    for sample_point in sp_sample.itertuples(index=False):
        scannedChannels = sample_point.scannedChannels.split()
        sp_channel_successes[int(scannedChannels[-1])] += 1
        first_scan_start_time = sample_point.ASN * SLOT_DURATION + sample_point.timeElapsedSinceReceptionSlotStartTime - sample_point.syncTime

        i = 0
        while i < len(scannedChannels):
            scannedChannel = int(scannedChannels[i])
            j = i
            # while we have the same scanned channel in the next scan period
            while j + 1 < len(scannedChannels) and scannedChannels[j + 1] == scannedChannel:
                j += 1

            scan_start_time = first_scan_start_time + i * sp_duration

            # approximate calculation of scan_start_asn
            if scan_start_time % SLOT_DURATION <= TX_OFFSET:
                scan_start_asn = scan_start_time // SLOT_DURATION
            else:
                scan_start_asn = scan_start_time // SLOT_DURATION + 1

            if j == len(scannedChannels) - 1:
                scan_end_asn = sample_point.ASN
            else:
                scan_end_time = first_scan_start_time + (j + 1) * sp_duration
                # approximate calculation of scan_end_asn
                if scan_end_time % SLOT_DURATION > TX_OFFSET:
                    scan_end_asn = scan_end_time // SLOT_DURATION
                else:
                    scan_end_asn = scan_end_time // SLOT_DURATION - 1

            numtrans_in_scanned_channels[scannedChannel] += eb_transmissions.query(
                "ASN >= @scan_start_asn and ASN <= @scan_end_asn and channel==@scannedChannel"
            )["ASN"].unique().size

            i = j + 1

    for ch in numtrans_in_scanned_channels:
        pSR[ch] = sp_channel_successes[ch] / numtrans_in_scanned_channels[ch]

    # find the avg synchronization time and the related confidence interval
    # the confidence interval is calculated using the bootstrapping method
    bs_sync_time = bootstrap(sp_sample["syncTime"].values, n=10000)(0.95)

    # find the avg energy consumption and the related confidence interval
    energy_sample_points = []
    for row in sp_sample.itertuples(index=False):
        energy_sample_points.append(energy_consumption(row.cpuActiveTime, row.LPMTIme, row.syncTime))

    bs_energy = bootstrap(energy_sample_points, n=10000)(0.95)

    ### Calculate CDF ###
    counter = [0]
    for k in range(1, CDF_MAX_STEP + 1):
        counter.append(
            sp_sample.query("@k-1 < syncTime/@SLOTFRAME_DURATION and @k >= syncTime/@SLOTFRAME_DURATION").shape[0]
        )  # we want k == ceil(syncTime/@SLOTFRAME_DURATION)

    cdf = [0]
    sum_counters = 0
    for k in range(1, CDF_MAX_STEP + 1):
        sum_counters += counter[k]
        cdf.append(sum_counters / sp_sample.shape[0])
    #####################

    final_statistics = final_statistics.append(
        {
            "channels": NUM_CHANNELS,
            "slots": NUM_SLOTS,
            "scanPeriod": sp,
            "pEB": pEB,
            "pSR": pSR,
            "avgSyncTime": sp_sample["syncTime"].mean(),  # bs_sync_time.value,
            "avgSyncTimeCILow": bs_sync_time[0],  # bs_sync_time.lower_bound,
            "avgSyncTimeCIHigh": bs_sync_time[1],  # bs_sync_time.upper_bound,
            "avgEnergyConsumption": sum(energy_sample_points) / len(energy_sample_points),
            "avgEnergyConsumptionCILow": bs_energy[0],
            "avgEnergyConsumptionCIHigh": bs_energy[1],
            **{"CDF(" + str(k) + ")": cdf[k] for k in range(1, CDF_MAX_STEP + 1)}
        },
        ignore_index=True
    )

final_statistics.to_csv(OUTPUT_CSV_FILE, index=False)

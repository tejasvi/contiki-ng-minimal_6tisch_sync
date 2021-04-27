This project was developed in the context of the paper: 
```
A. Karalis, D. Zorbas and C. Douligeris, "Optimal Initial Synchronization Time in the Minimal 6TiSCH Configuration" in 
IEEE Access.
```
As described in the paper, the project was executed on a testbed consisting of Zolertia RE-MOTE B devices. The 
configuration of the project is defined in `project-conf.h`. To define the role of a node go to the Makefile. To run 
the project on a Zolertia RE-Mote B device, run the following shell command within the project's directory:

```
make clean && make TARGET=zoul BOARD=remote-revb PORT=<your-device-usb-port> minimal_6tisch_sync.upload NODEID=<your-desired-device-id> && make <your-device-usb-port> login
```

It is noted that each node indicates its state by periodically enabling the led of the appropriate color (e.g., the red
led blinks if a fatal error occurred). Moreover, in the case where the results are stored in an sd card, the pressing of the
user button for 5 seconds is required for the safe unmount of the card. For more details, please see 
`minimal_6tisch_sync.c`.

The directory `results` contains the results of the experiments that we conducted. More specifically, it contains the 
following two subdirectories:
- `testbed_experiment`: it contains the primary (raw) data that were produced during the experiments as well as a python3 
  script for the analysis of the raw data. The raw data are included in the subdirectories `initial_experiment` and
  `extra_experiment_openwsn_default`. The subdirectory `extra_experiment_openwsn_default` refers to the additional 
  experiment that we conducted for the default OpenWSN scan period; this case was not included in the initial_experiment.
  Since we conducted experiments for 4, 8, 12 and 16 channels the raw data are divided into four folders one for each 
  case. Apart from the raw data produced by the nodes, these folders also contains a csv file named 
  `testbed_final_statistics.csv`, which was produced by the above-mentioned python script. This script is called 
  `testbed_statistics_processing.py` while the required python packages to run this script are described in `requirement.txt`.
- `testbed_vs_model`: it contains the results of the comparison between the experiment results and the model results.
------------------------------------------
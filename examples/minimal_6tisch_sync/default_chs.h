/*
 * \author Apostolos Karalis <akaralis@unipi.gr>
 */
#ifndef MINIMAL_6TISCH_SYNC_TESTBED_DEFAULT_CHANNEL_HOPPING_SEQUENCES_H
#define MINIMAL_6TISCH_SYNC_TESTBED_DEFAULT_CHANNEL_HOPPING_SEQUENCES_H

/*
 *  The default channel hopping sequences as defined by the standard, depending on the number of channels
 *  used by the network.
 *  An open source calculator for the default channel hopping sequences is available at the following link:
 *  https://gist.github.com/twatteyne/2e22ee3c1a802b685695
 */
#define DEFAULT_TSCH_HOPPING_SEQUENCE_1 (uint8_t[]){11}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_2 (uint8_t[]){11, 12}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_3 (uint8_t[]){11, 13, 12}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_4 (uint8_t[]){11, 13, 14, 12}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_5 (uint8_t[]){11, 13, 14, 15, 12}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_6 (uint8_t[]){16, 12, 15, 11, 13, 14}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_7 (uint8_t[]){14, 13, 15, 11, 16, 12, 17}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_8 (uint8_t[]){16, 12, 15, 11, 14, 13, 17, 18}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_9 (uint8_t[]){11, 13, 12, 16, 17, 18, 19, 14, 15}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_10 (uint8_t[]){16, 12, 19, 13, 17, 14, 20, 18, 15, 11}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_11 (uint8_t[]){16, 12, 11, 20, 17, 18, 14, 13, 19, 15, 21}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_12 (uint8_t[]){16, 19, 15, 20, 13, 12, 21, 18, 22, 11, 14, 17}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_13 (uint8_t[]){15, 13, 20, 19, 17, 23, 16, 12, 21, 22, 14, 11, 18}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_14 (uint8_t[]){14, 11, 21, 18, 16, 19, 17, 20, 22, 24, 15, 23, 12, 13}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_15 (uint8_t[]){17, 22, 24, 18, 12, 11, 25, 13, 19, 16, 14, 15, 20, 23, 21}
#define DEFAULT_TSCH_HOPPING_SEQUENCE_16 (uint8_t[]){16, 17, 23, 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21}

#define CONCAT(x, y) x##y
#define DEFAULT_TSCH_HOPPING_SEQUENCE(channels) CONCAT(DEFAULT_TSCH_HOPPING_SEQUENCE_, channels)

#endif //MINIMAL_6TISCH_SYNC_TESTBED_DEFAULT_CHANNEL_HOPPING_SEQUENCES_H

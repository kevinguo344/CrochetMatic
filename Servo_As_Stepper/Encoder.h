#ifndef ENCODER_H
#define ENCODER_H

// this code was initially found here: https://forum.pjrc.com/threads/610-Teensy-3-Encoder-Lib/page2
// and this AN: http://cache.freescale.com/files/32bit/doc/app_note/AN4381.pdf
//#include "mk20dx256.h"
#define SIM_SCGC6_FTM1_MASK      0x0000000020000000
#define SIM_SCGC6_FTM2_MASK      0x0000000040000000
#define SIM_SCGC3_FTM3_MASK      0x0000000002000000
#define FTM_MODE_FTMEN_MASK      0x0000000000000001
#define FTM_MODE_WPDIS_MASK      0x0000000000000100
#define FTM_CONF_BDMMODE_3       0x00000000000000C0
#define FTM_QDCTRL_QUADMODE_MASK 0b0000000000001000
#define FTM_QDCTRL_QUADEN_MASK   0x0000000000000001
#define FTM_QDCTRL_TOFDIR_MASK   0x0000000000000002
#define FTM_SC_CLKS_3            0x0000000000000018
#define FTM_SC_CLKS_3            0x0000000000000018
#define FTM_PHAFLTREN            0b0000000010000000
#define FTM_PHBFLTREN            0b0000000001000000
#define FTM_CH0FVAL              0b0000000000001111
#define FTM_CH1FVAL              0b0000000011110000
#define FTM_CSC_CHIE_MASK        0b0000000001000000
#define FTM_CSC_MS_MASK          0b0000000000110000
#define FTM_CSC_ELS_MASK         0b0000000000001100
#define FTM_CSC_ELS_RAISING_EDGE 0b0000000000000100
#define FTM_CSC_DMA_MASK         0b0000000000000001



void encoder_setup();

int32_t needle_stepper_position();
int32_t latch_stepper_position();

void set_needle_stepper_position(long int base_value = 0);
void set_latch_stepper_position(long int base_value = 0);

void start_tracking_needle_stepper();
void start_tracking_latch_stepper();

void stop_tracking_needle_stepper();
void stop_tracking_latch_stepper();



#endif

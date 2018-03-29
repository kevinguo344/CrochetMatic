#include "Encoder.h"


static const int16_t Needle_Stepper_Initial_Value = 00;
static const int16_t Latch_Stepper_Initial_Value = 00;
static const unsigned int FTM1_MOD_VALUE = 0xFFFF;
static const unsigned int FTM2_MOD_VALUE = 0xFFFF;

static int32_t needle_stepper_base_value;
static int32_t latch_stepper_base_value;
static int16_t FTM1_previous_value;
static int16_t FTM2_previous_value;


void FTM1_encoder_setup()
{
  // enable the clock for FTM1
  SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
  FTM1_SC &= ~FTM_SC_CLKS_3; 
  // enable FTM1
  FTM1_MODE |= FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
  // set the counter to run in debug mode, not really necessary
  FTM1_CONF |= FTM_CONF_BDMMODE_3;
  // set the counter initial value
  FTM1_CNTIN = Needle_Stepper_Initial_Value;
  FTM1_CNT = Needle_Stepper_Initial_Value;
  // Set max value
  FTM1_MOD = FTM1_MOD_VALUE;
  // set the FTM for quadrature mode, default options
  FTM1_QDCTRL |=  FTM_QDCTRL_QUADEN_MASK | FTM_PHAFLTREN | FTM_PHBFLTREN | FTM_QDCTRL_QUADMODE_MASK;
  // start the FTM by selecting external clock
  FTM1_SC |= FTM_SC_CLKS_3; //0x03 -> Divide by 8
  FTM1_SC = (FTM1_SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(0);
  FTM1_FILTER = (FTM1_FILTER & (~FTM_CH0FVAL)) | 0b111;
  FTM1_FILTER = (FTM1_FILTER & (~FTM_CH1FVAL)) | (0b111 << 4);
  // configure the input pins
  PORTA_PCR12 = PORT_PCR_MUX(7);  // Teensy pin 3 Phase A is count
  PORTA_PCR13 = PORT_PCR_MUX(7);  // Teensy pin 4 Phase B is direction
  Serial.println(FTM_QDCTRL_QUADMODE_MASK,BIN);
  Serial.println(FTM1_QDCTRL,BIN);
}

void FTM2_encoder_setup()
{
  // enable the clock for FTM2
  SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;
  FTM2_SC &= ~FTM_SC_CLKS_3; 
  // enable FTM2
  FTM2_MODE |= FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
  // set the counter to run in debug mode, not really necessary
  FTM2_CONF |= FTM_CONF_BDMMODE_3;
  // set the counter initial value
  FTM2_CNTIN = Latch_Stepper_Initial_Value;
  FTM2_CNT = Latch_Stepper_Initial_Value;
  // Set max value
  FTM2_MOD = FTM2_MOD_VALUE; //0x4E1F;
  // set the FTM for quadrature mode, default options
  FTM2_QDCTRL |= FTM_QDCTRL_QUADMODE_MASK | FTM_QDCTRL_QUADEN_MASK | FTM_PHAFLTREN | FTM_PHBFLTREN;
  // start the FTM by selecting external clock
  FTM2_SC |= FTM_SC_CLKS_3; // 0x05 -> divide by 32
  FTM2_SC = (FTM2_SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(0);
  FTM2_FILTER = (FTM2_FILTER & (~FTM_CH0FVAL)) | 0b111;
  FTM2_FILTER = (FTM2_FILTER & (~FTM_CH1FVAL)) | (0b111 << 4);
  // configure the input pins
  PORTB_PCR18 = PORT_PCR_MUX(6);  // Teensy pin 29 Phase A is count
  PORTB_PCR19 = PORT_PCR_MUX(6);  // Teensy pin 30 Phase B is direction
}


void encoder_setup()
{ 
  FTM1_previous_value = 0;
  FTM1_encoder_setup();
  set_needle_stepper_position(0);
  
  FTM2_previous_value = 0;
  FTM2_encoder_setup();
  set_latch_stepper_position(0);
}

int32_t needle_stepper_position()
{
  //long int result FTM1_CNT;
  uint16_t value = FTM1_CNT;
  int quad_control = FTM1_QDCTRL;
  int status_and_control = FTM1_SC;
  FTM1_SC &= ~FTM_SC_TOF;
  while (status_and_control & FTM_SC_TOF)
  {
    value = FTM1_CNT;
    if (quad_control & FTM_QDCTRL_TOFDIR_MASK)
      {
        needle_stepper_base_value += (FTM1_MOD_VALUE + 1);
      }
      else
      {
        needle_stepper_base_value -= (FTM1_MOD_VALUE + 1);
      }
    quad_control = FTM1_QDCTRL;
    status_and_control = FTM1_SC;
    FTM1_SC &= ~FTM_SC_TOF;
  }
  return((int32_t)(value) + needle_stepper_base_value);
}


void set_needle_stepper_position(long int base_value)
{
  FTM1_SC &= ~FTM_SC_CLKS_3;
  needle_stepper_base_value = base_value;
  FTM1_CNT = 0;
  FTM1_SC &= ~FTM_SC_TOF;
  FTM1_SC |= FTM_SC_CLKS_3;
}
 
void start_tracking_needle_stepper()
{
  FTM1_SC |= FTM_SC_CLKS_3;
}
 
void stop_measuring_material()
{
  FTM1_SC &= ~FTM_SC_CLKS_3;
}

int32_t latch_stepper_position()
{
  //long int result FTM2_CNT;
  int status_and_control = FTM2_SC;
  uint16_t value = FTM2_CNT;
  int quad_control = FTM2_QDCTRL;

  while (status_and_control & FTM_SC_TOF)
  {
    if (quad_control & FTM_QDCTRL_TOFDIR_MASK)
      {
        latch_stepper_base_value += (FTM2_MOD_VALUE + 1);
      }
      else
      {
        latch_stepper_base_value -= (FTM2_MOD_VALUE + 1);
      }
    FTM2_SC &= ~FTM_SC_TOF;
    status_and_control = FTM2_SC;
    value = FTM2_CNT;
    quad_control = FTM2_QDCTRL;
  }
  return((int32_t)(value + latch_stepper_base_value));
}

void set_latch_stepper_position(long int base_value)
{
  FTM2_SC &= ~FTM_SC_CLKS_3;
  latch_stepper_base_value = base_value - FTM2_CNT;
  FTM2_SC &= ~FTM_SC_TOF;
  FTM2_SC |= FTM_SC_CLKS_3;
}

void start_tracking_latch_stepper()
{
  FTM2_SC |= FTM_SC_CLKS_3;
}
 
void stop_tracking_latch_stepper()
{
  FTM2_SC &= ~FTM_SC_CLKS_3;
}



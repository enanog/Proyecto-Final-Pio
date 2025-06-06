/*
 * ----------------------
 * TSI.c (para el MKL25Z)
 * ----------------------
 * 
 * Materias:
 * Sistemas Electr�nicos Embebidos
 * 
 * 6to A�o T�cnico en Electr�nica
 * Colegio Pio IX
 * 
 * Revisi�n 1
 * A�o 2023
 * 
 * Profesores: Mat�as Marfisi, Tom�s Zuran, Federico Diograzia
 * 
 */

#include "TSI.h"

#define NO_TOUCH                 0
#define SLIDER_LENGTH           40 //LENGTH in mm
#define TOTAL_ELECTRODE          2

#define TSI0a        0
#define TSI1         1
#define TSI2         2
#define TSI3         3
#define TSI4         4
#define TSI5         5
#define TSI6         6
#define TSI7         7
#define TSI8         8
#define TSI9         9
#define TSI10        10
#define TSI11        11
#define TSI12        12
#define TSI13        13
#define TSI14        14
#define TSI15        15

/*Chose the correct TSI channel for the electrode number*/
#define ELECTRODE0   TSI9
#define ELECTRODE1   TSI10
#define ELECTRODE2   TSI0a
#define ELECTRODE3   TSI1
#define ELECTRODE4   TSI2
#define ELECTRODE5   TSI3
#define ELECTRODE6   TSI4
#define ELECTRODE7   TSI5
#define ELECTRODE8   TSI6
#define ELECTRODE9   TSI7
#define ELECTRODE10  TSI8
#define ELECTRODE11  TSI11
#define ELECTRODE12  TSI12
#define ELECTRODE13  TSI13
#define ELECTRODE14  TSI14
#define ELECTRODE15  TSI15

#define THRESHOLD0   100
#define THRESHOLD1   100
#define THRESHOLD2   100
#define THRESHOLD3   100
#define THRESHOLD4   100
#define THRESHOLD5   100
#define THRESHOLD6   100
#define THRESHOLD7   100
#define THRESHOLD8   100
#define THRESHOLD9   100
#define THRESHOLD10   100
#define THRESHOLD11   100
#define THRESHOLD12   100
#define THRESHOLD13   100
#define THRESHOLD14   100
#define THRESHOLD15   100

static uint8_t total_electrode = TOTAL_ELECTRODE;
static uint8_t elec_array[16] = { ELECTRODE0, ELECTRODE1, ELECTRODE2,
		ELECTRODE3, ELECTRODE4, ELECTRODE5, ELECTRODE6, ELECTRODE7, ELECTRODE8,
		ELECTRODE9, ELECTRODE10, ELECTRODE11, ELECTRODE12, ELECTRODE13,
		ELECTRODE14, ELECTRODE15 };
static uint16_t gu16TSICount[16];
static uint16_t gu16Baseline[16];
static uint16_t gu16Threshold[16] = { THRESHOLD0, THRESHOLD1, THRESHOLD2,
		THRESHOLD3, THRESHOLD4, THRESHOLD5, THRESHOLD6, THRESHOLD7, THRESHOLD8,
		THRESHOLD9, THRESHOLD10, THRESHOLD11, THRESHOLD12, THRESHOLD13,
		THRESHOLD14, THRESHOLD15 };
static uint16_t gu16Delta[16];
static uint8_t ongoing_elec;
static uint8_t end_flag = 1;

static uint8_t SliderPercentegePosition[2] = { NO_TOUCH, NO_TOUCH };
static uint8_t SliderDistancePosition[2] = { NO_TOUCH, NO_TOUCH };
static uint32_t AbsolutePercentegePosition = NO_TOUCH;
static uint32_t AbsoluteDistancePosition = NO_TOUCH;

void selfCalibration(void);

void TSI_Init() {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_TSI_MASK;

	TSI0_GENCS |= (TSI_GENCS_ESOR_MASK | TSI_GENCS_MODE(0)
			| TSI_GENCS_REFCHRG(4) | TSI_GENCS_DVOLT(0) | TSI_GENCS_EXTCHRG(7)
			| TSI_GENCS_PS(4) | TSI_GENCS_NSCN(11) | TSI_GENCS_TSIIEN_MASK
			| TSI_GENCS_STPE_MASK);

	TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;

	NVIC_EnableIRQ(TSI0_IRQn);

	selfCalibration();
}

void selfCalibration(void) {
	unsigned char cnt;
	unsigned char trigger_backup;

	TSI0_GENCS |= TSI_GENCS_EOSF_MASK;      // Clear End of Scan Flag
	TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;    // Disable TSI module

	if (TSI0_GENCS & TSI_GENCS_STM_MASK) // Back-up TSI Trigger mode from Application
		trigger_backup = 1;
	else
		trigger_backup = 0;

	TSI0_GENCS &= ~TSI_GENCS_STM_MASK;      // Use SW trigger
	TSI0_GENCS &= ~TSI_GENCS_TSIIEN_MASK;    // Enable TSI interrupts

	TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;     // Enable TSI module

	for (cnt = 0; cnt < total_electrode; cnt++) // Get Counts when Electrode not pressed
			{
		TSI0_DATA = ((elec_array[cnt] << TSI_DATA_TSICH_SHIFT));
		TSI0_DATA |= TSI_DATA_SWTS_MASK;
		while (!(TSI0_GENCS & TSI_GENCS_EOSF_MASK))
			;
		TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
		gu16Baseline[cnt] = (TSI0_DATA & TSI_DATA_TSICNT_MASK);
	}

	TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;    // Disable TSI module
	TSI0_GENCS |= TSI_GENCS_TSIIEN_MASK;     // Enale TSI interrupt
	if (trigger_backup)                      // Restore trigger mode
		TSI0_GENCS |= TSI_GENCS_STM_MASK;
	else
		TSI0_GENCS &= ~TSI_GENCS_STM_MASK;

	TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;     // Enable TSI module

	TSI0_DATA = ((elec_array[0] << TSI_DATA_TSICH_SHIFT));
	TSI0_DATA |= TSI_DATA_SWTS_MASK;
}

void sliderRead(void) {
	if (end_flag) {
		end_flag = 0;
		if ((gu16Delta[0] > gu16Threshold[0])
				|| (gu16Delta[1] > gu16Threshold[1])) {
			SliderPercentegePosition[0] = (gu16Delta[0] * 100)
					/ (gu16Delta[0] + gu16Delta[1]);
			SliderPercentegePosition[1] = (gu16Delta[1] * 100)
					/ (gu16Delta[0] + gu16Delta[1]);
			SliderDistancePosition[0] = (SliderPercentegePosition[0]
					* SLIDER_LENGTH) / 100;
			SliderDistancePosition[1] = (SliderPercentegePosition[1]
					* SLIDER_LENGTH) / 100;
			AbsolutePercentegePosition = ((100 - SliderPercentegePosition[0])
					+ SliderPercentegePosition[1]) / 2;
			AbsoluteDistancePosition = ((SLIDER_LENGTH
					- SliderDistancePosition[0]) + SliderDistancePosition[1])
					/ 2;
		} else {
			SliderPercentegePosition[0] = NO_TOUCH;
			SliderPercentegePosition[1] = NO_TOUCH;
			SliderDistancePosition[0] = NO_TOUCH;
			SliderDistancePosition[1] = NO_TOUCH;
			AbsolutePercentegePosition = NO_TOUCH;
			AbsoluteDistancePosition = NO_TOUCH;
		}
	}
}

uint8_t TSI_ReadDistance() {
	sliderRead();
	return AbsoluteDistancePosition;
}

void changeElectrode(void) {
	int16_t u16temp_delta;

	gu16TSICount[ongoing_elec] = (TSI0_DATA & TSI_DATA_TSICNT_MASK); // Save Counts for current electrode
	u16temp_delta = gu16TSICount[ongoing_elec] - gu16Baseline[ongoing_elec]; // Obtains Counts Delta from callibration reference
	if (u16temp_delta < 0)
		gu16Delta[ongoing_elec] = 0;
	else
		gu16Delta[ongoing_elec] = u16temp_delta;

	//Change Electrode to Scan
	if (total_electrode > 1) {
		if ((total_electrode - 1) > ongoing_elec)
			ongoing_elec++;
		else
			ongoing_elec = 0;

		TSI0_DATA = ((elec_array[ongoing_elec] << TSI_DATA_TSICH_SHIFT));
		TSI0_DATA |= TSI_DATA_SWTS_MASK;
	}
}

void TSI0_IRQHandler(void) {
	end_flag = 1;
	TSI0_GENCS |= TSI_GENCS_EOSF_MASK; // Clear End of Scan Flag
	changeElectrode();
}

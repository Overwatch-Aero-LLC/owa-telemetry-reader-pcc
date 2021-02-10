/*! \file
	\brief Timebase and decrementer interface unit for the Motorola MPC 5XX

	TimeBase provides functions to manage the PowerPC timebase, which provides
	a 64-bit counter that will run at 1Mhz with a 4Mhz clock input, or 1.25Mhz
	with a 20Mhz clock input.  The timebase clock rate is the external
	oscillator input divided by 4 or 16.  The division factor depends on the
	state of the MODCK bits at reset.  Since the timebase is 64 bits wide the
	counter value is stored in two separate registers, and the timebase module
	provides access to each register independently of the other.  Note that
	enabling the timebase also enables the decrementer, since they are driven
	by the same	clock.

	On platforms other than the MPC5XX this module emulates some of the
	timebase features.
*/

// Timebase.h
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco Street             //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _TB_H
#define _TB_H

#include "Types.h"

#ifndef EXCLUDE_MPC555_DETAILS
// Register definitions

// Decrementer is special-purpose-register 22 [MPC555 - 6.13.4.1], [MPC565 - 6.14.4.1]
// Time Base read is spr 268, 269 [MPC555 - 6.13.4.2], [MPC565 - 6.14.4.2]
// Time Base writeis spr 284, 285 [MPC555 - 6.13.4.2], [MPC565 - 6.14.4.2]

/*! Time base reference register 0 [MPC555 - 6.13.4.3], [MPC565 - 6.14.4.3] */
#define TBREF0 (*(VUInt32*)0x002FC204)

/*! Time base reference register 1 [MPC555 - 6.13.4.3], [MPC565 - 6.14.4.3] */
#define TBREF1 (*(VUInt32*)0x002FC208)

/*! Time base reference register 0 key [MPC555 - 8.9.3.2], [MPC565 - 8.8.3.2] */
#define TBREF0K (*(VUInt32*)0x002FC304)

/*! Time base reference register 1 key [MPC555 - 8.9.3.2], [MPC565 - 8.8.3.2]  */
#define TBREF1K (*(VUInt32*)0x002FC308)


/** @name Time base control and status register */
//@{
#define TBSCR (*(VUInt16*)0x002FC200) //!< Status and control register [MPC555 - 6.13.4.4], [MPC565 - 6.14.4.4]
#define TBIRQ 0xFF00	//!< Top 8 bits are interrupt priority
#define REFA  0x0080    //!< Set if TBREF0 matches lower timebase
#define REFB  0x0040	//!< Set if TBREF1 matches lower timebase
#define REFAE 0x0008	//!< Set to enable TBREF0 interrupts
#define REFBE 0x0004	//!< Set to enable TBREF1 interrupts
#define TBF   0x0002	//!< Set to stop timebase and decrementer during freeze
#define TBE   0x0001    //!< Set to enable timebase and decrementer
//@}

/*! Time base control and status register key [MPC555 - 8.9.3.2], [MPC565 - 8.8.3.2] */
#define TBSCRK (*(VUInt32*)0x002FC300)

/*! Set the value of the timebase reference A register.  User privilege
	\param x is the value to apply the timebas reference A register. */
#define SetTBRefA(x)	(TBREF0 = (x))

/*! Set the value of the timebase reference B register.  User privilege
	\param x is the value to apply the timebas reference B register. */
#define SetTBRefB(x)	(TBREF1 = (x))

/*! Get the value of the timebase reference A register.  User privilege
	\return The value in the timebas reference A register. */
#define GetTBRefA()		(TBREF0)

/*! Get the value of the timebase reference B register.  User privilege
	\return The value in the timebas reference B register. */
#define GetTBRefB()		(TBREF1)

/*! Returns the state of the timebase reference A interrupt status bit.
	User privilege.
	return Non-zero if the reference A interrupt bit is asserted, zero if not */
#define IsTBRefAAsserted()(TBSCR & REFA)


/*! Returns the state of the timebase reference B interrupt status bit.
	User privilege.
	return Non-zero if the reference B interrupt bit is asserted, zero if not */
#define IsTBRefBAsserted()(TBSCR & REFB)

#endif // EXCLUDE_MPC555_DETAILS

#ifdef __cplusplus
extern "C" {
#endif

//! Initialize the timebase
void InitTimeBase(UInt8 Level);

//! Legacy timebase initialization routine
void InitTimeBaseEx(UInt8 Level, UInt32 TimeBaseRate);

//! Update the timebase rate based upon information fed to the PLL.
void SetTimeBaseRateFromPLL(UInt32 Multiply, UInt32 OscInput);

//! Return the clock rate of the timebase in Hz.
UInt32 GetTimeBaseRate(void);

//! Return the clock rate of the timebase in floating point Hz.
double GetTimeBaseRateDouble(void);

//! Return the clock rate of the timebase in cycles per millisecond
UInt32 GetTimeBaseRate_ms(void);

//! Return the period of a single timebase tick in floating point seconds
double GetTimeBasePeriod(void);

//! Return the 64-bit timebase value
UInt64 GetTimeBase(void);

//! Return the 64-bit timebase value
UInt64 GetTimeBaseBoth(void);

//! Return the lower 32-bit timebase value
UInt32 GetTimeBaseLower(void);

//! Write the 64-bit timebase value
void WriteTimeBase(register UInt64 Time);

//! Write the 64-bit timebase value if you have super user privilege
void WriteTimeBase_su(register UInt64 Time);

//! Enable the timebase to count
void EnableTimeBase(void);

//! Diable the timebase from counting
void DisableTimeBase(void);

//! Clear the time base reference A interrupt status
void ClearTBRefA(void);

//! Clear the time base reference B interrupt status
void ClearTBRefB(void);

//! Clear the time base reference A and B interrupt status
void ClearTBRefBoth(void);

//! Enable the time base reference A interrupt
void EnableTBRefA(void);

//! Enable the time base reference B interrupt
void EnableTBRefB(void);

//! Enable the time base reference A and B interrupt
void EnableTBRefBoth(void);

//! Disable the time base reference A interrupt
void DisableTBRefA(void);

//! Disable the time base reference B interrupt
void DisableTBRefB(void);

//! Disable the time base reference A and B interrupt
void DisableTBRefBoth(void);

//! Read the decrementer value
UInt32 ReadDEC(void);

//! Read the decrementer value from with super user privilege
UInt32 ReadDEC_su(void);

//! Write the decrementer value
void WriteDEC(register UInt32 Time);

//! Write the decrementer value with super user privilege
void WriteDEC_su(register UInt32 Time);

//! Cause the decrementer interrupt to occur right now
void TriggerDEC(void);

//! Cause the decrementer interrupt to occur right now with super user privilege
void TriggerDEC_su(void);

#ifdef __cplusplus
}
#endif

#endif // !_TB_H

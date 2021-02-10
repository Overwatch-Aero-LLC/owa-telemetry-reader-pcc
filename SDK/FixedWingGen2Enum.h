/*! \file
	\brief Enumerations used by the fixed wing generation 2 controller

	This file contains a list of enumerations used by the FWG2 controller
	These enumerations are also visible in the Controller2_VerX.xml file.
	The enumerations may change with different versions of the FWG2 controller
*/
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco St.                //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _FIXED_WING_GEN_TWO_ENUM_H
#define _FIXED_WING_GEN_TWO_ENUM_H

#include "FixedWingCommonEnum.h"

/*! Number of loops supported by the FWG2 controller */
#define NUM_AP_LOOPS (AP_LOOP_VRATE+1)

#endif

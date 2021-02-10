/*! \file
    \brief Enumerations used by both fixed wing generation 2 and 3 controllers
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

#ifndef _FIXED_WING_ENUM_H
#define _FIXED_WING_ENUM_H

#include "PiccoloDefinitions.h"

/*! List of actuator types supported by the cotnroller */
enum ActuatorTypes
{
    NO_ACTUATION,               //!< No actuator, i.e. line available for other use
    LEFT_AILERON,               //!< Left aileron actuator [rad]
    RIGHT_AILERON,              //!< Right aileron actuator [rad]
    ELEVATOR,                   //!< Elevator (behind cg) actuator [rad]
    CANARD,                     //!< Canard (ahead of cg) actuator [rad]
    RUDDER,                     //!< Rudder actuator [rad]
    LEFT_FLAP,                  //!< Left flap actuator [rad]
    RIGHT_FLAP,                 //!< Right flap actuator [rad]
    LEFT_THROTTLE,              //!< Throttle actuator [0-1]
    BRAKES,                     //!< Brakes actuator [0-1]
    NOSEGEAR,                   //!< Nose gear (ahead of main gear) actuator [rad]
    LEFT_ELEVON,                //!< Combination of left aileron and elevator [rad]
    RIGHT_ELEVON,               //!< Combination of right aileron and elevator [rad]
    LEFT_RUDDERVATOR,           //!< Left combination of rudder and elevator [rad]
    RIGHT_RUDDERVATOR,          //!< Right combination of rudder and elevator [rad]
    LEFT_INVERTED_RUDDERVATOR,  //!< Left combination of inverted rudder and elevator [rad]
    RIGHT_INVERTED_RUDDERVATOR, //!< Right combination of inverted rudder and elevator [rad]
    TAILWHEEL,                  //!< Tail wheel (behind of main gear) actuator [rad]
    LEFT_RUDDER,                //!< Left rudder, used for rudder differentail
    RIGHT_RUDDER,               //!< Right rudder, used for rudder differentail
    LEFT_BRAKE,                 //!< Brakes actuator for left wheel [0-1]
    RIGHT_BRAKE,                //!< Brakes actuator for right wheel [0-1]
    LEFT_CANARDERON,            //!< Combination of left aileron and canard [rad]
    RIGHT_CANARDERON,           //!< Combination of right aileron and canard [rad]
    RIGHT_THROTTLE,             //!< Throttle actuator [0-1]
    LEFT_SPEED_BRAKE,           //!< Speed break on the left side [rad]
    RIGHT_SPEED_BRAKE,          //!< Speed break on the right side [rad]
    LEFT_BRAKERON_0,            //!< Customer proprietary [rad]
    LEFT_BRAKERON_1,            //!< Customer proprietary [rad]
    LEFT_BRAKERON_2,            //!< Customer proprietary [rad]
    LEFT_BRAKERON_3,            //!< Customer proprietary [rad]
    RIGHT_BRAKERON_3,           //!< Customer proprietary [rad]
    RIGHT_BRAKERON_2,           //!< Customer proprietary [rad]
    RIGHT_BRAKERON_1,           //!< Customer proprietary [rad]
    RIGHT_BRAKERON_0,           //!< Customer proprietary [rad]
    LEFT_OUTBOARD_ELEVON,       //!< Left elevon plus brake mixing, surface up for more brake [rad]
    LEFT_INBOARD_ELEVON,        //!< Left elevon plus brake mixing, surface down for more brake [rad]
    RIGHT_INBOARD_ELEVON,       //!< Right elevon plus brake mixing, surface down for more brake [rad]
    RIGHT_OUTBOARD_ELEVON,      //!< Right elevon plus brake mixing, surface up for more brake [rad]
    NUM_ACTUATOR_TYPES
};


/*! List of controller categories */
enum ControllerCategories
{
    LATGAINS_CATEGORY,          //!< Category of lateral gains
    LONGAINS_CATEGORY,          //!< Category of longitudinal gains
    TRIMS_CATEGORY,             //!< Category of trims
    LIMITS_CATEGORY,            //!< Category of limits
    VEHICLE_PARAMS_CATEGORY,    //!< Category of vehicle parameters
    MIXING_CATEGORY,            //!< Category of actuator mixing
    LANDING_PARAMS_CATEGORY,    //!< Category of landing paramters
    LAUNCH_PARAMS_CATEGORY,     //!< Category of launch parameters
    SENSOR_NAV_CATEGORY,        //!< Category of sensor relative navigation
    LANDING_LATGAINS_CATEGORY,  //!< Category of lateral gains used for landing
    LANDING_LONGAINS_CATEGORY,  //!< Category of longitudinal gains used for landing
    LANDING_LIMITS_CATEGORY,    //!< Category of limits used for landing
    COMMANDS_CATEGORY,          //!< Commands category
    NUM_CATEGORIES
};


/*! Autopilot loop indices */
enum APLoopIndices
{
    AP_LOOP_IAS,    //!< Indicated air speed
    AP_LOOP_ALT,    //!< Altitude
    AP_LOOP_BANK,   //!< Bank angle
    AP_LOOP_FLAP,   //!< Flaps
    AP_LOOP_HEADING,//!< Ground track heading
    AP_LOOP_VRATE,  //!< Vertical rate
    AP_LOOP_PITCH   //!< Pitch angle loop
};


/*! Autopilot mode states */
enum APStates
{
    AP_PRE_LAUNCH_STATE,        //!< Waiting for launch
    AP_TRANSITION_STATE,        //!< Launch in progress
    AP_LIFTOFF_STATE,           //!< Rotating for liftoff
    AP_CLIMBOUT_STATE,          //!< Airborne, climbing out
    AP_FLYING_STATE,            //!< Standard operating mode
    AP_LAND_PATTERN_STATE,      //!< In the landing pattern
    AP_DOWNWIND_LEG_STATE,      //!< On the downwind leg before base
    AP_BASE_LEG_STATE,          //!< On the base leg before final
    AP_FINAL_APPROACH_STATE,    //!< On the final approach
    AP_SHORT_FINAL_STATE,       //!< Almost to touchdown
    AP_TOUCHDOWN_STATE,         //!< Touchdown mode
    AP_ROLLOUT_STATE,           //!< Rolling down the runway
    AP_FINAL_TOUCHNGO_STATE,    //!< On the final approach, doing a touch and go
    AP_SHORT_TOUCHNGO_STATE,    //!< Almost to touchdown, doing a touch and go
    AP_TOUCHNGO_STATE,          //!< Touchdown mode, doing a touch and go
    NUM_AP_STATES               //!< Number of mode states
};


/*! List for different landing pattern types */
enum LandingTypes
{
    WHEELED_LANDING,    //!< Landing on wheels, requires high fidelity ground contact detection for rollout
    BELLY_LANDING,      //!< Landing on belly, ground contact detection not critical
    NET_LANDING,        //!< Landing in net, different guidance and go-around policy
    NUM_LANDING_TYPES
};


/*! List for different launch types */
enum LaunchTypes
{
    CATAPULT_LAUNCH,        //!< Catapult launch model
    ROLLING_TAKEOFF_LAUNCH, //!< Rolling takeoff launch model
    BALLOON_LAUNCH,         //!< Balloon launch model
    TUBE_LAUNCH,            //!< Tube launch model
    CARTOP_LAUNCH,          //!< Car top launch model
    VERTICAL_CATAPULT_LAUNCH,//!< Launched straight up
    HAND_ELECTRIC,          //!< Electric hand launch
    NUM_LAUNCH_TYPES
};


/** @name Fixed wing state aliases */
//@{
#define AP_TAKEOFF_ROLL_STATE AP_TRANSITION_STATE
#define AP_TOWED_STATE AP_TRANSITION_STATE
//@}


/*! Control output indices for the manual and auto pilot.  These are in addition
    to the standard outputs in PiccoloDefinitions.h */
enum FWGXControlOutputs
{
    CO_BRAKE = NUM_CONTROL_AXIS,    //!< Brakes
    CO_NOSEGEAR,                    //!< Nosegear
    CO_SPEED_BRAKE,                 //!< Speed brakes
    NUM_FWGX_CONTROL_AXIS
};

#endif

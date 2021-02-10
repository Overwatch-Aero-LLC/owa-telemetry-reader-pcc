/*! \file
    \brief Gimbal packet encode and decode functionality
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

#ifndef _GIMBAL_PACKET_GENERIC_H
#define _GIMBAL_PACKET_GENERIC_H

#include "Types.h"
#include "Indices.h"
#include "Definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_GIMBAL_PACKET_DATA_SIZE 253

#ifndef MAX_CUSTOM_STR_LEN
#define MAX_CUSTOM_STR_LEN 80
#endif

/*! Low level gimbal packet description */
typedef struct
{
    UInt8 Sync1;        //!< First synchronization character 0x00
    UInt8 Sync2;        //!< Second synchronization character 0xFF
    UInt8 Sync3;        //!< Third synchronization character 0x5A
    UInt8 Size;         //!< Size of the packet including frame, subframe, and data but not checksum
    UInt8 Group;        //!< Packet group
    UInt8 PktType;      //!< Packet type
    UInt8 Data[257];    //!< Data space including checksum space

    UInt32 i;           //!< Variable used by receiving state machine
    UInt32 State;       //!< Variable used by receiving state machine

}GimbalPkt_t;


/** @name States of the gimbal packet receive state machine */
//@{
enum
{
    GIMBAL_PKT_SYNC1_STATE,
    GIMBAL_PKT_SYNC2_STATE,
    GIMBAL_PKT_SYNC3_STATE,
    GIMBAL_PKT_SIZE_STATE,
    GIMBAL_PKT_GROUP_STATE,
    GIMBAL_PKT_PKTTYPE_STATE,
    GIMBAL_PKT_DATA_STATE,
    GIMBAL_PKT_CHECK1_STATE,
    GIMBAL_PKT_CHECK2_STATE
};
//@}


/** @name Gimbal group identifiers */
//@{
#define GIMBAL_COMMAND_AND_CONTROL_GROUP    0x00    //!< Group number for all gimbal command and control packets
#define GIMBAL_DYNAMICS_INFORMATION_GROUP   0x08    //!< Group number for all dynamics packets (rates, attitudes)
#define GIMBAL_POSITION_INFORMATION_GROUP   0x10    //!< Group number for all position packets (GPS, SPOI)
#define GIMBAL_STATUS_INFORMATION_GROUP     0x28    //!< Group number for status packets
#define GIMBAL_SETUP_GROUP                  0x48    //!< Group number for setup packets
#define GIMBAL_DEVELOPMENT_GROUP            0xE8    //!< Group number for development packets
#define INTER_PROCESSOR_GROUP               0xE0    //!< Group number for packets between processor in the gimbal
#define VGS_COMMAND_AND_CONTROL_GROUP       0xF0    //!< Group number for video ground station packets
#define VPS_PROGRAMMING_GROUP               0xFD    //!< Group number for VPS programming packets
#define VPS_GROUP_DEPRECATED                0xFE    //!< Group number for VPS work - deprecated in 2.2.3
#define VPS_DEVELOPMENT_GROUP               0xFF    //!< Group number for VPS development work (flash programming)
#define MICROBLAZE_DEVELOPMENT_GROUP        0xF7    //!< Group number for Microblaze development work
#define MICROBLAZE_PROGRAMMING_GROUP        0xF8    //!< Group number for Microblaze flash programming
#define MC_PROGRAMMING_AND_DEV_GROUP        0xF9    //!< Group number for Motion Control programming and dev packets
//@}


/** @name Command and control group packet types */
//@{

#define GYRO_ZERO_CMD_GIMBAL_PKTTYPE        0x40    //!< Packet type for gyro zero command
#define CAMERA_CMD_GIMBAL_PKTTYPE           0x41    //!< Packet type for camera control
#define CAMERA_PASS_THROUGH_GIMBAL_PKTTYPE  0x42    //!< Camera pass through packet type
#define RETRACT_DEPLOY_CMD_GIMBAL_PKTTYPE   0x43    //!< Command retract or deploy of 3AX
#define LASER_ARM_CMD_GIMBAL_PKTTYPE        0x44    //!< Command to arm a laser
#define LASER_FIRE_CMD_GIMBAL_PKTTYPE       0x45    //!< Command to fire a laser
#define SPOI_PATH_CMD_PKTTYPE               0x46    //!< Command SPOI path following
#define SPOI_PATH_NEAREST_CMD_PKTTYPE       0x47    //!< Command SPOI to nearest point on the current path
#define CAMERA_ZOOM_GIMBAL_PKTTYPE          0x48    //!< Continuous camera zoom command
#define LASER_SAFETY_STATUS_PKTTYPE         0x49    //!< Laser safety and safety status.
#define FFC_CMD_GIMBAL_PKTTYPE              0x4A    //!< Command to perform FFC

#define LASER_WATCHDOG_PKTTYPE        		0x4B    //!< Laser watchdog reminder to user.
#define LASER_PASSWORD_ENABLER_PKTTYPE      0x4C    //!< Overall Laser ops enabler - pswd is the S/N of the gimbal.

#define TRACKER_CMD_GIMBAL_PKTTYPE          0x50    //!< Command to control the video tracker
#define SYMBOLOGY_CMD_GIMBAL_PKTTYPE        0x51    //!< Command to control the video symbology
#define ENHANCEMENT_CMD_GIMBAL_PKTTYPE      0x52    //!< Command to control image enhancement (deprecated)
#define RADIO_SETUP_GIMBAL_PKTTYPE          0x60    //!< Command to control the radio setup
#define MOTOR_DRIVE_ENABLE_GIMBAL_PKTTYPE   0x70    //!< Command to enable/disable the motor drive
#define DO_MOTION_CALIBRATION	            0x71    //!< Command the gimbal to do a motio
#define GIMBAL_CMD_GIMBAL_PKTTYPE           0x80    //!< Packet type for basic gimbal commands
#define LEGACY_CAMERA_CMD_GIMBAL_PKTTYPE    0x81    //!< Legacy camera command packet type
#define GIMBAL_AGL_PKTTYPE					0x82	//!< Letting the Gimbal know what its Altitude AGL is.
#define GIMBAL_SCAN_CONTROL_PKTTYPE			0x83	//!< Command and configure gimbal scan mode.
#define RIG_TEST_CMD_GIMBAL_PKTTYPE         0xFE    //!< Rig test command
//@}


/** @name Dynamics information group packet types */
//@{
#define GIMBAL_ANGLE_GIMBAL_PKTTYPE         0x10    //!< Packet type for gimbal angle data
#define GIMBAL_RATE_GIMBAL_PKTTYPE          0x40    //!< Packet type for gimbal angle and mount rate data
#define HOST_SENSOR_GIMBAL_PKTTYPE          0x41    //!< Packet type for host supplied gimbal mount body rates
#define HOST_ATTITUDE_GIMBAL_PKTTYPE        0x42    //!< Packet type for host supplied gimbal mount attitude data
#define VIBRATION_DATA_GIMBAL_PKTTYPE       0x80    //!< Packet type for vibration data from gimbal
#define INITIAL_HEADING_GIMBAL_PKTTYPE      0x90    //!< Packet type for initializaing INS heading
#define TRACKING_DATA_GIMBAL_PKTTYPE        0xA0    //!< Packet type for tracking data to an autopilot
#define FRAME_SHIFTS_GIMBAL_PKTTYPE         0xB0    //!< Packet type for receiving image frame shifts from VPS or Tracker
//@}


/** @name Position information group packet types */
//@{
#define HOST_GPS_DATA_GIMBAL_PKTTYPE        0x10    //!< Packet type for host supplied GPS data
#define SPOI_CMD_GIMBAL_PKTTYPE             0x40    //!< Packet type for sensor point of interest command
#define GEOREFERENCE_DATA_GIMBAL_PKTTYPE    0x41    //!< Packet type for the geo referencing meta data, deprecated
#define TARGET_LOCATION_DATA_GIMBAL_PKTTYPE 0x42    //!< Target location (from gimbal)
#define TARGET_LOCATION_ESTIMATE_GIMBAL_PKTTYPE 0x43//!< Estimate of target location (to gimbal)
#define SPOI_PATH_POINT_DATA_PKTTYPE        0x45    //!< SPOI path points data
#define GIMBAL_TELEMETRY_GIMBAL_PKTTYPE     0x47    //!< Gimbal telemetry packet type
#define GIMBAL_TELEMETRY_2_GIMBAL_PKTTYPE   0x48    //!< Expanded gimbal telemetry packet as of 230
//@}


/** @name Status group packet types */
//@{
#define USER_WARNING_GIMBAL_PKTTYPE         0x00    //!< Packet type for sending a human readable warning message
#define RESET_REPORT_GIMBAL_PKTTYPE         0x01    //!< Packet type for sending a gimbal reset report
#define DIAGNOSTIC_GIMBAL_PKTTYPE           0x02    //!< Packet type for sending diagnostic information from the gimbal
#define MICROBLAZE_WARN_USER                0x10    //!< Packet type for sending a human readable warning message from the MB
#define MICROBLAZE_RESET_REPORT             0x11    //!< Packet type for sending a MB reset report 
#define MICROBLAZE_DIAGNOSTIC_PKTTYPE       0x12    //!< Packet type for sending a MB Diagnostics packet
#define VPS_USER_WARNING_PKTTYPE            0x20    //!< Packet type for sending a human readable warning message from the VPS
#define VPS_RESET_REPORT_PKTTYPE            0x21    //!< Packet type for sending a vps reset report  
#define VPS_DIAGNOSTIC_PKTTYPE              0x22    //!< Packet type for sending a VPS Diagnostics packet
#define MC_DIAGNOSTIC_PKTTYPE               0x32    //!< Packet type for sending a Motion Control Diagnostics packet
//@}


/** @name Setup group packet types */
//@{
#define SERIAL_INTERFACE_GIMBAL_PKTTYPE             0x00    //!< Packet type for changing serial interface setup
#define SENSOR_FILTER_GIMBAL_PKTTYPE                0x01    //!< Packet type for changing sensor filter settings
#define AXIS_SETTINGS_GIMBAL_PKTTYPE                0x02    //!< Packet type for changing axis settings
#define SYSTEM_VERSION_GIMBAL_PKTTYPE               0x03    //!< Packet type for system version information (deprecated in 230)
#define SYSTEM_VERSION_2_GIMBAL_PKTTYPE             0x04    //!< Packet type for newer system version information
#define CAMERA_DEFINITION_GIMBAL_PKTTYPE            0x05    //!< Packet type for camera definitions
#define GPS_LEVER_ARM_GIMBAL_PKTTYPE                0x06    //!< Packet type for GPS lever arm
#define CAMERA_ALIGN_GIMBAL_PKTTYPE                 0x07    //!< Packet type for camera alignment
#define LASERS_INSTALLED_GIMBAL_PKTTYPE             0x08    //!< Packet type for setup/query of installed lasers
#define TRACKER_ADV_CMD_GIMBAL_PKTTYPE              0x09    //!< Packet type for setup of tracker (advanced settings)
#define SPOI_PATH_PARAM_PKTTYPE                     0x0A    //!< Packet type for configuring the SPOI Path Navigator
#define EXTERNAL_STABILIZER_GIMBAL_PKTTYPE          0x0B    //!< Packet type for setup of external stabilizer
#define MAG_CAL_GIMBAL_PKTTYPE                      0x0C    //!< Packet type for magnetometer calibration data
#define FACTORY_AXIS_SETTINGS_GIMBAL_PKTTYPE        0x0D    //!< Packet type for changing factory axis settings
#define FACTORY_CAMERA_DEFINITION_GIMBAL_PKTTYPE    0x0E    //!< Packet type for factory camera definitions
#define FACTORY_CAMERA_ALIGN_GIMBAL_PKTTYPE         0x0F    //!< Packet type for factory camera alignment
#define FACTORY_LASERS_INSTALLED_GIMBAL_PKTTYPE     0x10    //!< Packet type for setup/query of factory installed lasers
#define SENSOR_CONFIGURATION_GIMBAL_PKTTYPE         0x11    //!< Packet type for sensor configuration data
#define KLV_DETAILS_GIMBAL_PKTTYPE                  0x12    //!< Packet type for configuration KLV data
#define FLIR_CONFIGURATION_GIMBAL_PKTTYPE           0x13    //!< Packet type for configuring the FLIR camera
#define FLIR_SAVE_CONFIGURATION_GIMBAL_PKTTYPE      0x14    //!< Packet type for saving FLIR camera configuration as startup settings
#define FLIR_DEFAULT_CONFIGURATION_GIMBAL_PKTTYPE   0x15    //!< Packet type for commanding FLIR camera to default configuration
#define LENS_PREFOCUS_CONFIGURATION                 0x16    //!< Packet type for setup/query of lens pre-focus data
#define FACTORY_EXTERNAL_STABILIZER_GIMBAL_PKTTYPE  0x17    //!< Packet for sending/querying factory stabilizer board settings
#define EXTERNAL_STABILIZER_VERSION_GIMBAL_PKTTYPE  0x18    //!< Packet for stabilizer board version info
#define LENS_CONTROL_VERSION_PKTTYPE                0x19    //!< Packet type for the lens controller version information
#define CAN_PASSTHROUGH_GIMBAL_PKTTYPE              0x20    //!< Packet type for setup/query of CANbus passthrough IDs
#define RETRACTOR_CONFIG_GIMBAL_PKTTYPE             0x21    //!< Packet type for setup/query of retractor configuration
#define IRCAM_CONFIGURATION_GIMBAL_PKTTYPE          0x22    //!< Packet type for configuring a T500 IR camera
#define IRCAM_SAVE_RESTORE_GIMBAL_PKTTYPE           0x23    //!< Packet type for saving/restoring the T500 IR startup configuration
#define FACTORY_TRACKER_ADV_CMD_GIMBAL_PKTTYPE      0x24    //!< Packet type for factory advanced tracker settings.
#define SUCSX_CONFIGURATION_GIMBAL_PKTTYPE          0x25    //!< Packet type for configuring a SU CSX Camera
#define SUCSX_SAVE_RESTORE_GIMBAL_PKTTYPE           0x26    //!< Packet type for saving/restoring the SU CSX camera startup configuration
#define LENS_CAL_ZOOM_CONFIGURATION                 0x27    //!< Packet type for setup/query of lens cal table zoom positions
#define LENS_CAL_FOCUS_CONFIGURATION                0x28    //!< Packet type for setup/query of lens cal table temperature and focus positions
#define VPS3_VERSION_PKTTYPE                        0x40    //!< Packet type for the VPS3 version information
#define HD_CONTROL_SETTINGS_PKTTYPE                 0x41    //!< Packet type for the HD control modes of the VPS
#define OVERLAY_SETTINGS_PKTTYPE                    0x42    //!< Packet type for the overlay settings
#define PICTURE_IN_PICTURE_SETTINGS_PKTTYPE         0x43    //!< Packet type for the picture in picture settings
#define ELECTRONIC_STAB_SETTINGS_PKTTYPE            0x44    //!< Packet type for the electronic stabilization settings
#define FACTORY_HD_CONTROL_SETTINGS_PKTTYPE         0x4A    //!< Packet type for the factory HD control modes of the VPS
#define FACTORY_OVERLAY_SETTINGS_PKTTYPE            0x4B    //!< Packet type for the factory overlay settings
#define FACTORY_PICTURE_IN_PICTURE_SETTINGS_PKTTYPE 0x4C    //!< Packet type for the factory picture in picture settings
#define FACTORY_ELECTRONIC_STAB_SETTINGS_PKTTYPE    0x4D    //!< Packet type for the factory electronic stabilization settings
#define CAMERA_SAVE_CONFIGURATION_GIMBAL_PKTTYPE    0x50    //!< Packet type for saving camera configuration as startup settings
#define CAMERA_DEFAULT_CONFIGURATION_GIMBAL_PKTTYPE 0x51    //!< Packet type for commanding a camera to its default configuration
#define MISB_SETTINGS_PKTTYPE                       0x52    //!< Packet type for the MISB settings
#define FACTORY_MISB_SETTINGS_PKTTYPE               0x53    //!< Packet type for the factory MISB settings
#define FPGA_BIT_VERSION_PKTTYPE                    0x56    //!< Packet type for the FPGA bit file version information
#define MICROBLAZE_APP_VERSION_PKTTYPE              0x57    //!< Packet type for the MB application version information
#define VIDEO_ENHANCEMENT_SETTINGS_PKTTYPE          0x58    //!< Packet type for the video enhancement settings
#define FACTORY_VIDEO_ENHANCEMENT_SETTINGS_PKTTYPE  0x59    //!< Packet type for the factory video enhancement settings
#define GIMBAL_WATERMARK_DATA_PKTTYPE               0x5A    //!< Packet type for the vps watermark data
#define MOTION_CONTROLLER_BOOTLOADER_VERSION        0xA0    //!< Packet type for motion controller bootloader version information
#define MOTION_CONTROLLER_VERSION                   0xA1    //!< Packet type for motion controller version information
#define MOTION_CONTROL_PARAMS	                    0xA2    //!< Packet type for motion control settings
#define MOTION_CONTROL_PARAMS_FACTORY	            0xA3    //!< Packet type for motion control factory settings
#define MOTION_CALIBRATION_PARAMS	                0xA4    //!< Packet type for motion control calibration settings
#define MOTION_CALIBRATION_PARAMS_FACTORY	        0xA5    //!< Packet type for motion control factory calibration settings
#define AXIS_PARAMS	                                0xA6    //!< Packet type for motion control axes settings
#define AXIS_PARAMS_FACTORY	                        0xA7    //!< Packet type for motion control factory axes settings
#define AXIS_LIMITS	                                0xA8    //!< Packet type for motion control limit settings
#define AXIS_LIMITS_FACTORY	                        0xA9    //!< Packet type for motion control factory limit settings
#define SENSOR_ACQ_PARAMS	                        0xAA    //!< Packet type for motion control acq settings
#define SENSOR_ACQ_PARAMS_FACTORY	                0xAB    //!< Packet type for motion control acq factory settings

//@}


/** @name Inter-Processor Group */
//@{
#define MICROBLAZE_BUILT_IN_TEST_PKTTYPE        0x01    //!< Packet type for starting built-in tests
#define MICROBLAZE_TO_VPS_DIGITAL_ZOOM_PKTTYPE  0x02    //!< Packet type for commanding digital zoom
#define MICROBLAZE_TO_VPS_STROBE_PKTTYPE        0x04    //!< Packet type for providing data to VPS
#define VPS_TO_MICROBLAZE_STROBE_PKTTYPE        0x05    //!< Packet type for getting data from VPS
#define CAMERA_FFC_COMMAND_PKTTYPE              0x06    //!< Packet type for commanding flat-field correction
#define LASER_STATUS_MB_TO_565_PKTTYPE          0x07    //!< Packet type for reporting laser status
#define LASER_KEEP_ALIVE_565_TO_MB_PKTTYPE      0x08    //!< Packet type for laser heartbeat message
#define LASER_ARM_565_TO_MB_PKTTYPE	    	    0x09	//!< Packet type for laser arming
#define LASER_FIRE_565_TO_MB_PKTTYPE		    0x0A	//!< Packet type for laser firing
#define LASER_RF_DATA_MB_TO_565				    0x0B	//!< Packet type for laser range fider data
#define MOTION_CONTROL_MODE	                    0xA1    //!< Packet type for setting/reporting motion control mode
#define SET_AXIS_POS_RATE	                    0xA2    //!< Packet type for commanding motion control position or rate
#define AXIS_STATUS	                            0xA3    //!< Packet type for reporting motion control axes status
#define MC_BIST_STATUS                          0xA4    //!< Packet type for reporting BIST status.
//@}


/** @name Video ground station group packet types */
//@{
#define SET_ANTENNA_YAW_VGS_PKTTYPE         0x00    //!< Packet type for changing the antenna yaw offset
#define DIRECT_CONTROL_VGS_PKTTYPE          0x01    //!< Packet type for direct control of VGS
#define RADIO_SETUP_VGS_PKTTYPE             0x02    //!< Packet type for changing radio setup of VGS
#define TELEMETRY_VGS_PKTTYPE               0x03    //!< Packet type for video grund station telemetry
//@}


/** @name VPS Programming group packet types */
//@{
#define VPS_DSP_LICENSE_KEY_PKTTYPE     0x4D
//@}


/** @name Development group packet types */
//@{
#define ENABLE_NAV_DEV_PKTTYPE          0x01    //!< Packet type for enabling or disabling navigation data
#define STAB_TELEM_DEV_PKTTYPE          0x10    //!< Packet type for development telemetry for stabilization analysis
#define STAB_T2_SET_DEV_PKTTYPE         0x18    //!< Packet type for commanding changes to stabilization settings
#define CURRENT_DEV_PKTTYPE             0x20    //!< Packet type for motor current data output
#define STABILITY_DEV_PKTTYPE           0x30    //!< Packet type for sending FOG board stability data
#define VIDEO_TRK_DEV_PKTTYPE           0x40    //!< Packet type for sending out video track diagnostic data
//@}


/** @name VPS Development group packet types */
//@{
#define VPS_PROGRAM_IN_PKTTYPE          0x00    //!< Packet type for sending programming data from host through TASE and into VPS
#define VPS_PROGRAM_OUT_PKTTYPE         0x01    //!< Packet type for sending programming data from VPS through TASE and into host
//@}


/** @name Microblaze Development Group */
//@{
#define MB_LENS_REGISTER_PKTTYPE            0x00    //!< Command to read/write a register for a lens controlled through the Microblaze.
#define MB_LENS_COMMAND_PKTTYPE             0x01    //!< Command the lens position or a re-initialization.
#define MB_ENABLE_LENS_DIAGNOSTICS          0x02    //!< Used to command the Microblaze to enable diagnostic outputs for a specifed lens.
#define MB_LENS_DIAGNOSTIC_PKTTYPE          0x03    //!< Report of Microblaze lens diagnostic data (when enabled).       
//@}


/** @name Microblaze Programming Group */
//@{
#define MICROBLAZE_FLASH_DOWNLOAD_START     0x40    //!< Inform the MB FFB that image packets are forthcoming. If received in the application sends to FFB.
#define MICROBLAZE_FLASH_DOWNLOAD_NO_ACK    0x41    //!< Identify an image data packet that does not require an ack.
#define MICROBLAZE_FLASH_DOWNLOAD_AND_ACK   0x42    //!< Identify an image data packet that expects an ack.
#define MICROBLAZE_FLASH_RESPONSE           0x43    //!< Identify a response packet from the FFB. This packet includes a null terminated data message.
#define MICROBLAZE_FLASH_DOWNLOAD_END       0x44    //!< Inform the MB FFB that the entire image has been sent.
#define MICROBLAZE_FLASH_ERASE              0x45    //!< Command the MB to begin the erase process.
#define MICROBLAZE_FLASH_STROBE             0x46    //!< Identify an FFB strobe packet.  This packet includes FFB version info as a null terminated data message.
//@}


/** @name Motion Control Programming and Dev Group */
//@{
#define MC_DSP_FLASH_DOWNLOAD_START     0x40    //!< Inform the MC FFB that image packets are forthcoming. If received in the application goes to FFB.
#define MC_DSP_FLASH_DATA_NO_ACK        0x41    //!< Identify an image data packet that does not require an ack.
#define MC_DSP_FLASH_DATA_ACK           0x42    //!< Identify an image data packet that expects an ack.
#define MC_DSP_FLASH_RESPONSE           0x43    //!< Identify a response packet from the FFB. This packet includes a null terminated data message.
#define MC_DSP_FLASH_DOWNLOAD_END       0x44    //!< Inform the MC FFB that the entire image has been sent.
#define MC_DSP_FLASH_ERASE              0x45    //!< Command the MC to begin the erase process.
#define MC_DSP_FLASH_STROBE             0x46    //!< Identify an FFB strobe packet.  This packet includes FFB version info as a null terminated data message.
#define MC_BIQUAD_PARAMS                0x50    //!< Development/test packet
#define MC_DEBUG_PARAMS                 0x51    //!< Development/test packet
//@}


/** @name Gimbal mode enumeration */
//@{
enum GimbalModes
{
    GIMBAL_VELOCITY_FEEDBACK,       //!< Holding a fixed gimbal rate
    GIMBAL_POSITION_FEEDBACK,       //!< Holding a fixed gimbal angle
    GIMBAL_SPOI_FEEDBACK,           //!< Holding a fixed ground location
    GIMBAL_VIDEO_ST_RATE_FEEDBACK,  //!< Holding a fixed video scene with rate mode
    GIMBAL_VIDEO_TT_RATE_FEEDBACK,  //!< Holding a fixed video target with rate mode
    GIMBAL_VIDEO_ST_SPOI_FEEDBACK,  //!< Holding a fixed video scene with SPOI mode
    GIMBAL_VIDEO_TT_SPOI_FEEDBACK,  //!< Holding a fixed video target with SPOI mode
    GIMBAL_SPOI_PATH_MODE,          //!< The SPOI is following a fixed path in the inertial space
    GIMBAL_SCAN_ELLIPSE_MODE,       //!< Holding a rate around an elliptical path.
    GIMBAL_RIG_TEST_MODE,           //!< Performing rig test
    GIMBAL_PAN_INIT_MODE,           //!< Driving pan axis to find pan sync
    GIMBAL_TILT_MAX_INIT_MODE,      //!< Driving tilt axis to find min limit
    GIMBAL_TILT_MIN_INIT_MODE,      //!< Driving tilt axis to find max limit
    GIMBAL_POS_INIT_MODE,           //!< Driving pan and tilt to the initial position
    GIMBAL_DETECT_RETRACT_MODE,     //!< Looking for the retract at powerup
    GIMBAL_WAIT_FOR_DEPLOY_MODE,    //!< Waiting to move until retractor says OK

    NUM_GIMBAL_MODES
};
//@}


//! Discrete SPOI stepping modes
enum
{
    STEP_SPOI_OFF,
    STEP_SPOI_BY_DIST,                  //!< Discrete SPOI steps at fixed distance intervals
    STEP_SPOI_BY_POINTS,                //!< Discrete SPOI steps at the path points
    STEP_SPOI_BY_TIME,                  //!< Discrete SPOI steps at fixed time intervals
    STEP_RESERVED0,                     //!< Not yet implemented for BY_TIME_WITH_POINTS
    STEP_SPOI_JOYSTICK,                 //!< Joystick controlled stepping
    STEP_SPOI_AUTO_SCAN,                //!< Stepping by specified rate scaled by the FOV
    NUM_SPOI_STEP_MODES
};


/** @name Camera protocol enumeration */
//@{
enum CameraProtocolTypes
{
    CAMERA_TYPE_NO_CAMERA,          //!< No camera installed
    CAMERA_TYPE_DUMB_CAMERA,        //!< A camera with no protocol to talk to
    CAMERA_TYPE_SONY = 10,          //!< Block cameras from Sony that use the VISCA protocol
    CAMERA_TYPE_SONY_XCL = 15,      //!< Camera Link camera from Sony XCL-C130C Narrow Angle
    CAMERA_TYPE_PHOTON = 20,        //!< LWIR camera from FLIR that uses photon protocol
    CAMERA_TYPE_TAU,                //!< LWIR camera from FLIR that uses tau protocol (similar to photon)
    CAMERA_TYPE_HRC,                //!< MWIR camera from FLIR that similar to photon/tau protocol
    CAMERA_TYPE_SWIRTAU = 25,       //!< SWIR camera from FLIR that uses a totally different protocol (@#&!)
    CAMERA_TYPE_THERMO = 30,        //!< LWIR camera from thermoteknix
    CAMERA_TYPE_PROSILICA = 40,     //!< Prosilica camera
    CAMERA_TYPE_320_KTX = 50,       //!< Sensors unlimited SWIR camera
    CAMERA_TYPE_640_KTX,            //!< Sensors unlimited SWIR camera, 640
	CAMERA_TYPE_SU_JSX,		        //!< Sensors unlimited SWIR camera, High Resolution, Mil-Rugged, InGaAs
	CAMERA_TYPE_IRCAM,              //!< MWIR Camera with AgileCore Controller from IR CAMERA
    CAMERA_TYPE_640_CSX,            //!< Sensors unlimited SWIR camera, High Sensitivity, Mil-Rugged, Small SWap, InGaAs
    CAMERA_TYPE_VPS_MIN = 100,      //!< First index of camera controlled by the VPS
    CAMERA_TYPE_VPS_FLEA3C,         //!< Point Grey or Baumer GigE color cam.
    CAMERA_TYPE_VPS_BLACKFLY_345,   //!< Point Grey BlackFly GigE color cam 3.45 um pixel size
    CAMERA_TYPE_VPS_BLACKFLY_220,   //!< Point Grey BlackFly GigE color cam 2.20 um pixel size
    CAMERA_TYPE_VPS_MAX
};
//@}


/** @name Lens type enumeration */
//@{
enum LensTypes
{
    LENS_TYPE_NO_LENS,          //!< No lens
    LENS_TYPE_GENERIC_FIXED,    //!< Generic fixed focal length lens
    LENS_TYPE_GENERIC_CZOOM,    //!< Generic continuous zoom
    LENS_TYPE_LENS_DOUBLER,     //!< Lens doubler optic
    LENS_TYPE_CAMERA_SPECIFIED, //!< Lens integrated with camera (a al Sony)
    LENS_TYPE_CCT_MWIR_CZOOM,   //!< MWIR continuous zoom lens
    LENS_TYPE_CCT_LWIR_DZOOM,   //!< LWIR discrete zoom lens
    LENS_TYPE_CCT_SWIR_CAT,     //!< SWIR catadioptric lens
    LENS_TYPE_SPOTTER,          //!< 3rd bay spotter lens (LRS)
    LENS_TYPE_SUPER_SPOTTER,        //!< super spotter lens (DXR)
    LENS_TYPE_CCT_HD_MWIR_CZOOM,    //!< MWIR lens
    LENS_TYPE_CCT_HD_VIS_SWIR_CZOOM,//!< SWIR/NFOV VIS dual aperture lens
    LENS_TYPE_CCT_SWIR_FIXED    //!< SWIR fixed lens
};
//@}

/** @name Type of sensor enumeration */
//@{
enum TypeOfSensor
{
    NO_SENSOR,          //!< No sensor on this channel/index
    SENSOR_EO,          //!< An E/O sensor (daylight)
    SENSOR_LWIR_WHITE,  //!< Long wave infrared in white-hot mode
    SENSOR_LWIR_BLACK,  //!< Long wave infrared in black-hot mode
    SENSOR_MWIR_WHITE,  //!< Mid wave infrared in white-hot mode
    SENSOR_MWIR_BLACK,  //!< Mid wave infrared in black-hot mode
    SENSOR_SWIR,        //!< Short wave infrared.
    SENSOR_EO_NO_ZOOM,  //!< An E/O w/o zoom
    SENSOR_EO_SPOTTER,  //!< An E/O spotter sensor
    NUM_SENSOR_TYPES    //!< Total number of sensor types
};
//@}

/** @name Video output type enumeration */
//@{
enum OutputTypes
{
    VIDEO_OUTPUT_NONE,      //!< No video output
    VIDEO_OUTPUT_NTSC,      //!< NTSC analog output
    VIDEO_OUTPUT_PAL,       //!< PAL analog output
    VIDEO_OUTPUT_HD_720I,   //!< 720i
    VIDEO_OUTPUT_HD_720P,   //!< 720p
    VIDEO_OUTPUT_HD_1080I,  //!< 1080i
    VIDEO_OUTPUT_HD_1080P,  //!< 1080p
    VIDEO_OUTPUT_CUSTOM,    //!< Not a standard output type
    VIDEO_OUTPUT_NTSC_GIGE  //!< NTSC GigE output to VPS FPGA daughtercard
};
//@}

//! OLD - Camera definition structure
typedef struct
{
    UInt8 LegacyType;       //!< Legacy camera definition type, only for backwards compatibility
    UInt8 ProtocolType;     //!< Protocol type, specifies kind of camera
    UInt8 AutoCameraType;   //!< Automatic camera type (i.e. from Sony camera)
    UInt8 LensType;         //!< Lens type, specifies what kind of lens is installed
    UInt8 Invert : 1;       //!< Set if image needs to be inverted (non VPS controlled cameras)
    UInt8 DeInterlace : 1;  //!< Set if camera needs to be de-interlaced
    UInt8 NoFfcShutter : 1; //!< Set if FFC needs to move gimbal to cover lens
    UInt8 NoFOVMatch : 1;   //!< Set this bit to stop FOV matching for this camera (either master or slave)
    UInt8 OutputFormat : 4; //!< Output type enumeration
    UInt8 InvertHoriz : 1;  //!< Set if image needs to be inverted horizontally (VPS controlled cameras)
    UInt8 InvertVert : 1;   //!< Set if image needs to be inverted vertically (VPS controlled cameras)
    UInt8 Reserved2 : 2;    //!< Reserved space for expansion
    UInt8 DigitalZoomLimit: 4;//!< Maximum digital zoom allowed
    UInt16 Reserved3;       //!< Reserved space for expansion
    UInt16 HorFpaSize;      //!< Horizontal size of the focal plane in micro-meters
    UInt16 VerFpaSize;      //!< Vertical size of the focal plane in micro-meters
    UInt16 MinFocalLength;  //!< Minimum focal length of the lens in 10s of micro-meters
    UInt16 MaxFocalLength;  //!< Maximum focal length of the lens in 10s of micro-meters
}CameraDefinition_1_t;

//! Camera definition structure
typedef struct
{
    UInt8 LegacyType;       //!< Legacy camera definition type, only for backwards compatibility
    UInt8 ProtocolType;     //!< Protocol type, specifies kind of camera
    UInt8 AutoCameraType;   //!< Automatic camera type (i.e. from Sony camera)
    UInt8 LensType;         //!< Lens type, specifies what kind of lens is installed
    UInt8 Invert : 1;       //!< Set if image needs to be inverted (non VPS controlled cameras)
    UInt8 DeInterlace : 1;  //!< Set if camera needs to be de-interlaced
    UInt8 NoFfcShutter : 1; //!< Set if FFC needs to move gimbal to cover lens
    UInt8 NoFOVMatch : 1;   //!< Set this bit to stop FOV matching for this camera (either master or slave)
    UInt8 OutputFormat : 4; //!< Output type enumeration
    UInt8 InvertHoriz : 1;  //!< Set if image needs to be inverted horizontally (VPS controlled cameras)
    UInt8 InvertVert : 1;   //!< Set if image needs to be inverted vertically (VPS controlled cameras)
    UInt8 WhiteBalance: 1;  //!< Set if one push white balance needs to be executed
    UInt8 Reserved2 : 1;    //!< Reserved space for expansion
    UInt8 DigitalZoomLimit: 4;//!< Maximum digital zoom allowed
    UInt16 Reserved3;       //!< Reserved space for expansion
    UInt16 HorFpaSize;      //!< Horizontal size of the focal plane in micro-meters
    UInt16 VerFpaSize;      //!< Vertical size of the focal plane in micro-meters
    UInt16 MinFocalLength;  //!< Minimum focal length of the lens in 10s of micro-meters
    UInt16 MaxFocalLength;  //!< Maximum focal length of the lens in 10s of micro-meters
    float  GainRed;         //!< Gain applied to red channel for white balancing
    float  GainBlue;        //!< Gain applied to blue channel for white balancing
}CameraDefinition_t;

/** @name Sony Camera type enumeration */
//@{
enum SonyCameraTypes
{
    SONY_FCB_EX480B = 0x24, //!< 18x optical zoom, NTSC 0x24
    SONY_FCB_EX480BP,       //!< 18x optical zoom, PAL  0x25
    SONY_FCB_EX48B,         //!< 18x optical zoom, NTSC 0x26
    SONY_FCB_EX48BP,        //!< 18x optical zoom, PAL  0x27
    SONY_FCB_IX11 = 0x2E,   //!< 10x optical zoom, NTSC
    SONY_FCB_IX11P,         //!< 10x optical zoom, PAL
    SONY_FCB_EX980 = 0x30,  //!< 26x optical zoom, NTSC 0x30
    SONY_FCB_EX980P,        //!< 26x optical zoom, PAL  0x31
    SONY_FCB_EX980S,        //!< 26x optical zoom, Stabilized, NTSC 0x32
    SONY_FCB_EX980SP,       //!< 26x optical zoom, Stabilized, PAL 0x33
    SONY_FCB_EX480C,        //!< 18x optical zoom, NTSC  0x34
    SONY_FCB_EX480CP,       //!< 18x optical zoom, PAL  0x35
    SONY_FCB_EX45MC = 0x3A, //!< 18x optical zoom, NTSC 0x3A
    SONY_FCB_EX45MCC,       //!< 18x optical zoom, NTSC 0x3B
    SONY_FCB_EX1000 = 0x42, //!< 36x optical zoom, NTSC 0x42
    SONY_FCB_EX1000P,       //!< 36x optical zoom, PAL 0x43
    SONY_FCB_EX1010,        //!< 36x optical zoom, NTSC 0x44
    SONY_FCB_EX1010P,       //!< 36x optical zoom, PAL 0x45
    SONY_FCB_H10 = 0x4A,    //!< 10x optical zoom, high definition video selectable
    SONY_FCB_H11,           //!< 10x optical zoom, high definition video selectable
    SONY_FCB_EX1020 = 0x50, //!< 36x optical zoom, NTSC progressive 0x50
    SONY_FCB_EX1020P,       //!< 36x optical zoom, PAL progressive 0x51
    SONY_FCB_EH4300 = 0x5C, //!< 20x optical zoom, CMOS HD
    SONY_FCB_EH6300 = 0x5F, //!< 20x optical zoom, CMOS HD
	SONY_FCB_EH6500 = 0x66,	//!< 30x optical zoom, CMOS HD
	SONY_FCB_EV7500,		//!< 30x optical zoom, CMOS HD
	SONY_FCB_EV7100			//!< 10x optical zoom, CMOS HD
};
//@}


//! Types of lasers
enum LaserTypes
{
    NO_LASER,           //!< No laser installed
    LASER_POINTER,      //!< Laser is a pointer. BUT: call them illuminators as well.
    LASER_ILLUMINATOR,  //!< Laser is an illuminator
    LASER_RANGER,       //!< Laser is a range finder
    LASER_DESIGNATOR,   //!< Laser is a designator
    NUM_LASER_TYPES
};

//! Laser classifications
enum LaserClasses
{
	LASER_NA_CLASS,
	LASER_CLASS1,
	LASER_CLASS1M,
	LASER_CLASS2,
	LASER_CLASS2M,
	LASER_CLASS3R,
	LASER_CLASS3B,
	LASER_CLASS4,
	NUM_LASER_CLASS
};

//! Laser protocols
enum LaserProtocol
{
    LASER_TYPE_DUMB_LASER,          //!< A laser with no protocol to talk to or the default when no laser is installed.
    LASER_TYPE_FLIR_CLI_30 = 10     //!< FLIR CLI-30 laser communication protocol.
};

#define LASER_WD_TIMEOUT 300 						// 5 minutes watchdog timeout for laser == 300 seconds
#define LASER_REMINDER_TIMEOUT 270 					// 4:30 minutes reminder timeout for laser = 270 seconds.
#define LASER_TIMEOUT_INDEFINITE 0xFFFFFFFFFFFFFFF

//! Laser Safety Flags
enum LaserSafetyFlags
{
	LOST_COMMS_AP = 0,		//!< Lost comms between gimbal & autopilot
	LOST_COMMS_GND,			//!< Lost comms between autopilot & ground
	ON_GROUND,				//!< Aircraft on ground
	POS_ELEV_ANGLE,			//!< Positive elevation angle
	WATCHDOG_TIMER,			//!< Laser been firing continuously with no power change from user.
	MIN_AGL,				//!< Laser minimum AGL restriction not met (gimbal too low).
	GLOBAL_PSWD,			//!< Correct Laser global password not received at gimbal (gimbal serial number).
	AGL_INVALID,            //!< for some reason, we could not calculate the gimbal's AGL.
	NUM_LASER_SAFETY_FLAGS
};


//! Laser rangefinder status
enum LaserRangeFinderStatus
{
    LRF_STATUS_STANDBYE = 0,    //!< The laser range finder is in standby and not firing.
    LRF_STATUS_FIRING,          //!< The laser range finder is firing for range with no measurement errors.
    LRF_STATUS_BORE_SIGHTING,   //!< The laser range finder is in continuous output mode for bore sighting.
    LRF_STATUS_ERROR_ACCURACY,  //!< The laser range finder is firing for range with error code A: running out of distance accuracy.
    LRF_STATUS_ERROR_TEMP,      //!< The laser range finder is firing for range with error code T: running out of temperature spec.
    LRF_STATUS_ERROR_ACC_TEMP,  //!< The laser range finder is firing for range with error code C: running out of distance accuracy and temp spec.
    LRF_STATUS_ERROR_RANGE,     //!< The laser range finder is firing for range with error code R: a range error occurred and result is invalid.
    LRF_STATUS_ERROR_CHECKSUM   //!< The laser range finder is firing for range but all measurements failed their checksum.
};


typedef struct
{
    float Divergence;  	 //!< Beam divergence angle in radians
    float Power;       	 //!< Power in Watts
    float Wavelength;    //!< Wavelength in nano-meters
	UInt8 Class;		 //!< Laser classification, see LaserClasses enumeration
    UInt8 Type;          //!< Type of laser
    UInt16 MinSafetyAGL; //!< Lasers will not fire if AGL is less than this.
    UInt8 Protocol;      //!< Laser Communication Protocol.
}LaserSetup_t;


//! Laser status information
typedef struct
{
    UInt32 Laser3PowerLimited : 1;  //!< Set if gimbal is limiting laser power
    UInt32 Laser3Armed        : 1;  //!< Set if gimbal is laser is armed
    UInt32 Laser2PowerLimited : 1;  //!< Set if gimbal is limiting laser power
    UInt32 Laser2Armed        : 1;  //!< Set if gimbal is laser is armed
    UInt32 Laser1PowerLimited : 1;  //!< Set if gimbal is limiting laser power
    UInt32 Laser1Armed        : 1;  //!< Set if gimbal is laser is armed
    UInt32 Laser0PowerLimited : 1;  //!< Set if gimbal is limiting laser power
    UInt32 Laser0Armed        : 1;  //!< Set if gimbal is laser is armed
    UInt32 Laser3Power        : 7;  //!< Laser power from 0% to 100%
    UInt32 Laser2Power        : 7;  //!< Laser power from 0% to 100%
    UInt32 Laser1Power        : 7;  //!< Laser power from 0% to 100%
    UInt32 Laser0Power        : 7;  //!< Laser power from 0% to 100%
    UInt16 DisableFlags       : 8;  //!< Flags used to describe why the lasers are disabled.
    UInt8  BypassSafety       : 1;  //!< Flag to denote if the user is bypassing the safety checks. 
}LaserStatus_t;


/*! Gimbal serial interface data */
typedef struct
{
    UInt8 Protocol;     //!< Protocol, see enumeration
    UInt8 Baud;         //!< Baud rate in 1200 baud per bit
    UInt8 Angle;        //!< Update frequency of angle message
    UInt8 Rate;         //!< Update frequency of rate message
    UInt8 Georef;       //!< Update frequency of geo-ref message
    UInt8 Reserved;     //!< Set to zero
}GimbalSerial_t;


//! Types of communications protocols
enum GimbalProtocolTypes
{
    NO_GIMBAL_PROTOCOL, //!< No communications
    GIMBAL_COMM,        //!< Standard gimbal communications
    GIMBAL_KLV,         //!< KLV meta data output
    GIMBAL_VPS,         //!< Video processing communications
    GIMBAL_HMR_2300,    //!< Connection to Honeywell HMR2300
    GIMBAL_HMR_3400,    //!< Connection to Honeywell HMR3400
    GIMBAL_NOVATEL,     //!< Connection to Novatel GPS
    NUM_GIMBAL_PROTOCOLS
};


//! Gimbal sensor configuration bit flags
typedef struct
{
    UInt32 DisableuBloxSBAS : 1;//!< Set to disable SBAS mode in uBlox GPS
    UInt32 UseExtAttitude : 1;  //!< Set to allow consumption of external attitude data
    UInt32 UseExtHeading : 1;   //!< Set to allow consumption of external heading data
    UInt32 UseVPSHeading : 1;   //!< Set to allow VPS frame shifts as a heading observation
    UInt32 UseInternalMag : 1;  //!< Set to enable use of the internal magnetometer (deprecated in 223)
    UInt32 UseNovatelAlign : 1; //!< Set to enable use of Novatel Align (if present)
    UInt32 Reserved : 26;

}GimbalSensorFlags_t;


//! Types of aiding that can be applied to a gimbal INS.
enum GimbalINSAidingtypes
{
    NO_GIMBAL_INS_AID,              //!< No aiding, pure GPS/INS
    MAG_HEADING_GIMBAL_INS_AID,     //!< Magnetometer supplied heading as an aid to INS
    EXT_HEADING_GIMBAL_INS_AID,     //!< Externally supplied heading as an aid to INS
    VPS_HEADING_GIMBAL_INS_AID,     //!< VPS computed heading as an aid to INS
    EXT_ATTITUDE_GIMBAL_INS_AID,    //!< Externally supplied attitude as an aid to INS
    NOVATEL_ALIGN_INS_AID,          //!< Novatel Align suppplied heading as an aid to INS       
    NUM_GIMBAL_INS_AID
};

//! Flags that go along with gimbal telemetry data
typedef struct
{
    UInt32 FPATempIncluded : 1; //!< Set if FPA temp included in packet
    UInt32 LaserIncluded : 1;   //!< Set if laser data included in packet
    UInt32 RangeIncluded: 1;    //!< Set if range data included in packet
    UInt32 MagIncluded : 1;     //!< Set if magnetometer data included in packet
    UInt32 ExtHdgIncluded : 1;  //!< Set if external heading data included in packet
    UInt32 GimbalOnTimeIncluded : 1;   //!< Set if gimbal on time included in packet
    UInt32 PathTrackPosIncluded : 1;   //!< Set if path track position data included in packet
    UInt32 SceneIntensityIncluded:1;   //!< Set if scene intensity information included in packet
    UInt32 ZoomIncluded:1;      //!< Set if zoom ratio included in packet
    UInt32 PayloadTempAndHumidityIncluded:1;//!< Set if payload temperature and humidity are included
    UInt32 FocusModeIncluded: 1;//!< Set if focus mode is included
    UInt32 BaroIncluded : 1;    //!< Baro usage info is included
    UInt32 GPSExternal : 1;     //!< Set if GPS is external
    UInt32 INSAid : 3;          //!< Enumerated INS aiding information, see GimbalINSAidingTypes
	UInt32 BaroInUse : 1;		//!< Set if baro is used for autopilot guidance
	UInt32 Reserved : 15;       //!< Bits left for expansion
}GimbalTelemetryFlags_t;


//! Focus enumerations for GimbalTelemetryData_t.FocusMode
enum FocusMode_t
{
    AUTO_FOCUS,     //!< Automatic focus
    FOCUS_ONCE,     //!< Automatic focus and then stop auto-focus
    INFINITE_FOCUS, //!< Infinite focus
    MANUAL_FOCUS,   //!< manual focus
    NUM_FOCUS_MODE
};

//! Exposure mode enumerations
enum ExposureMode_t
{
    EXPOSURE_FULL_AUTO          = 0,     //!< Automatic exposure mode
    EXPOSURE_SHUTTER_PRIORITY   = 1,     //!< Shutter priority exposure mode
    EXPOSURE_BRIGHT             = 2,     //!< Bright exposure mode
    NUM_EXPOSURE_MODES,                  //!< Number of exposure modes
    EXPOSURE_NO_CHANGE          = 0xFF   //!< No change to exposure mode
};

//! Primary data set from the gimbal
typedef struct
{
    GimbalTelemetryFlags_t Flags;   //!< Data about the gimbal data
    UInt32 Time;                    //!< System time since reset in milliseconds
    UInt32 GimbalOnTime;            //!< Total gimbal on time in minutes.
    GPSData_t GPS;                  //!< Position and velocity and other GPS information
    float MagField[NAXIS];          //!< Magnetic field information in milli-Gause
    float ExtHeading;               //!< External heading observation (possibly from mag) -PI to PI
    float CameraAttitude[NAXIS];    //!< Attitude in inertial space of the camera
    float GimbalAttitude[NAXIS];    //!< Attitude of the gimbal mount in inertial space
    float GimbalAngles[NGIMBAL];    //!< Gimbal angles with respect to gimbal mount
    float HFOV;                     //!< Horizontal field of view in radians
    float VFOV;                     //!< Vertical field of view in radians
    float LaserRange;               //!< Range from laser range finder in meters, or -1 if no range
    float FPATemp;                  //!< Focal plane array temperature in degrees Celcius
    float BoardTemp;                //!< Main board temperature in degrees Celcius
    float PathTrackPos;             //!< Path track position in units of waypoint # and fraction to next waypoint
    float Zoom;                     //!< Zoom ratio (current focal length / min focal length)
	float Baro;						//!< Barometric altitude in meters
    SInt8 PayloadTemp;              //!< Temperature of the payload ball in degrees Celcius
    SInt8 PayloadHumidity;          //!< Humidity in the payload ball in %
    UInt16 FocusPos;                //!< Focus position
    UInt16 SceneIntensity;          //!< Mean scene intensity from FLIR HRC camera
    UInt8 Mode;                     //!< Operational mode of the gimbal
    UInt8 Camera;                   //!< Active camera enumerator
    UInt8 VideoStamp;               //!< The video stamp, 255 if no video stamp
    UInt8 NumCameras;               //!< The number of physical cameras in the system
    UInt8 CameraIsInterlaced;       //!< Non-zero if the the active camera has interlaced output
    UInt8 FocusMode;                //!< Focus mode
    NavStatus_t NavStatus;          //!< Navigation status information
    LaserStatus_t Laser;            //!< Laser status information
    float AlignHeading;             //!< Novatel Align heading (corrected for antenna baseline mount angles) in rad.
    float AlignHeadingSigma;        //!< Novatel Align heading standard deviation in rad.
    UInt8 AlignSolnType;            //!< Novatel Align solution type
    UInt8 AlignVisibleSats;         //!< Novatel Align number of visible satellites
    UInt8 AlignTrackedSats;         //!< Novatel Align number of tracked satellites
}GimbalTelemetryData_t;


//! Telemetry information from the video ground station
typedef struct
{
    UInt32 Time;                    //!< System time since reset in milliseconds
    GPSData_t GPS;                  //!< Position and velocity and other GPS information
    float Pan;                      //!< Pan position of the antenna pointer
    float Tilt;                     //!< Tilt position of the antenna pointer
    float PanCmd;                   //!< Pan position command of the antenna pointer
    float TiltCmd;                  //!< Tilt position command of the antenna pointer
    float RSSI;                     //!< Receive signal strength from radio

}VGSTelemetryData_t;


/*! KLV configuration data */
typedef struct
{
    char MissionID[40];                 //!< Mission ID to be included in KLV
    char PlatformDesignation[40];       //!< Platform designation to be included in KLV
    char SCI_SCH[40];                   //!< Sensitive Compartmented Information (SCI) / Special Handling Instructions (SHI)
    char Caveats[40];                   //!< Added information
    char ReleasingInstructions[40];     //!< Instructions on how to release
    char ObjectCountryCodes[40];        //!< Concatenated object countries
    char ClassifyingCountry[6];         //!< Actual classifying country
    UInt8 Classification;               //!< Classification: top secret(0x05), secret(0x04), confidential(0x03), restricted(0x02), unclassified(0x01)
    UInt8 ObjectCountryCodeMethod;      //!< Method for coding object countries
    UInt8 ClassifyingCountryCodeMethod; //!< Method for coding classifying country

}KLVDetails_t;


/*! FLIR specific camera configuration data. */
typedef struct
{
    UInt8 AGCType;          //!< Type of AGC algorithm: Plateau histogram, Once bright, Auto bright, Manual, reserved, Linear.
    UInt8 Constrast;        //!< Contrast value tou use in once bright, auto bright, or manual AGC algorithms.
    UInt16 Brightness;      //!< Brightness value to use in manual AGC algorithm, 0 to 16383
    SInt16 BrightBias;      //!< Brightness offset to use in once bright, auto bright, or manual AGC algorithm, -16384 to 16383
    UInt16 PlateauLevel;    //!< Plateau level for plateau histogram, 0 to 1000
    UInt16 AGCMaxGain;      //!< Maximum AGC gain in the plateau histogram algorithm
    UInt8 ITTMean;          //!< Mid point of the grey scale in plateau histogram or linear AGC algorithms
    UInt8 IntegrationTime;  //!< Integration time in milliseconds (selects NUC table)
    UInt8 ColorLUT;         //!< Selection of color look up table (White hot, Black hot, Fusion, Rainbow, Globow, Ironbow1, Ironbow2, Sepia, Color1, Color2, Ice and fire, Rain, OEM custom #1, Red hot, Green hot, 255 == don't change
    UInt8 AgcRoiPercent;    //!< Percent of the whole image to be taken into account when doing AGC, 1 to 100;
}FLIRConfig_t;


/*! IRCam flags. */
typedef struct
{
    UInt8 NRFState : 1;         //!< On/off state of NRF.
    UInt8 LAPState : 1;         //!< On/off state of LAP.
    UInt8 ResetAGLC : 1;        //!< Flag to reset the AGLC settings.
    UInt8 ResetLAP : 1;         //!< Flag to reset the LAP_DnCE settings.
    UInt8 Reserved : 4;         //!< Reserved.
}IRCamFlags_t;


/*! IRCam IR specific camera configuration data. (MWIR camera - Used by the T500) */
typedef struct
{
    IRCamFlags_t Flags;         //!< IRCam flags.
    UInt8   OperationMode;      //!< Operation mode for the camera: Hot, average, cold.
    UInt8   AGLCMode;           //!< Type of AGLC mode: Manual, Histogram, Linear.
    UInt8   AGLCRoi;            //!< The AGLC ROI enum: 0x0 - Full, 0x1 = 1/2, 0x2 = 1/3, 0x3 = 1/4.
    SInt16  Brightness;         //!< Brightness value for the AGLC mode.
    UInt16  Contrast;           //!< Contrast value for the AGLC mode.
    UInt16  Background;         //!< Background level for the LAP_DnCE.
    UInt16  Foreground;         //!< Foreground boost for the LAP_DnCE.
}IRCamConfig_t;


/*! SU 640 CSX flags. */
typedef struct
{
    UInt16 OpMode : 3;          //!< Operation mode for the camera: Hot, average, cold.
    UInt16 AgcMode : 3;         //!< AGC mode for the camera: Auto, Manual.
    UInt16 EnhanceMode : 3;     //!< Enhancement mode for the camera: Auto, Manual, Off.
    UInt16 CamMessage : 1;      //!< Flags if this packet includes a direct pass message for the camera.
    UInt16 Reserved : 6;        //!< Reserved.
}SuCsxCamFlags_t;


/*! SU 640 CSX specific camera configuration data */
typedef struct
{
    #define CSX_MSG_MAX_CHAR 32
    SuCsxCamFlags_t Flags;              //!< SU 640 CSX flags.
    UInt8           AgcCurVal;          //!< The current AGC value (Must be in low-high range).
    UInt8           AgcLow;             //!< The AGC lower limit operational setting. Range 0-43.
    UInt8           AgcHigh;            //!< The AGC upper limit operational setting. Range 0-43.
    UInt8           AgcStart;           //!< The start-up AGC value (Must be in low-high range).
    UInt16          AgcGain;            //!< Manual AGC Gain vales over range 1-511.
    UInt8           EnhancePower;       //!< Enhancement mode contrast value. Range 0-100 in 10s of value (100 == 10.0).
    UInt8           EnhanceAverage;     //!< Enhancement mode frame average value. Range 0-5.
    char            CamMessage[CSX_MSG_MAX_CHAR];     //!< Direct passthrough command to the camera, null terminated.
}SuCsxCamConfig_t;


/*! SonyXCL Narrow Angle specific camera configuration data. */
typedef struct
{
    UInt8	AGCType;          //!< Type of AGC algorithm: Off, Manual, Linear, Histogram
	UInt16  Contrast;         //!< Contrast value to use (sensor type is SBFP_SFB207)
	SInt16  Brightness;       //!< Brightness value to use in manual AGC algorithm, 0 to 16383
    UInt16  IntegrationTime;  //!< Integration time in milliseconds
}SonyXCLConfig_t;


/*! MISB configuration data. */
typedef struct
{
    UInt32  IpAddress;          //!< IP address for compressed video output. Coded whereas 0x01020304 is equivalent to 1.2.3.4.
	UInt16 	Port;               //!< UDP port for compressed video output. Values below 1024 are not allowed.
    float   BitRate;            //!< Bitrate of compressed video output in MB/s.
    UInt8   GOPSize;            //!< Group of picture size; defines the distance between two full images.
}MISBConfig_t;


/*! Gimbal scan control configuration data. */
typedef struct
{
    BOOL  Enabled;                         //!< Scan mode enable state.
    float CenterPosition[NGIMBAL_SHORT];   //!< Elliptical scan path center (deg).
    float AxisRadius[NGIMBAL_SHORT];       //!< Elliptical axis radii (deg).
    float RevPerSec;                       //!< Desired scan loop revolutions per second.
}GimbalScanControlConfig_t;


//! Flags that go along with gimbal diagnostic data
typedef struct
{
    UInt16 InputPowerIncluded : 1;      //!< Set if input power included
    UInt16 VPSCPUIncluded : 1;          //!< Set if VPS CPU load included
    UInt16 InternalVPSPowerIncluded: 1; //!< Set if internal VPS power included
    UInt16 PayloadDataIncluded : 1;     //!< Set if payload data included
    UInt16 MotorVoltageIncluded : 1;    //!< Set if pan and tilt motor average voltage included
    UInt16 MotorStatisticsIncluded : 1; //!< Set if pan and tilt motor power statistics
    UInt16 VibrationDataIncluded : 1;   //!< Set if vibration data included.
    UInt16 LaserDataIncluded : 1;       //!< Set if laser temperature data included.
    UInt16 CameraCaseTempIncluded : 1;  //!< Set if camera case temperature is included.
    UInt16 Reserved : 8;                //!< Bits left for expansion
}GimbalDiagnosticsFlags_t;


//! Gimbal diagnostic data
typedef struct
{
    GimbalDiagnosticsFlags_t Flags; //!< Flags regarding included data
    UInt16 LogicBoardFault : 1;     //!< Set if the logic board has a fault
    UInt16 LogicBoardCPU : 7;       //!< CPU usage in percent of the logic board
    UInt16 ReservedBit : 1;         //!< Reserved bit for alignment
    UInt16 VPSCPU : 7;              //!< CPU usage in percent of the video processor
    SInt8 LogicTemp;                //!< Logic board temperature in Celsius
    SInt8 PayloadTemp;              //!< Payload board temperature in Celsius
    SInt8 PayloadHumidity;          //!< Payload board humidity in percent
    SInt8 Reserved;         //!< Reserved field for alignment
    float InputV;           //!< Main input voltage in Volts
    float InputC;           //!< Main input current in Amps
    float LogicBoardV;      //!< Logic board voltage in Volts
    float LogicBoardC;      //!< Logic board current Amps
    float IntVPSBoardV;     //!< Internal VPS board voltage in Volts
    float IntVPSBoardC;     //!< Internal VPS board current in Amps
    float PayloadV;         //!< Payload voltage in Volts
    float PayloadC;         //!< Payload curreng in Amps
    float AvgPanMotorV;     //!< Average pan motor voltage in Volts
    float PanMotorVDev;     //!< Pan motor voltage standard deviation in Volts
    float AvgPanMotorC;     //!< Average pan motor current in Amps
    float PeakPanMotorC;    //!< Peak pan motor current in Amps
    float PanMotorCErr;     //!< RMS error in pan motor current
    float AvgTiltMotorV;    //!< Average tilt motor voltage in Volts
    float TiltMotorVDev;    //!< Tilt motor voltage standard deviation in Volts
    float AvgTiltMotorC;    //!< Average tilt motor current in Amps
    float PeakTiltMotorC;   //!< Peak tilt motor current in Amps
    float TiltMotorCErr;    //!< RMS error in tilt motor current
    float VibrationX;       //!< X acceleration vibration statistic
    float VibrationY;       //!< Y acceleration vibration statistic
    float VibrationZ;       //!< Z acceleration vibration statistic
    float VibrationP;       //!< Roll rate vibration statistic
    float VibrationQ;       //!< Pitch rate vibration statistic
    float VibrationR;       //!< Yaw rate vibration statistic
    UInt8 LaserPointerTemp; //!< Laser pointer temperature in degress Celcius.
    SInt8 CameraCaseTemp;   //!< Camera case temperature in degrees Celcius.
}GimbalDiagnostics_t;


//! T500 VPS diagnostic data
typedef struct
{
	SInt8 BoardTemp;        //!< Payload board temperature in degrees Celsius.
	SInt8 FpgaTemp;         //!< Payload FPGA temperature in degrees Celsius.
	UInt8 CpuLoad;          //!< VPS CPU load in percentage of max.
    UInt32 CpuFreq;         //!< VPS CPU frequency of the VPS in Hertz.     
    SInt8 CpuTemp;          //!< VPS CPU temperature in degrees Celsius. 
}Vps3Diagnostics_t;


//! T500 motion control diagnostic data
typedef struct
{
    float AvgVoltage[2];  	            //!< Average voltage by node (outer/inner)
    float AvgCur[NGIMBALINOUT];       	//!< Average current by axis (all nodes)
    float AvgStab[NGIMBAL_SHORT];		//!< Average stability by axis (outer nodes)
    float AvgPkPk[NGIMBAL_SHORT];  		//!< Average peak to peak by axis (outer nodes)
    float AvgClevisTemp;   				//!< Temperature reading from the clevis (Celcius)
}MotionCtrlDiagnostics_t;


//! T500 Microblaze diagnostic data
typedef struct
{
    UInt32 PacketRx;        //!< Packet receipt count over the VPS<->MB Interface                        
    UInt32 PacketTx;        //!< Packet transmit count over the VPS<->MB Interface 
}MicroblazeDiagnostics_t;


//! Video track control diagnostics
typedef struct
{
    UInt8 TrackMode;        //!< Current video track mode
    UInt8 Stamp;            //!< Current frame stamp, 0-254.
    float PanRate;          //!< Current gimbal outer loop pan rate
    float TiltRate;         //!< Current gimbal outer loop tilt rate
    float ShiftX;           //!< Current VPS reported pan shift.
    float ShiftY;           //!< Current VPS reported tilt shift.
    float AccumErrorX;      //!< Current accumulated pan scene error
    float AccumErrorY;      //!< Current accumulated tilt scene error
    float ControlRateX;     //!< Resultant pan rate from controller
    float ControlRateY;     //!< Resultant tilt rate from controller
    float FeedFwdRateX;     //!< Resultant pan feed forward rate
    float FeedFwdRateY;     //!< Resultant tilt feed forward rate
}VideoTrackDiagnostics_t;


//! Retractor enumerations
enum
{
    NO_RETRACTOR,       //!< No retractor installed
    THREEAX_RETRACTOR,  //!< 3AX retractor for TASE150 or TASE200
    ACR_RETRACTOR,      //!< ACR retractor for TASE150 or TASE200
    ARCTURUS_RETRACTOR, //!< Arcturus sensor safe retractor
    VTRACT1_RETRACTOR,  //!< VTRACT1 retractor for TASE300 or TASE400
    VTRACT2_RETRACTOR,  //!< VTRACT2 retractor for TASE300 or TASE400
    NUM_RETRACTORS
};


//! Retractor configuration data
typedef struct
{
    UInt32 Reserved : 21;       //!< Reserved bits, set to zero
    UInt32 RetractAtStart : 1;  //!< Set to automatically retract at startup
    UInt32 DeployAtStart : 1;   //!< Set to automatically deploy at startup
    UInt32 SafeToMove : 1;      //!< Set if gimbal can move/initialize while retracted
    UInt32 RetractorType : 8;   //!< Set to indicate the type of retraction system
    float RetractPan;           //!< Safe Pan angle used for retraction of SafeToMove is clear [rad]
    float RetractTilt;          //!< Safe Tilt angle used for retraction of SafeToMove is clear [rad]
    UInt16 DesiredDeploy;       //!< Desired deploy distance in millimeters.  Set to 0 or large value to deploy all the way.
    UInt16 DoorOpenPWM;         //!< Pulse width in microseconds to open the door of a ARCTURUS_RETRACTOR
    UInt16 DoorClosePWM;        //!< Pulse width in microseconds to close the door of a ARCTURUS_RETRACTOR
    UInt16 RetractUpPWM;        //!< Pulse width in microseconds to drive the retract servo of a ARCTURUS_RETRACTOR up
    UInt16 RetractNeutralPWM;   //!< Pulse width in microseconds to drive the retract servo of a ARCTURUS_RETRACTOR to neutral
    UInt16 RetractDownPWM;      //!< Pulse width in microseconds to drive the retract servo of a ARCTURUS_RETRACTOR down
    UInt16 LockPWM;             //!< Pusle width in microseconds to engage the lock servo of a ARCTURUS_RETRACTOR
    UInt16 UnlockPWM;           //!< Pusle width in microseconds to disengage the lock servo of a ARCTURUS_RETRACTOR

}Retractor_t;

/** @name Lens control type configuration enumerations */
//@{
typedef enum
{
    LENS_CONFIG_T300_LWIR_4680,     //!< T300 Delta Payload (obsolete)
    LENS_CONFIG_T300_NG_LWIR_4680,  //!< T300 Next Gen Delta Payload (obsolete)
    LENS_CONFIG_T400_MWIR_4680,     //!< T400 MWIR PIC18F4680
    LENS_CONFIG_T400_MWIR_66K80,    //!< T400 MWIR PIC18F66K80(never produced)
    LENS_CONFIG_T350_LWIR_66K80,    //!< T350 LWIR PIC18F66K80
    LENS_CONFIG_T400_LRS_66K80,     //!< T400 MWIR and LRS PIC18F66K80
    LENS_CONFIG_T400_DXR_66K80,     //!< T400 DXR PIC18F66K80
    LENS_CONFIG_T400_DNXR_4680,     //!< T400 DNXR PIC18F4680
    LENS_CONFIG_NONE = 0x7F         //!< No lens control exists for the hardware (e.g. T100, T150, T200)
} LensControlConfig;
//@}

//! Lens controller type 
typedef struct
{
	UInt8 HasType : 1;//!< Set if the lens type information is available
	UInt8 Type    : 7;//!< Type of lens controller board
}LensCtrlType_t;

//! Lens diagnostic information.
typedef struct
{
    UInt8   CameraId;               //!< Identifier of the camera with the lens that these diagnostics apply.
    UInt8   ControlId;              //!< Indicates if these diagnostics are for the cameras focus or zoom lens.
    UInt8	LimitSwitch : 1;		//!< '1' if limit switch is asserted/active
    UInt8	InitState : 3;		    //!< The initilization state.
    UInt8	Reserved : 4;			//!< Available
    SInt16  MinPosition;			//!< Minimum position in encoder ticks
    SInt16	MaxPosition;			//!< Maximum position in encoder ticks
    SInt16	CommandedPos;			//!< Commanded position in encoder ticks
    SInt16  ActualPos;				//!< Actual position in encoder ticks
    UInt16	EncoderErrCount;		//!< Encoder error count
    SInt16	CommandedVelocity;		//!< Commanded velocity in RPM
    SInt16  ActualVelocity;			//!< Actual velocity in RPM
    float   Temperature;	        //!< Lens temperature in degrees Celcius.
    float   Metric;                 //!< Last reported focus metric.
}LensDiagnostics_t;

//! Camera data
typedef struct
{    
    UInt8   CameraCount;            //!< Number of physical cameras installed in the gimbal.
    UInt8   ActiveCamera;           //!< Active camera index.
    float   HFov;                   //!< Horizontal field of view in radians
    float   VFov;                   //!< Vertical field of view in radians
    UInt16  FocusPos;               //!< Focus position as reported by the current camera
    float   FpaTemp;                //!< Focal plan array temperature in degrees Celcius
    float   ZoomRatio;              //!< Zoom ratio (current focal length / min focal length)
    UInt8   FocusMode;              //!< Focus mode
}CameraData_t;

//! VPS3 reset source data
typedef enum
{
	VPS3_RESET_POWER = 0,		//!< Power-on reset
	VPS3_RESET_WATCHDOG,		//!< Watchdog reset
	VPS3_RESET_UNKNOWN = 0xFF	//!< Unknown source
} Vps3Reset_t;

//! HD control modes on VPS3
typedef enum
{
    HD_MODE_NATIVE = 0,
    HD_MODE_CENTER_WINDOW = 1,
    HD_MODE_SHRINK_TO_FIT = 2
}HDControlSettings_t;

//! Estab settings
typedef enum
{
    ESTAB_ENABLED     = 0x00,
    ESTAB_DISABLED    = 0x01,
    ESTAB_DONT_CHANGE = 0xFF
}EStabModes_t;

//! VPS3 symbology flags
typedef struct
{
    UInt32 NoChange     : 1;    //!< Don't change flag
    UInt32 AltLayout    : 1;    //!< Use Alternate Layout (originally for AAI)
    UInt32 Reserved1    : 30;	//!< Reserved
    UInt32 Reserved2    : 14;   //!< Reserved
    UInt32 LocalTime    : 1;    //!< Local time (with GPS)
    UInt32 BitStatus    : 1;    //!< Bit Status
    UInt32 FaultStatus  : 1;    //!< Fault Status
    UInt32 TitleString  : 1;    //!< show user defined string.
    UInt32 AltSrcInUse  : 1;    //!< Show whichever source (gps or baro) is used by the piccolo.
    UInt32 AltFromBaro  : 1;	//!< User wants to use Barometric Altimeter in overlays. if not set use GPS
    UInt32 DebugInfo    : 1;	//!< Debugging info (fps, cpu, etc.)
    UInt32 Compass      : 1;	//!< Compass
    UInt32 GimbalAngles : 1;	//!< Gimbal pan/tilt or azim/elev
    UInt32 GimbalHFov   : 1;	//!< Gimbal horizontal field of view
    UInt32 GimbalPos    : 1;	//!< Gimbal position
    UInt32 TargetRange  : 1;	//!< Target range
    UInt32 TargetPos    : 1;	//!< Target position
    UInt32 DateAndTime  : 1;	//!< Date and time (with GPS)
    UInt32 Scale        : 1;	//!< Scale
    UInt32 Brackets     : 1;	//!< Center brackets
    UInt32 Crosshair    : 1;	//!< Crosshairs
    UInt32 TrackBox     : 1;	//!< Track box
} VPS3SymbolFlags_t;

//! VPS3 position format
typedef enum
{
	VPS_POS_FORMAT_MGRS_10 = 1,
	VPS_POS_FORMAT_UTM_HEMI,
	VPS_POS_FORMAT_RESERVED_0,
	VPS_POS_FORMAT_DEG,
	VPS_POS_FORMAT_DMS,
	VPS_POS_FORMAT_DMM,
	VPS_POS_FORMAT_NO_CHANGE = 0xFF
} VPS3PosFormat_t;

typedef enum
{
	LEN_FORMAT_METERS = 0,
	LEN_FORMAT_FEET,
	NUM_LEN_FORMATS,
	LEN_FORMAT_NO_CHANGE = 0xFF
} VPSLenFormat_t;

typedef enum
{
	ANGLE_FORMAT_PAN_TILT = 0,
	ANGLE_FORMAT_AZIM_ELEV,
	NUM_ANGLE_FORMATS,
	ANGLE_FORMAT_NO_CHANGE = 0xFF
} VPSAngleFormat_t;

//! VPS3 symbology settings
typedef struct
{
	VPS3SymbolFlags_t   Symbols;
	VPS3PosFormat_t     PosFormat;
	VPSLenFormat_t      LenFormat;
	VPSAngleFormat_t    AngleFormat;
	UInt8               FgColor[4];
	UInt8               BgColor[4];
	UInt8               TextFgColor[4];
	UInt8               TextBgColor[4];
    char                UserString[MAX_CUSTOM_STR_LEN];
} VPS3Symbology_t;

//! Microblaze reset source data
typedef enum
{
	MB_RESET_POWER = 0,		//!< Power-on reset
	MB_RESET_WATCHDOG,		//!< Watchdog reset
	MB_RESET_UNKNOWN = 0xFF	//!< Unknown source
} MbReset_t;

//! Max number of points that can be stored in RAM
#define MAX_SPOI_PATH_PTS 39

/** @name Gains enumeration for Accel Net (T2) */
//@{
enum
{
    AC_PAN_POS_P,
    AC_PAN_POS_VF,
    AC_PAN_POS_AF,
    AC_PAN_VEL_P,
    AC_PAN_VEL_I,
    AC_PAN_CUR_P,
    AC_PAN_CUR_I,
    AC_PAN_FREQ,
    AC_TILT_POS_P,
    AC_TILT_POS_VF,
    AC_TILT_POS_AF,
    AC_TILT_VEL_P,
    AC_TILT_VEL_I,
    AC_TILT_CUR_P,
    AC_TILT_CUR_I,
    AC_TILT_FREQ,
    AC_NUM_GAINS
};
//@}

//! Return a string description of the camera
SInt32 GetCameraStringName(const CameraDefinition_t* pCam, char* pName, UInt32 MaxSize);

//! Return a string description of the lens
SInt32 GetLensStringName(const CameraDefinition_t* pCam, char* pName, UInt32 MaxSize);

//! Fill out a gimbal packet header
void MakeGimbalPacketHeader(GimbalPkt_t* pPkt, UInt8 Frame, UInt8 SubFrame, UInt8 Size);

//! Fill out a gimbal packet header and checksum
void FinishGimbalPacket(GimbalPkt_t* pPkt, UInt8 Frame, UInt8 SubFrame, UInt8 Size);

//! Calculate and fill out a gimbal packet checksum
void ChecksumGimbalPacket(GimbalPkt_t* pPkt);

//! Calculate a gimbal packet checksum
UInt16 CalcGimbalPacketChecksum(const GimbalPkt_t* pPkt);

//! Build a gimbal packet from passed data
void MakeGimbalPacket(GimbalPkt_t* pPkt, UInt8 Frame, UInt8 SubFrame,
                      const UInt8* pData, UInt8 Size);

//! Look for a gimbal packet in  a series of bytes
BOOL LookForGimbalPacketInByte(GimbalPkt_t* pPkt, UInt8 Byte);

//! Decode a packet with the retractor configuration data.
BOOL DecodeGimbalRetractorConfigurationPacket(const GimbalPkt_t* pPkt, Retractor_t* pRetractor);

//! Form a packet with the retractor configuration data.
void FormGimbalRetractorConfigurationPacket(GimbalPkt_t* pPkt, const Retractor_t* pRetractor);

//! Decode a packet with FLIR configuration information.
BOOL DecodeGimbalFLIRConfigurationPacket(const GimbalPkt_t* pPkt, FLIRConfig_t* pConfig);

//! Form a packet with FLIR configuration information.
void FormGimbalFLIRConfigurationPacket(GimbalPkt_t* pPkt, const FLIRConfig_t* pConfig);

//! Decode a packet with IRCam configuration information.
BOOL DecodeGimbalIRCamConfigurationPacket(const GimbalPkt_t* pPkt, IRCamConfig_t* pConfig);

//! Form a packet with IRCam IR configuration information.
void FormGimbalIRCamConfigurationPacket(GimbalPkt_t* pPkt, const IRCamConfig_t* pConfig);

//! Decode a packet to save/restore IRCam IR configuration information.
BOOL DecodeGimbalIRCamSaveRestorePacket(const GimbalPkt_t* pPkt, UInt8* pConfigAction);

//! Form a packet to save/restore IRCam IR configuration information.
void FormGimbalIRCamSaveRestorePacket(GimbalPkt_t* pPkt, UInt8 ConfigAction);

//! Decode a packet with SU CSX configuration information.
BOOL DecodeSuCsxConfigurationPacket(const GimbalPkt_t* pPkt, SuCsxCamConfig_t* pConfig);

//! Form a packet with SU CSX configuration information.
void FormSuCsxConfigurationPacket(GimbalPkt_t* pPkt, const SuCsxCamConfig_t* pConfig);

//! Decode a packet to save/restore SU CSX configuration information.
BOOL DecodeSuCsxSaveRestorePacket(const GimbalPkt_t* pPkt, UInt8* pConfigAction);

//! Form a packet to save/restore SU CSX configuration information.
void FormSuCsxSaveRestorePacket(GimbalPkt_t* pPkt, UInt8 ConfigAction);

//! Decode a packet with KLV settings information
BOOL DecodeGimbalKLVDetailsPacket(const GimbalPkt_t* pPkt, KLVDetails_t* pKLV);

//! Form a packet with the KLV details
void FormGimbalKLVDetailsPacket(GimbalPkt_t* pPkt, const KLVDetails_t* pKLV);

//! Decode a packet with sensor configuration data
BOOL DecodeGimbalSensorConfigPacket(const GimbalPkt_t* pPkt, float Rotation[N3D], GimbalSensorFlags_t* pFlags);

//! Form a packet with the sensor configuration data
void FormGimbalSensorConfigPacket(GimbalPkt_t* pPkt, const float Rotation[N3D], GimbalSensorFlags_t Flags);

//! Decode a packet with magnetometer calibration data.
BOOL DecodeGimbalMagCalPacket(const GimbalPkt_t* pPkt, float HardIronErr[N3D], float ScaleFactErr[N3D]);

//! Form a packet with the magnetometer calibration data.
void FormGimbalMagCalPacket(GimbalPkt_t* pPkt, const float HardIronErr[N3D], const float ScaleFactErr[N3D]);

//! Interpret a serial interface setup packet
BOOL DecodeGimbalSerialInterfacePacket(const GimbalPkt_t* pPkt, GimbalSerial_t Interface[6], BOOL* pTemporary);

//! Create a serial interface setup packet
void FormGimbalSerialInterfacePacket(GimbalPkt_t* pPkt, const GimbalSerial_t Interface[6], BOOL Temporary);

//! Interpret a camera definitions packet
BOOL DecodeCameraDefinitionsPacket(const GimbalPkt_t* pPkt, CameraDefinition_t Cam[NUM_CAMERAS], BOOL Received[NUM_CAMERAS]);

//! Createa a camera definitions packet
void FormCameraDefinitionsPacket(GimbalPkt_t* pPkt, const CameraDefinition_t Cam[NUM_CAMERAS], const BOOL Send[NUM_CAMERAS], BOOL Factory);

//! Interpret a camera command packet containing the field of view and focus.
BOOL DecodeGimbalCameraCmdPacket(const GimbalPkt_t* pPkt, UInt8* Camera, float* pHFOV, UInt16* pFocus, UInt8* pEMode, UInt8* pShutter, UInt8 *pBright, UInt8* pFlags);

//! Create a camera command packet containing the field of view and focus.
void FormGimbalCameraCmdPacket(GimbalPkt_t* pPkt, UInt8 Camera, float HFOV, UInt16 Focus, UInt8 Mode, UInt8 Shutter, UInt8 Bright, UInt8 Flags);

//! Interpret a gimbal sensor point of interest packet containing the position of the SPOI
BOOL DecodeGimbalSPOICmdPacket(const GimbalPkt_t* pPkt, double Pos[NPOS], float Vel[NDIR], UInt8* pFlags);

//! Create a gimbal sensor point of interest packet containing the position of the SPOI
void FormGimbalSPOICmdPacket(GimbalPkt_t* pPkt, const double Pos[NPOS], const float Vel[NDIR], UInt8 Flags);

//! Interpret a gimbal axis control packet
BOOL DecodeGimbalAxisSettingsPacket(const GimbalPkt_t* pPkt, float NoGoCenter[NGIMBAL_SHORT], float NoGoRange[NGIMBAL_SHORT], float MaxRate[NGIMBAL_SHORT], float MaxAccel[NGIMBAL_SHORT], float PosGain[NGIMBAL_SHORT], float VelGain[NGIMBAL_SHORT], float DerGain[NGIMBAL_SHORT], BOOL* pTemporary);

//! Create a gimbal axis control packet
void FormGimbalAxisSettingsPacket(GimbalPkt_t* pPkt, const float NoGoCenter[NGIMBAL_SHORT], const float NoGoRange[NGIMBAL_SHORT], const float MaxRate[NGIMBAL_SHORT], const float MaxAccel[NGIMBAL_SHORT], const float PosGain[NGIMBAL_SHORT], const float VelGain[NGIMBAL_SHORT], const float DerGain[NGIMBAL_SHORT], BOOL Temporary, BOOL Factory);

//! Interpret a gimbal filter packet
BOOL DecodeGimbalFilterPacket(const GimbalPkt_t* pPkt, float* pCutoff, float* pDeadband);

//! Create a gimbal filter packet
void FormGimbalFilterPacket(GimbalPkt_t* pPkt, float Cutoff, float Deadband);

//! Interpret a gimbal GPS lever arm packet
BOOL DecodeGimbalGPSLeverArmPacket(const GimbalPkt_t* pPkt, float Arm[N3D]);

//! Create a gimbal GPS lever arm packet
void FormGimbalGPSLeverArmPacket(GimbalPkt_t* pPkt, const float Arm[N3D]);

//! Interpret a gimbal camera alignment packet
BOOL DecodeGimbalCameraAlignPacket(const GimbalPkt_t* pPkt, float* pZoom1, float Error1[NGIMBAL], float* pZoom2, float Error2[NGIMBAL], UInt8* pCamera);

//! Create a gimbal camera alignment packet
void FormGimbalCameraAlignPacket(GimbalPkt_t* pPkt, float Zoom1, const float Error1[NGIMBAL], float Zoom2, const float Error2[NGIMBAL], UInt8 Camera, BOOL Factory);

//! Create the gimbal camera zoom packet
void FormGimbalCameraZoomPacket(GimbalPkt_t* pPkt, UInt8 Camera, SInt8 Rate, float Timeout);

//! Decode the gimbal camera zoom packet
BOOL DecodeGimbalCameraZoomPacket(const GimbalPkt_t* pPkt, UInt8* pCamera, SInt8* pRate, float* pTimeout);

//! Interpret a 3AX retract or deploy command packet
BOOL DecodeGimbalRetractDeployCmdPacket(const GimbalPkt_t* pPkt, UInt32* pDeploy);

//! Create a 3AX retract or deploy command packet
void FormGimbalRetractDeployCmdPacket(GimbalPkt_t* pPkt, UInt32 Deploy);

//! Interpret a gimbal rig test command packet
BOOL DecodeGimbalRigTestCmdPacket(const GimbalPkt_t* pPkt, float* pMaxSpeed, float* pMaxAccel, UInt16* pTime);

//! Create a gimbal rig test packet
void FormGimbalRigTestCmdPacket(GimbalPkt_t* pPkt, float MaxSpeed, float MaxAccel, UInt16 Time);

//! Interpret a laser arm command
BOOL DecodeGimbalLaserArmCmdPacket(const GimbalPkt_t* pPkt, UInt8* pLaserIndex, BOOL* pArm);

//! Create a laser arm command
void FormGimbalLaserArmCmdPacket(GimbalPkt_t* pPkt, UInt8 LaserIndex, BOOL Arm);

//! Interpret a laser fire command
BOOL DecodeGimbalLaserFireCmdPacket(const GimbalPkt_t* pPkt, UInt8* pLaserIndex, UInt8* pPower);

//! Create a laser fire command
void FormGimbalLaserFireCmdPacket(GimbalPkt_t* pPkt, UInt8 LaserIndex, UInt8 Power);

//! Interpret a lasers installed packet
BOOL DecodeGimbalLasersInstalledPacket(const GimbalPkt_t* pPkt, LaserSetup_t Lasers[NUM_LASERS]);

//! Create a lasers installed packet
void FormGimbalLasersInstalledPacket(GimbalPkt_t* pPkt, const LaserSetup_t Lasers[NUM_LASERS], BOOL Factory);

//! Create a laser status and safety packet
void FormLaserSafetyStatus( GimbalPkt_t* pPkt, BOOL BypassSafety, UInt8 flags );

//! Interpret a laser status and safety packet
BOOL DecodeLaserSafetyStatus( const GimbalPkt_t* pPkt, BOOL* BypassSafety, UInt8* pFlags );

//! Form a laser watchdog packet for the laser with index LaserIndex;
void FormLaserWatchdogPacket(GimbalPkt_t* pPkt, UInt8 LaserIndex);

//! Interpret a laser watchdog packet. The relevent laser index is returned if successful.
BOOL DecodeLaserWatchdogPacket(const GimbalPkt_t* pPkt, UInt8 *LaserIndex);

//! Form a packet with the password to enable laser operations.
void FormLaserEnablePswdPacket( GimbalPkt_t* pPkt, UInt16 pswd, BOOL ByPassAgl );

//! Decode a laser password packet.
BOOL DecodeLaserEnablePswdPacket( const GimbalPkt_t* pPkt, UInt16* pswd, BOOL* pByPassAgl );

//! Tell the gimbal what its AGL altitude is
void FormGimbalAglPacket( GimbalPkt_t* pPkt, UInt16 AglMeters, BOOL AglValid);

//! Decode a AGL altitude packet for the gimbal
BOOL DecodeGimbalAglPacket(const GimbalPkt_t* pPkt, UInt16* pAglMeters, BOOL* pAglValid );

//! Decode a radio settings packet for the gimbal
BOOL DecodeGimbalRadioSettings(const GimbalPkt_t* pPkt, RadioSettings_t* pSettings);

//! Form a radio settings packet for the gimbal
void FormGimbalRadioSettings(GimbalPkt_t* pPkt, const RadioSettings_t* pSettings);

//! Decode the track command packet
BOOL DecodeGimbalTrackCommand(const GimbalPkt_t* pPkt, float* pInitialX, float* pInitialY, UInt8* pAlgorithm, UInt8* pSize, UInt8* pStamp);

//! Form the track command packet
void FormGimbalTrackCommand(GimbalPkt_t* pPkt, float InitialX, float InitialY, UInt8 Algorithm, UInt8 Size, UInt8 Stamp);

//! Decode the image enhancement command packet
BOOL DecodeGimbalEnhancementCommand(const GimbalPkt_t* pPkt, BOOL *pEnabled, UInt8 *pIntensity, UInt8 *pLimit);

//! Form the image enhancement command packet
void FormGimbalEnhancementCommand(GimbalPkt_t* pPkt, BOOL Enabled, UInt8 Intensity, UInt8 Limit);

//! Decode the advanced settings packet
BOOL DecodeGimbalTrackerSettingsAdvancedCommand(const GimbalPkt_t* pPkt, float* pGainP, float* pGainI, float* pGainD, float* pRateLimit, float* pDeadband);

//! Form the advanced settings packet
void FormGimbalTrackerSettingsAdvancedCommand(GimbalPkt_t* pPkt, float GainP, float GainI, float GainD, float RateLimit, float Deadband, BOOL Factory);

//! Decode a radio settings packet for the video ground station
BOOL DecodeVGSRadioSettings(const GimbalPkt_t* pPkt, RadioSettings_t* pSettings);

//! Form a radio settings packet for the video ground station
void FormVGSRadioSettings(GimbalPkt_t* pPkt, const RadioSettings_t* pSettings);

//! Decode a packet that sets the current yaw angle of the antenna
BOOL DecodeVGSAntennaYaw(const GimbalPkt_t* pPkt, float* pYaw);

//! Form a packet that sets the current yaw angle of the antenna
void FormVGSAntennaYaw(GimbalPkt_t* pPkt, float Yaw);

//! Decode a T2 development settings packet with motor controller gains.
BOOL DecodeStabDevT2Settings(const GimbalPkt_t* pPkt, SInt16 Gains[AC_NUM_GAINS]);

//! Create a T2 development settings packet with motor controller gains.
void FormStabDevT2Settings(GimbalPkt_t* pPkt, const SInt16 Gains[AC_NUM_GAINS]);

//! Decode a packet that has the gimbal tracking data in it
BOOL DecodeGimbalTrackingData(const GimbalPkt_t* pPkt, float GimbalAngles[NGIMBAL], float CameraAngles[NAXIS], float* pHFOV, float* pRange, UInt8* pMode);

//! Create a packet that has the gimbal tracking data in it
void FormGimbalTrackingData(GimbalPkt_t* pPkt, const float GimbalAngles[NGIMBAL], const float CameraAngles[NAXIS], float HFOV, float Range, UInt8 Mode);

//! Create a SPOI path navigator settings packet
void FormSPOIPathSettings(GimbalPkt_t* pPkt, UInt8 StepMode, float MaxVel, float ActiveRange, float ConvDist, float StepDist, UInt32 StepTime, float ScanRate);

//! Decode a SPOI path navigator settings packet
BOOL DecodeSPOIPathSettings(const GimbalPkt_t* pPkt, UInt8* pStepMode, float* pMaxVel, float* pActiveRange, float* pConvDist, float* pStepDist, UInt32* pStepTime, float* pScanRate);

//! Create a SPOI path point data packet
BOOL FormSPOIPathPointData(GimbalPkt_t* pPkt, const double PosECEF[MAX_SPOI_PATH_PTS][NPOS], UInt8 NumPoints);

//! Decode a SPOI path point data packet
BOOL DecodeSPOIPathPointData(const GimbalPkt_t* pPkt, double PosECEF[MAX_SPOI_PATH_PTS][NPOS], UInt8* pNumPoints);

//! Form a packet that contains the image frame shifts
void FormFrameShiftPacket(GimbalPkt_t* pPkt, UInt8 Source, float FrameShiftH, float FrameShiftV, UInt16 NumFrames, UInt16 DeltaT);

//! Decode a packet that contains the image frame shifts
BOOL DecodeFrameShiftPacket(const GimbalPkt_t* pPkt, UInt8* pSource, float* pFrameShiftH, float* pFrameShiftV, UInt16* pNumFrames, UInt16* pDeltaT);

//! Form a packet that specifies the pre-focus calibration table
void FormLensPrefocusPacket(GimbalPkt_t* pPkt, UInt8 CameraIndex, UInt8 LensType, const float Zooms[10], const UInt16 Focuses[10], UInt8 NumEntries);

//! Decode a packet that specifies the pre-focus calibration table
BOOL DecodeLensPrefocusPacket(const GimbalPkt_t* pPkt, UInt8* pCameraIndex, UInt8* pLensType, float Zooms[10], UInt16 Focuses[10], UInt8* pNumEntries);

//! Form a packet that specifies the zoom points of a pre-focus calibration table.
void FormLensCalZoomPacket(GimbalPkt_t* pPkt, UInt8 CameraIndex, const SInt16 Zooms[26], UInt8 NumEntries);

//! Decode a packet that specifies the zoom points of a pre-focus calibration table.
BOOL DecodeLensCalZoomPacket(const GimbalPkt_t* pPkt, UInt8* pCameraIndex, SInt16 Zooms[26], UInt8* pNumEntries);

//! Form a packet that specifies the temperature and focus points of a pre-focus calibration table
void FormLensCalFocusPacket(GimbalPkt_t* pPkt, UInt8 CameraIndex, UInt8 RowIndex, float Temperature, const SInt16 Focuses[AF_CAL_POINTS], UInt8 NumEntries);

//! Decode a packet that specifies the temperature and focus points of a pre-focus calibration table
BOOL DecodeLensCalFocusPacket(const GimbalPkt_t* pPkt, UInt8* pCameraIndex, UInt8* pRowIndex, float* pTemperature, SInt16 Focuses[AF_CAL_POINTS], UInt8* pNumEntries);

//! Form a packet containing a CAN frame received from or to be sent to CAN-B
void FormCanPassthroughPacket(GimbalPkt_t* pPkt, UInt16 ID, const UInt8 Data[8], UInt8 Length);

//! Decode a packet containing a CAN frame received from or to be sent to CAN-B
BOOL DecodeCanPassthroughPacket(const GimbalPkt_t* pPkt, UInt16 *pID, UInt8 Data[8], UInt8 *pLength);

//! Set laser parameters that are fixed based on laser type.
void SetLaserFixedParams(LaserSetup_t *pLaser);

//! Decode the HD control settings 
BOOL DecodeHdControlSettings(const GimbalPkt_t* pPkt, HDControlSettings_t* pSettings);

// Encode the HD control settings 
void FormHdControlSettings(GimbalPkt_t* pPkt, HDControlSettings_t Settings, BOOL Factory);

//! Decode the VPS3 overlay settings
BOOL DecodeVps3OverlaySettings(const GimbalPkt_t* pPkt, BOOL* pEnabled, VPS3Symbology_t* pSettings, float* localTimeOffsetHours, BOOL* localTimeDST);

// Encode the VPS3 overlay settings
void FormVps3OverlaySettings(GimbalPkt_t* pPkt, BOOL Enabled, VPS3Symbology_t* pSettings, float localTimeOffsetHours, BOOL localTimeDST, BOOL Factory);

//! Decode the VPS3 picture in picture settings
BOOL DecodeVps3PiPSettings(const GimbalPkt_t* pPkt, BOOL* pEnabled );

// Encode the VPS3 picture in picture settings
void FormVps3PiPSettings(GimbalPkt_t* pPkt, BOOL enabled, BOOL factory);

//! Decode the estab control settings 
BOOL DecodeEstabSettings(const GimbalPkt_t* pPkt, EStabModes_t* pMode);

// Encode the estab settings 
void FormEstabSettings(GimbalPkt_t* pPkt, EStabModes_t Mode, BOOL Factory);

//! Decode a video settings packet
BOOL DecodeMISBSettingsPacket(const GimbalPkt_t* pPkt, MISBConfig_t* pData);

//! Form a video settings packet
void FormMISBSettingsPacket(GimbalPkt_t* pPkt, const MISBConfig_t Data, BOOL Factory);

//! Decode the estimate of the target location position coming from the user.
BOOL DecodeGimbalTargetLocationEstimate(const GimbalPkt_t* pPkt, double PosLLA[NPOS], UInt32* pGPSTOW, UInt8* pSource, UInt8* pError);

//! Create a target location estimate packet
void FormGimbalTargetLocationEstimate(GimbalPkt_t* pPkt, const double PosLLA[NPOS], UInt32 GPSTOW, UInt8 source, UInt8 error );

//! Form a motion control do calibration packet
void FormMC_DoMotionCalibrationPacket(GimbalPkt_t *pPkt, UInt16 Flags, UInt16 Type );

//! Decode a motion control parameter packet
BOOL DecodeMC_MotionCalibrationParamPacket( const GimbalPkt_t *pPkt, float Offsets[3], float GainMatrix[9] );

//! Form a motion control parameter packet
void FormMC_MotionCalibrationParamPacket(GimbalPkt_t *pPkt, const float Offsets[3], const float GainMatrix[9], BOOL Factory );

//! Decode a motion control axis parameter packet
BOOL DecodeMC_AxisParamPacket(const GimbalPkt_t *pPkt, float *pInPanKp, float *pInPanKi, float *pInPanKd, float *pInTiltKp, float *pInTiltKi, float *pInTiltKd,
                               float *pOutPanKp, float *pOutPanKi, float *pOutPanKd, float *pOutTiltKp, float *pOutTiltKi, float *pOutTiltKd);

//! Form a motion control axis parameter packet
void FormMC_AxisParamPacket(GimbalPkt_t *pPkt, float InPanKp, float InPanKi, float InPanKd, float InTiltKp, float InTiltKi, float InTiltKd,
                               float OutPanKp, float OutPanKi, float OutPanKd, float OutTiltKp, float OutTiltKi, float OutTiltKd, BOOL Factory);

//! Decode a motion control limits packet
BOOL DecodeMC_LimitsPacket(const GimbalPkt_t *pPkt, float *pPanMin, float *pPanMax, float *pTiltMin, float *pTiltMax, 
                           float *pPanRateMax, float *pTiltRateMax, float *pPanAccelMax, float *pTiltAccelMax );

//! Form a motion control limits packet
void FormMC_LimitsPacket(GimbalPkt_t *pPkt, float PanMin, float PanMax, float TiltMin, float TiltMax, 
                         float PanRateMax, float TiltRateMax, float PanAccelMax, float TiltAccelMax, BOOL Factory);

//! Form a packet to set a lens register value
void FormLensRegisterPacket(GimbalPkt_t* pPkt, UInt8 CameraId, UInt8 ControlType, UInt16 Address, UInt32 Value, BOOL Write);

//! Decode a packet to set a lens register value
BOOL DecodeLensRegisterPacket(const GimbalPkt_t* pPkt, UInt8* pCameraId, UInt8* pControlType, UInt16* pAddress, UInt32* pValue, BOOL* pWrite);

//! Form a packet to directly command a Microblaze controlled lens.
void FormLensDevCommandPacket(GimbalPkt_t* pPkt, UInt8 CameraId, UInt8 ControlType, UInt8 InitFlag, SInt16 Position);

//! Decode a packet to directly command a Microblaze controlled lens.
BOOL DecodeLensDevCommandPacket(const GimbalPkt_t* pPkt, UInt8* pCameraId, UInt8* pControlType, UInt8* pInitFlag, SInt16* pPosition);

//! Form a packet to enable/disable lens diagnostic reporting
void FormSetLensDiagnosticsPacket(GimbalPkt_t* pPkt, UInt8 CameraId, BOOL Enabled);

//! Decode a lens diagnostic packet
BOOL DecodeLensDiagnosticsPacket(const GimbalPkt_t* pPkt, LensDiagnostics_t* pData);

//! Form a packet to enable and configure gimbal scan mode control.
void FormScanModeControlPacket(GimbalPkt_t* pPkt, const GimbalScanControlConfig_t* pData);

//! Decode a gimbal scan mode configuration packet.
BOOL DecodeScanModeControlPacket(const GimbalPkt_t* pPkt, GimbalScanControlConfig_t* pData);

//! Form a packet to send watermark payload data.
void FormGimbalWaterMarkPacket(GimbalPkt_t* pPkt, UInt8 Format, UInt8 Status, UInt32 Sequence, const UInt8* pImageData, UInt8 Size);

//! Decode a packet with watermark payload data.
BOOL DecodeGimbalWaterMarkPacket(const GimbalPkt_t* pPkt, UInt8* pFormat, UInt8* pStatus, UInt32* pSequence, UInt8* pImageData, UInt8* pSize);

#ifdef __cplusplus
}
#endif

#endif // _GIMBAL_PACKET_GENERIC_H

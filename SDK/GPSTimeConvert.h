#ifndef GPSTIMECONVERT_H
#define GPSTIMECONVERT_H

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Convert gps time (ITOW and week) to year month, day, hours, minutes,
void gps_to_ymdhms(UInt32 gps_week, UInt32 ITOW, UInt16 *year, UInt8 *month,
                   UInt8* day, UInt8 *hours, UInt8 *minutes, float *seconds);

//! Convert calendar year month day hours minutes seconds to GPS time.
void ymdhms_to_gps(UInt32 year, UInt32 month, UInt32 day, UInt32 hour,
                  UInt32 minute, float second, UInt16 *gps_week, UInt32 *ITOW);

//! Calculate hours minutes seconds from GPS time of week.
void GPSITOW_to_hms(UInt32 ITOW, UInt8 *hours, UInt8* minutes, float* seconds);

//! Convert year and day of year to gps time.
void yearday_to_gps(UInt32 year, UInt32 Days, UInt16 *gps_week, UInt32 *ITOW);

//! Calculate UTC week and ITOW from GPS week and ITOW. UTC time is leap
//  milliseconds behind GPS time.
void GPS_to_UTC_WeekITOW(UInt16 GPSWeek, UInt32 GPSITOW, UInt32 LeapMilliseconds, UInt16 *pUTCWeek, UInt32 *pUTCITOW);

//! Get time in microseconds since midnight Jan 1 1970
UInt64 UnixTime(UInt32 Week, UInt32 ITOW);

#ifdef __cplusplus
}
#endif

#endif


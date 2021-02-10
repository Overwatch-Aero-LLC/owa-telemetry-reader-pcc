/*! \file
	\brief Quaternion attitude representation functions
	This file includes quaternion utilities such as conversions to/from Euler angles,
	generation of Direction Cosine Matrix (DCM), the Jacobian of the DCM, as well as the
	quaternion dynamics (kinematic equations)
*/

#ifndef _ATTITUDE_H
#define _ATTITUDE_H

#include "Types.h"
#include "Indices.h"


#define MAX_QUAT_ERR	1E-6	//!< The quaternion threshold error for normalization


// Function headers

#ifdef __cplusplus
extern "C" {
#endif

//! Calculate the conjugate of a quaternion in place
void QuatConj(double Quat[N_QUAT]);

//! Calculate the conjugate of a quaternion in place
void QuatConjf(float Quat[N_QUAT]);

//! Calculate Euler angles from attitude quaternion
void Quat2Euler(const double Quat[N_QUAT], double Euler[NAXIS]);

//! Calculate attitude quaternion from Euler angles
void Euler2Quat(const double Euler[NAXIS], double Quat[N_QUAT]);

//! Calculate attitude quaternion from Euler angles
void Euler2Quatf(const float Euler[NAXIS], float Quat[N_QUAT]);

//! Calculate inertial-to-body transformation matrix from attitude quaternion
void Quat2DCM(const double Quat[N_QUAT], double *DCM);

//! Calculate attitude quaternion from inertial-to-body transformation matrix
void DCM2Quat(double *DCM, double Quat[N_QUAT]);

//! Translate a 3-D vector from inertial frame to body frame
void RotateNav2Body(const double Quat[N_QUAT], const double VecN[NDIR], double VecB[N3D]);

//! Translate a 3-D vector from body frame to inertial frame
void RotateBody2Nav(const double Quat[N_QUAT], const double VecB[N3D], double VecN[NDIR]);

//! Calculate magnitude of a quaternion vector
double MagnitudeQuat(const double Quat[N_QUAT]);

//! Normalize a quaternion vector
void NormalizeQuat(const double QuatIn[N_QUAT], double QuatOut[N_QUAT]);

//! Attitude propagation equations (kinematics) with quaternion formulation
void QuatDynamics(const double Quat[N_QUAT], const double Gyro[NAXIS], double QuatDot[N_QUAT]);

//! Jacobian of the kinematic equations with respect to quaternion elements and angular rates
void QuatJacobian(const double Quat[N_QUAT], const double Gyro[NAXIS], double *JQuat, double *JRates);

//! Jacobian of the inertial-to-body transformation matrix with respect to quaternion elements
void DCMJacobian(const double Quat[N_QUAT], double *JDCM);

//! The error between two angles -> between -pi and pi
double AngleDifference(double a, double b);

//! Quaternion multiplication operation
void QuatProduct(const double q[N_QUAT], const double s[N_QUAT], double p[N_QUAT]);

//! Quaternion multiplication operation
void QuatProductf(const float q[N_QUAT], const float s[N_QUAT], float p[N_QUAT]);

//! Quaternion multiplication of a 3-D vector with a quaternion.
void VecQuatProduct(const double v[N3D], const double q[N_QUAT], double p[N_QUAT]);

//! Quaternion multiplication of a 3-D vector with a quaternion.
void VecQuatProductf(const float v[N3D], const float q[N_QUAT], float p[N_QUAT]);

//! Compute the attitude error efficiently under the assumption of small attitude error
void QuatErrFast(const double Qinitial[N_QUAT], const double Qfinal[N_QUAT], double Err[NAXIS]);

//! Compute the angular error corresponding to the multiplicative error between two quaternions.
void QuatErrLim(const double Qinitial[N_QUAT], const double Qfinal[N_QUAT], double MaxErr, double Err[NAXIS]);

//! Compute the attitude error between two quaternions
void QuatErr(const double Quat1[N_QUAT], const double Quat2[N_QUAT], double Err[NAXIS]);

//! Compute the attitude error between two Euler vectors
void EulerErr(const double Euler1[NAXIS], const double Euler2[NAXIS], double Err[NAXIS]);

//! Rotate a vector about the vertical axis, from navigation frame to body frame
void RotateNav2Body_2D(double Psi, const double VecN[NDIR], double VecB[N3D]);

//! Rotate a vector about the vertical axis, from body frame to navigation frame
void RotateBody2Nav_2D(double Psi, const double VecB[N3D], double VecN[NDIR]);

//! Compute the attitude quaternion time derivative
void QuatDynamicsf(const float Quat[N_QUAT], const float Gyro[NAXIS], float QuatDot[N_QUAT]);

//! Compute the attitude error efficiently under the assumption of small attitude error
void QuatErrFastf(const float Qinitial[N_QUAT], const float Qfinal[N_QUAT], float Err[NAXIS]);

//! Compute the angular error corresponding to the multiplicative error between two quaternions.
void QuatErrLimf(const float Qinitial[N_QUAT], const float Qfinal[N_QUAT], float MaxErr, float Err[NAXIS]);

//! Compute the attitude error between two quaternions
void QuatErrf(const float Quat1[N_QUAT], const float Quat2[N_QUAT], float Err[NAXIS]);

//! Compute the attitude error between two Euler vectors
void EulerErrf(const float Euler1[NAXIS], const float Euler2[NAXIS], float Err[NAXIS]);

//! Rotate a vector about the vertical axis, from navigation frame to body frame
void RotateNav2Body_2Df(float Psi, const float VecN[NDIR], float VecB[N3D]);

//! Rotate a vector about the vertical axis, from body frame to navigation frame
void RotateBody2Nav_2Df(float Psi, const float VecB[N3D], float VecN[NDIR]);

//! Compute magnitude (norm) of the quaternion vector
float MagnitudeQuatf(const float Quat[N_QUAT]);

//! Normalize the attitude quaternion
void NormalizeQuatf(const float QuatIn[N_QUAT], float QuatOut[N_QUAT]);

//! Convert quaternion to Euler angles
void Quat2Eulerf(const float Quat[N_QUAT], float Euler[NAXIS]);

//! Convert Euler angles to quaternion
void Euler2Quatf(const float Euler[NAXIS], float Quat[N_QUAT]);

//! Calculate inertial-to-body transformation matrix from attitude quaternion
void Quat2DCMf(const float Quat[N_QUAT], float *DCM);

//! Transfer a float vector from navigation frame to body frame
void RotateNav2Bodyf(const float Quat[N_QUAT], const float VecN[NDIR], float VecB[N3D]);

//! Transfer a float vector from body frame to navigation frame
void RotateBody2Navf(const float Quat[N_QUAT], const float VecB[N3D], float VecN[NDIR]);

//! Calculate the roll angle derivatives to quaternion elements.
void CalculateRollDeriv(double dPhi[N_QUAT], const double Quat[N_QUAT]);

//! Calculate the pitch angle derivatives to quaternion elements.
void CalculatePitchDeriv(double dTheta[N_QUAT], const double Quat[N_QUAT]);

//! Calculate the heading angle derivatives to quaternion elements.
void CalculateHeadingDeriv(double dPsi[N_QUAT], const double Quat[N_QUAT]);

//! Calculate the roll angle derivatives to quaternion elements.
void CalculateRollDerivf(float dPhi[N_QUAT], const float Quat[N_QUAT]);

//! Calculate the pitch angle derivatives to quaternion elements.
void CalculatePitchDerivf(float dTheta[N_QUAT], const float Quat[N_QUAT]);

//! Calculate the heading angle derivatives to quaternion elements.
void CalculateHeadingDerivf(float dPsi[N_QUAT], const float Quat[N_QUAT]);

//! Calculate Euler angles standard deviation from quaternion standard deviation.
void CalculateEulerSigma(double EulerSigma[NAXIS], const double QuatSigma[N_QUAT], const double Quat[N_QUAT]);

//! Calculate Euler angles standard deviation from quaternion standard deviation, given the Euler angles
void CalculateEulerSigmaEx(double EulerSigma[NAXIS], const double QuatSigma[N_QUAT], double Theta, double Psi);

//! Calculate Euler angles standard deviation from quaternion standard deviation.
void CalculateEulerSigmaf(float EulerSigma[NAXIS], const float QuatSigma[N_QUAT], const double Quat[N_QUAT]);

//! Calculate Euler angles standard deviation from quaternion standard deviation, given the Euler angles
void CalculateEulerSigmaExf(float EulerSigma[NAXIS], const float QuatSigma[N_QUAT], float Theta, float Psi);

//! Calculate roll angle from quaternion.
double RollAngleFromQuat(const double Quat[N_QUAT]);

//! Calculate pitch angle from quaternion.
double PitchAngleFromQuat(const double Quat[N_QUAT]);

//! Calculate heading angle from quaternion.
double HeadingAngleFromQuat(const double Quat[N_QUAT]);

//! Calculate roll angle from quaternion.
float RollAngleFromQuatf(const float Quat[N_QUAT]);

//! Calculate pitch angle from quaternion.
float PitchAngleFromQuatf(const float Quat[N_QUAT]);

//! Calculate heading angle from quaternion.
float HeadingAngleFromQuatf(const float Quat[N_QUAT]);

//! Make quaternion for a roll angle rotation.
void QuatFromRollAngle(double Quat[N_QUAT], double Phi);

//! Make quaternion for a pitch angle rotation.
void QuatFromPitchAngle(double Quat[N_QUAT], double Theta);

//! Make quaternion for a heading angle rotation.
void QuatFromHeadingAngle(double Quat[N_QUAT], double Psi);

//! Make quaternion for a combined roll+pitch rotation.
void QuatFromRollPitchAngle(double Quat[N_QUAT], double Phi, double Theta);

//! Make quaternion for a roll angle rotation.
void QuatFromRollAnglef(float Quat[N_QUAT], float Phi);

//! Make quaternion for a pitch angle rotation.
void QuatFromPitchAnglef(float Quat[N_QUAT], float Theta);

//! Make quaternion for a heading angle rotation.
void QuatFromHeadingAnglef(float Quat[N_QUAT], float Psi);

//! Make quaternion for a combined roll+pitch rotation.
void QuatFromRollPitchAnglef(float Quat[N_QUAT], float Phi, float Theta);

//! Computer Euler derivatives
void EulerDynamics(const double Euler[NAXIS], const double Gyro[NAXIS], double EulerDot[NAXIS]);

//! Computer Euler derivatives
void EulerDynamicsf(const float Euler[NAXIS], const float Gyro[NAXIS], float EulerDot[NAXIS]);

//! Build quaternion for tracking transformation
void QuatForTracking(double Quat[N_QUAT], double Pan, double Tilt);

//! Build quaternion for tracking transformation
void QuatForTrackingf(float Quat[N_QUAT], float Pan, float Tilt);

//! Calculate the pan and tilt angles of a tracking vector
void GetTrackVectorAngles(double* pPan, double* pTilt, const double Track[N3D]);

//! Calculate the pan and tilt angles of a tracking vector
void GetTrackVectorAnglesf(float* pPan, float* pTilt, const float Track[N3D]);

//! Calculate the pan and tilt angles standard deviations of a tracking vector
void GetTrackVectorAnglesSigma(double* pPanSigma, double* pTiltSigma, const double Track[N3D],
							   const double TrackSigma[N3D]);

//! Calculate the pan and tilt angles standard deviations of a tracking vector
void GetTrackVectorAnglesSigmaf(float* pPanSigma, float* pTiltSigma, const float Track[N3D],
							    const float TrackSigma[N3D]);

//! Calculate a tracking vector from pan and tilt angles
void GetTrackVectorFromAngles(double Track[N3D], double Pan, double Tilt);

//! Calculate a tracking vector from pan and tilt angles
void GetTrackVectorFromAnglesf(float Track[N3D], float Pan, float Tilt);

//! Calculate a tracking vector's standard deviation from pan and tilt angles standard deviation
void GetTrackVectorSigmaFromAngles(double TrackSigma[N3D], double Pan, double Tilt,
								   double PanSigma, double TiltSigma);

//! Calculate a tracking vector's standard deviation from pan and tilt angles standard deviation
void GetTrackVectorSigmaFromAnglesf(float TrackSigma[N3D], float Pan, float Tilt,
								    float PanSigma, float TiltSigma);

//! Build quaternion give trigonometric functions of the half-angle.
void QuatFromHalfAngle(double Quat[N_QUAT], const double Axis[N3D], double Cphi, double Sphi);

//! Build quaternion give trigonometric functions of the half-angle.
void QuatFromHalfAnglef(float Quat[N_QUAT], const float Axis[N3D], float Cphi, float Sphi);

//! Build quaternion for a rotation about a fixed arbitrary axis
void QuatForFixedAxisRotation(double Quat[N_QUAT], const double Axis[N3D], double Angle);

//! Build quaternion for a rotation about a fixed arbitrary axis
void QuatForFixedAxisRotationf(float Quat[N_QUAT], const float Axis[N3D], float Angle);

//! Discrete quaternion propagation using Maclaurin series expansion.
void PropagateQuaternion(double Quat[N_QUAT], const double delta[NAXIS]);

//! Discrete quaternion propagation using Maclaurin series expansion.
void PropagateQuaternionf(float Quat[N_QUAT], const float delta[NAXIS]);

//! Update a nav-to-body quaternion by an angle increment in body frame
void UpdateQuaternionB(double Quat[N_QUAT], const double deltaB[NAXIS]);

//! Update a nav-to-body quaternion by an angle increment in body frame
void UpdateQuaternionBf(float Quat[N_QUAT], const float deltaB[NAXIS]);

//! Update a nav-to-body quaternion by the inverse of an angle increment in nav frame
void UpdateQuaternionN(double Quat[N_QUAT], const double deltaN[NDIR]);

//! Update a nav-to-body quaternion by the inverse of an angle increment in nav frame
void UpdateQuaternionNf(float Quat[N_QUAT], const float deltaN[NDIR]);

//! Change a quaternion such that that heading angle matches a specified value.
void SetHeadingInQuat(double Quat[N_QUAT], double NewHeading);

//! Change a quaternion such that that heading angle matches a specified value.
void SetHeadingInQuatf(float Quat[N_QUAT], float NewHeading);

//! Calculate the body-to-nav direction cosine matrix
void GetBodyToNavMat(double C[N_DCM], const double Quat[N_QUAT]);

//! Calculate the body-to-nav direction cosine matrix
void GetBodyToNavMatf(float C[N_DCM], const float Quat[N_QUAT]);

//! Calculate the nav-to-body direction cosine matrix
void GetNavToBodyMat(double C[N_DCM], const double Quat[N_QUAT]);

//! Calculate the nav-to-body direction cosine matrix
void GetNavToBodyMatf(float C[N_DCM], const float Quat[N_QUAT]);

//! Calculate the quaternion for nav-to-body frame transformation
void GetQuatFromBodyToNavMat(double Quat[N_QUAT], const double C[N_DCM]);

//! Calculate the quaternion for nav-to-body frame transformation
void GetQuatFromBodyToNavMatf(float Quat[N_QUAT], const float C[N_DCM]);

//! Calculate the quaternion for nav-to-body frame transformation
void GetQuatFromNavToBodyMat(double Quat[N_QUAT], const double C[N_DCM]);

//! Calculate the quaternion for nav-to-body frame transformation
void GetQuatFromNavToBodyMatf(float Quat[N_QUAT], const float C[N_DCM]);

//! Calculate the direction cosine matrix given a vector in two reference frames
void GetNavToBodyMatFromVec(double C[N_DCM], const double VecN[N3D], const double VecB[N3D]);

#ifdef __cplusplus
}
#endif

#endif

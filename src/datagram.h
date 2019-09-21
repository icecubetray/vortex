#ifndef __VORTEX_DATAGRAM_H
#define __VORTEX_DATAGRAM_H




#include "./types.h"




/*
** Datagram structure as sent by FDO.
** Currently v2.
*/
struct fdo_datagram {
	fdo_s32_t isRaceOn;

	fdo_u32_t timestampMs;

	fdo_f32_t engineMaxRpm;
	fdo_f32_t engineIdleRpm;
	fdo_f32_t engineCurrentRpm;

	fdo_f32_t accelerationX;
	fdo_f32_t accelerationY;
	fdo_f32_t accelerationZ;

	fdo_f32_t velocityX;
	fdo_f32_t velocityY;
	fdo_f32_t velocityZ;

	fdo_f32_t yaw;
	fdo_f32_t pitch;
	fdo_f32_t roll;

	fdo_f32_t suspensionCompressionFrontLeft;
	fdo_f32_t suspensionCompressionFrontRight;
	fdo_f32_t suspensionCompressionRearLeft;
	fdo_f32_t suspensionCompressionRearRight;

	fdo_f32_t slipRatioFrontLeft;
	fdo_f32_t slipRatioFrontRight;
	fdo_f32_t slipRatioRearLeft;
	fdo_f32_t slipRatioRearRight;

	fdo_f32_t rotationFrontLeft;
	fdo_f32_t rotationFrontRight;
	fdo_f32_t rotationRearLeft;
	fdo_f32_t rotationRearRight;

	fdo_s32_t onRumbleFrontLeft;
	fdo_s32_t onRumbleFrontRight;
	fdo_s32_t onRumbleRearLeft;
	fdo_s32_t onRumbleRearRight;

	fdo_f32_t puddleFrontLeft;
	fdo_f32_t puddleFrontRight;
	fdo_f32_t puddleRearLeft;
	fdo_f32_t puddleRearRight;

	fdo_f32_t rumbleFrontLeft;
	fdo_f32_t rumbleFrontRight;
	fdo_f32_t rumbleRearLeft;
	fdo_f32_t rumbleRearRight;

	fdo_f32_t slipAngleFrontLeft;
	fdo_f32_t slipAngleFrontRight;
	fdo_f32_t slipAngleRearLeft;
	fdo_f32_t slipAngleRearRight;

	fdo_f32_t slipCombinedFrontLeft;
	fdo_f32_t slipCombinedFrontRight;
	fdo_f32_t slipCombinedRearLeft;
	fdo_f32_t slipCombinedRearRight;

	fdo_f32_t suspensionTravelFrontLeft;
	fdo_f32_t suspensionTravelFrontRight;
	fdo_f32_t suspensionTravelRearLeft;
	fdo_f32_t suspensionTravelRearRight;

	fdo_s32_t carOrdinal;
	fdo_s32_t carClass;
	fdo_s32_t carPerformanceIndex;
	fdo_s32_t carDrivetrainType;
	fdo_s32_t carNumCylinders;

	/* Members added in v2. */
	fdo_f32_t positionX;
	fdo_f32_t positionY;
	fdo_f32_t positionZ;

	fdo_f32_t speed;
	fdo_f32_t power;
	fdo_f32_t torque;

	fdo_f32_t tireTemperatureFrontLeft;
	fdo_f32_t tireTemperatureFrontRight;
	fdo_f32_t tireTemperatureRearLeft;
	fdo_f32_t tireTemperatureRearRight;

	fdo_f32_t boost;
	fdo_f32_t fuel;
	fdo_f32_t distanceTraveled;
	fdo_f32_t bestLap;
	fdo_f32_t lastLap;
	fdo_f32_t currentLap;
	fdo_f32_t currentRaceTime;

	fdo_u16_t lapNumber;
	fdo_u8_t racePosition;
	fdo_u8_t acceleration;
	fdo_u8_t brake;
	fdo_u8_t clutch;
	fdo_u8_t handBrake;
	fdo_u8_t gear;
	fdo_u8_t steer;

	fdo_u8_t drivingLine;
	fdo_u8_t aiBrakeDifference;
};




#endif

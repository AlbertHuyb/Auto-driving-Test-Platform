/*
 * DataDef.h
 *
 *  Created on: 2017年6月22日
 *  Author: wyz
 *  Description: 
 *
 */

#ifndef DATADEF_H_
#define DATADEF_H_

#pragma pack(1)
// Define Set struct
#define	_SET_OF(type)					\
	struct {									\
		type **array;							\
		int count;	/* Meaningful size */	\
	}

typedef _SET_OF(void) a_anonymous_set_;
#define _A_SET_FROM_VOID(ptr)		((a_anonymous_set_ *)(ptr))

// Define GNSS information
typedef struct GNSSInfo{
	// Positioning time stamp in millisecond within 1 minute, 0-59999ms
	unsigned short pos_t;

	// Geodetic Coordinates
	double	longitude;	//longitude of device
	double	latitude;	//latitude of device
	double height;

	float	speed;		//Speed of device, in m/s
	float	heading;		//Direction of device, in degree, in GPRMC format
	float	accel;		//accelerate of device
	float	accelAngle;	//accelerate direction
}GNSSInfo_s;

// define vehicle trace
typedef _SET_OF(GNSSInfo_s) VehicleTrace_set;

// Size of object
typedef struct SizeInfo{
	float length;
	float width;
	float height;
}SizeInfo_s;

// Definition of tartget detected by vehicle
typedef struct Target{
	// ID of the PTC in this device
	// Given by this device
	unsigned short local_id;

	// Position of this PTC
	GNSSInfo_s pos;

	// Size of this PTC
	SizeInfo_s size;

	// 0 = Unknown, 1 = motor, 2 = non-motor, 3 = pedestrian
	unsigned char ptc_type;

	// angle of target from perspective of host vehicle
	double target_angle;

	// distance from host vehicle to target vehicle
	double target_distance;

	// Vehicle basic class
	unsigned char basic_vehicle_class;
}Target_s;

// One target that has a collision risk
typedef struct CollisionTarget{
	// Basic information of this target
	Target_s tgt;

	// time to collision. if not exist, ttc > 9000
	double ttc;
}CollisionTarget_s;

// Collision target result set
typedef _SET_OF(CollisionTarget_s) CollisionTarget_set;

// Position point
typedef struct PosPoint{
	double	longitude;	//longitude of device
	double	latitude;	//latitude of device
	double height;
}PosPoint_s;

// Different kind of speed limit
typedef struct MapSpeedLim{
	// speed limit Type
	/**
	0 = unknown, -- Speed limit type not available
	1 = maxSpeedInSchoolZone, -- Only sent when the limit is active
	2 = maxSpeedInSchoolZoneWhenChildrenArePresent, -- Sent at any time
	3 = maxSpeedInConstructionZone, -- Used for work zones, incident zones, etc.-- where a reduced speed is present
	4 = vehicleMinSpeed,
	5 = vehicleMaxSpeed, -- Regulatory speed limit for general traffic
	6 = vehicleNightMaxSpeed,
	7 = truckMinSpeed,
	8 = truckMaxSpeed,
	9 = truckNightMaxSpeed,
	10 = vehiclesWithTrailersMinSpeed,
	11 = vehiclesWithTrailersMaxSpeed,
	12 = vehiclesWithTrailersNightMaxSpeed,
	*/
	unsigned char speed_limit_type;

	// limit speed in m/s
	float speed;
}MapSpeedLim_s;

// Define the movement at intersections and the related traffic light phase
typedef struct MapMovement{
	// This movement is from link upstr_node->downstr_node to downstr_node->remote_node
	long long remote_region_node_id;

	// Phase ID
	unsigned char phase_id;
}MapMovement_s;

// Link information
typedef struct MapLink{
	// This link is from upstream_node to father_node
	long long upstr_region_node_id;

	// Link name
	char name[64];

	// Lane Width
	float lane_width;

	// Lane count
	unsigned char lane_count;

	// Default max speed limit, in m/s
	float default_max_speed_limit;

	// Speed limit list belongs to this link
	_SET_OF(MapSpeedLim_s) speed_lims;

	// point list of the center of this link from upstream to downstream
	_SET_OF(PosPoint_s) points;

	// Movements defined by link
	_SET_OF(MapMovement_s) movements;
}MapLink_s;

// Map node information
typedef struct MapNode{
	// ID of region this node belongs to
	//unsigned short region_id;
	// ID of this node in this region
	//unsigned short node_id;
	// combine 2 ids to long long like: regionid*1e5 + nodeid
	// max value is 6553565535
	long long region_node_id;

	// Node position
	PosPoint_s pos;

	// All the in-links from other node to this node
	_SET_OF(MapLink_s) in_links;
}MapNode_s;

// The whole map is formed by a set of nodes
typedef _SET_OF(MapNode_s) MapNode_set;

// Vehicle brake system
typedef struct BrakeSystem{
	// unavailable = 0; off = 1; on = 2
	unsigned char BrakePedalStatus;

//	BrakeAppliedStatus ::= BIT STRING {
//		unavailable (0), -- When set, the brake applied status is unavailable
//		leftFront (1), -- Left Front Active
//		leftRear (2), -- Left Rear Active
//		rightFront (3), -- Right Front Active
//		rightRear (4) -- Right Rear Active
//	} Bits counts from high to low
	unsigned char  BrakeAppliedStatus;

	// unavailable = 0; off = 1; on = 2; engaged = 3
	unsigned char  TractionControlStatus;

	// unavailable = 0; off = 1; on = 2; engaged = 3
	unsigned char  AntiLockBrakeStatus;

	// unavailable = 0; off = 1; on = 2; engaged = 3
	unsigned char  StabilityControlStatus;

	// unavailable = 0; off = 1; on = 2
	unsigned char  BrakeBoostApplied;

	// unavailable = 0; off = 1; on = 2; engaged = 3
	unsigned char  AuxiliaryBrakeStatus;
}BrakeSystem_s;

// Vehicle state
typedef struct VehicleState{
	// neutral (0),park (1),forwardGears (2),reverseGears (3),unavailable (7)
	unsigned char transmission_state;
	// unavailable = 0; CANOBD = 1
	unsigned char transmission_state_source;

	// steering wheel angle, + means to right, - means to left. in degree
	float steering_wheel_angle;
	// unavailable = 0; CANOBD = 1
	unsigned char steering_wheel_angle_source;

	// vehicle brake system
	BrakeSystem_s brake_system;
	// unavailable = 0; CANOBD = 1
	unsigned char brake_system_source;

//	VehicleEventFlags ::= BIT STRING {
//		eventHazardLights (0),
//		eventStopLineViolation (1), -- Intersection Violation
//		eventABSactivated (2),
//		eventTractionControlLoss (3),
//		eventStabilityControlactivated (4),
//		eventHazardousMaterials (5),
//		eventReserved1 (6),
//		eventHardBraking (7),
//		eventLightsChanged (8),
//		eventWipersChanged (9),
//		eventFlatTire (10),
//		eventDisabledVehicle (11), -- The DisabledVehicle DF may also be sent
//		eventAirBagDeployment (12)
//		} Bits counts from high to low
	unsigned char vehicle_event_flag[2];
	// unavailable = 0; CANOBD = 1; GNSS = 2
	unsigned char vehicle_event_flag_source;

//	ExteriorLights ::= BIT STRING {
//		-- All lights off is indicated by no bits set
//		lowBeamHeadlightsOn (0),
//		highBeamHeadlightsOn (1),
//		leftTurnSignalOn (2),
//		rightTurnSignalOn (3),
//		hazardSignalOn (4),
//		automaticLightControlOn (5),
//		daytimeRunningLightsOn (6),
//		fogLightOn (7),
//		parkingLightsOn (8)
//		} Bits counts from high to low
	unsigned char exterior_lights[2];
	// unavailable = 0; CANOBD = 1; GNSS = 2
	unsigned char exterior_lights_source;
}VehicleState_s;

// OBU
typedef struct OBUInfo{
	// ID of the PTC in this device
	// Given by this device
	unsigned short local_id;

	// Position of this PTC
	GNSSInfo_s pos;

	// Size of this PTC
	SizeInfo_s size;

	// Unique ID of this device
	char ID[16];

	// Vehice plate number
	char plateNo[16];

	// Vehicle basic class
	unsigned char basic_vehicle_class;

	// Vehicle status
	VehicleState_s status;

}OBUInfo_s;

// A Set of OBU
typedef _SET_OF(OBUInfo_s) OBU_set;

// PTC
typedef struct PTCInfo{
	// ID of the PTC in this device
	// Given by this device
	unsigned short local_id;

	// Position of this PTC
	GNSSInfo_s pos;

	// Size of this PTC
	SizeInfo_s size;

	// Participant type
	// 0 = Unknown, 1 = motor, 2 = non-motor, 3 = pedestrian
	unsigned char ptc_type;

	// temporary id given by RSU
	unsigned short ptc_given_temp_Id;

	// temporary id given by detector
	unsigned short ptc_given_temp_Id_detector;

	// Detector ID
	char detector_ID[16];

	// Source detector type of this PTC data
	// 0 = unknown, 1 = selfinfo, 2 = v2x, 3 = video, 4 = microwaveRadar, 5 = loop
	unsigned char source_detect_type;

	// Device ID of the data source
	char source_device_ID[16];

	// Unique ID of this device, if exists
	char ptc_device_ID[16];

	// Vehice plate number, if exists
	char ptc_plateNo[16];

	// Vehicle basic class
	unsigned char basic_vehicle_class;
}PTCInfo_s;

// A Set of PTC
typedef _SET_OF(PTCInfo_s) PTC_set;

// RSI result
typedef struct RSIRes{
	// RSI type
	/**
	-- Text message warning = 0

	-- Traffic signs according to China GB5768
	-- Danger = 37
	-- Rockfall = 15
	-- SharpTurn = 2
	-- UnderConstruction = 38
	-- Slippy = 17
	-- Tunnel = 21
	*/
	unsigned short type;

	// Text traffic incident description
	char description[256];
}RSIRes_s;

// A Set of RSIRes
typedef _SET_OF(RSIRes_s) RSIRes_set;

// RSI Information
typedef struct RSIInfo{
	// temporary id given by who detected the ptc
	unsigned short given_temp_Id;

	// Device ID of the data source
	char source_device_ID[16];

	// time stamp in minute of the year
	unsigned int minute_of_the_year;

	// RSI type
	/**
	-- Text message warning = 0

	-- Traffic signs according to China GB5768
	-- Danger = 37
	-- Rockfall = 15
	-- SharpTurn = 2
	-- UnderConstruction = 38
	-- Slippy = 17
	-- Tunnel = 21
	*/
	unsigned short type;

	// Text traffic incident description
	char description[256];

	// priority of this RSI message
	/**
	-- The lower five bits are reserved and shall be set to zero
	-- Value from B00000000 to B11100000 represents the lowest to the highest level
	*/
	unsigned char priority;

	// The biggest distance away from the alert path within which the warning is active.
	float radius;

	// RSI position
	PosPoint_s pos;

	// point list of the RSI path
	_SET_OF(PosPoint_s) path;

}RSIInfo_s;

// A set of RSI information
typedef _SET_OF(RSIInfo_s) RSI_set;

// RSU
typedef struct RSUInfo{
	// ID of the PTC in this device
	// Given by this device
	unsigned short local_id;

	// Position of this PTC
	GNSSInfo_s pos;

	// Unique ID of this device
	char ID[16];
}RSUInfo_s;

// A Set of RSU
typedef _SET_OF(RSUInfo_s) RSU_set;

// Different kind of target result set, like Dangerous targets, target need yield...
typedef _SET_OF(Target_s) Target_set;

// Traffic light phase result
typedef struct TLPhaseRes{
	// turning angle in rad. Straight forward is 0, turn left < 0, turn right > 0, U turn is -PI
	float turnAngle;

	// Follow the definition of light_state in LMPhaseStateBase.h
	unsigned char light_state;

	// Time left, in second
	unsigned char time_left;

	// recommended drive speed, in m/s
	double rec_speed_ceil;
	double rec_speed_floor;
}TLPhaseRes_s;

// Set of Traffic light phase result
typedef _SET_OF(TLPhaseRes_s) TLPhaseRes_set;

// State info of one traffic light phase
typedef struct TLPhaseState{
	/**
		unavailable (0),
		-- This state is used for unknown or error
		dark (1),
		-- The signal head is dark (unlit)

		-- Reds
		stop-Then-Proceed (2),
		-- 'flashing red'
		-- Driver Action:
		-- Stop vehicle at stop line.
		-- Do not proceed unless it is safe.
		-- Note that the right to proceed either right or left when
		-- it is safe may be contained in the lane description to
		-- handle what is called a 'right on red'
		stop-And-Remain (3),
		-- 'red light'
		-- Driver Action:
		-- Stop vehicle at stop line.
		-- Do not proceed.
		-- Note that the right to proceed either right or left when
		-- it is safe may be contained in the lane description to
		-- handle what is called a 'right on red'

		-- Greens
		pre-Movement (4),
		-- Not used in the US, red+yellow partly in EU, VMS partly in China
		-- Driver Action:
		-- Stop vehicle.
		-- Prepare to proceed (pending green)
		-- (Prepare for transition to green/go)
		permissive-Movement-Allowed (5),
		-- 'permissive green'
		-- Driver Action:
		-- Proceed with caution,
		-- must yield to all conflicting traffic
		-- Conflicting traffic may be present
		-- in the intersection conflict area
		protected-Movement-Allowed (6),
		-- 'protected green'
		-- Driver Action:
		-- Proceed, tossing caution to the wind,
		-- in indicated (allowed) direction.

		-- Yellows / Ambers
		-- The vehicle is not allowed to cross the stop bar if it is possible
		-- to stop without danger.
		intersection-clearance (7),
		-- 'yellow light'
		-- Driver Action:
		-- Prepare to stop.
		-- Proceed if unable to stop,
		-- in indicated direction (to connected lane)
		-- Clear Intersection.

		caution-Conflicting-Traffic (8)
		-- 'flashing yellow'
		-- Often used for extended periods of time
		-- Driver Action:
		-- Proceed with caution,
		-- Conflicting traffic may be present
		-- in the intersection conflict area
		*/
		unsigned char light_state;

		float start_time;
	//	-- When this phase state started,
	//	-- if already started, the value is 0
		// -1 = unabailable

		float minEndTime;
	//	-- Expected shortest end time
	//	-- if already started, the value is the min left time from now
	//	-- if not started, the value means the min length of this phase
		// -1 = unabailable

		float maxEndTime;
	//	-- Expected longest end time
	//	-- if already started, the value is the max left time from now
	//	-- if not started, the value means the max length of this phase
		// -1 = unabailable

		float likelyEndTime;
	//	-- Best predicted value based on other data
	//	-- if already started, the value is the likely left time from now
	//	-- if not started, the value means the likely length of this phase
		// -1 = unabailable

		unsigned char timeConfidenceIn200;
	//	-- Confidence of likelyTime

		float nextTime;
	//	-- A rough estimate of time when
	//	-- this phase state may next occur again
	//	-- after the above endTime.
	//	-- used to support various ECO driving power management needs.
	//	-- If already started, this value is recommended to be delivered.
		// -1 = unabailable

		float nextDuration;
	//	-- A rough estimate of the time length of this phase state
	//	-- when it may next occur again after the above endTime.
	//	-- used to support various ECO driving power management needs.
	//	-- If already started, this value is recommended to be delivered.
}TLPhaseState_s;

// One phase of traffic light
typedef struct TLPhase{
	// Phase ID
	unsigned char phase_id;

	// Phase state number
	unsigned char phase_state_count;

	// phase states of this phase
	_SET_OF(TLPhaseState_s) states;
}TLPhase_s;

// One traffic light information
typedef struct TrafficLight{
	// Signalized intersection local id
	long long intersection_region_node_id;

	/**
	manualControlIsEnabled (0),
	-- Timing reported is per programmed values, etc. but person
	-- at cabinet can manually request that certain intervals are
	-- terminated early (e.g. green).
	stopTimeIsActivated (1),
	-- And all counting/timing has stopped.
	failureFlash (2),
	-- Above to be used for any detected hardware failures,
	-- e.g. conflict monitor as well as for police flash
	preemptIsActive (3),
	signalPriorityIsActive (4),
	-- Additional states
	fixedTimeOperation (5),
	-- Schedule of signals is based on time only
	-- (i.e. the state can be calculated)
	trafficDependentOperation (6),
	-- Operation is based on different levels of traffic parameters
	-- (requests, duration of gaps or more complex parameters)
	standbyOperation (7),
	-- Controller: partially switched off or partially amber flashing
	failureMode (8),
	-- Controller has a problem or failure in operation
	off (9),
	-- Controller is switched off
	-- Related to MAP and SPAT bindings
	recentMAPmessageUpdate (10),
	-- Map revision with content changes
	recentChangeInMAPassignedLanesIDsUsed (11),
	-- Change in MAP's assigned lanes used (lane changes)
	-- Changes in the active lane list description
	noValidMAPisAvailableAtThisTime (12),
	-- MAP (and various lanes indexes) not available
	noValidSPATisAvailableAtThisTime (13)
	-- SPAT system is not working at this time
	-- Bits 14,15 reserved at this time and shall be zero
	*/
	unsigned char tl_status[2];

	// time stamp in ms
	unsigned short time_stamp;

	// phase number
	unsigned char phase_count;

	// phases of this traffic light
	_SET_OF(TLPhase_s) phs;
}TrafficLight_s;

// A Set of traffic light
typedef _SET_OF(TrafficLight_s) TrafficLight_set;



#endif /* DATADEF_H_ */

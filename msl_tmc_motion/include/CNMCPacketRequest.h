/*
 * CNMCPacketRequest.h
 *
 *  Created on: Sep 30, 2015
 *      Author: cnpaul
 */

#ifndef CNC_MSLDRIVER_MSL_TMC_MOTION_INCLUDE_CNMCPACKETREQUEST_H_
#define CNC_MSLDRIVER_MSL_TMC_MOTION_INCLUDE_CNMCPACKETREQUEST_H_


#include <vector>
#include <memory>
#include <iostream>


namespace msl_driver
{
	typedef unsigned char byte;
	typedef signed char sbyte;

	class CNMCPacket
	{
	public:
		CNMCPacket();
		virtual ~CNMCPacket();
		//Horrible shit I know that, but it works ;-)
		struct VAROBJECT
		{
		    enum o_t { UNSIGNEDCHAR, INT, SHORT, SIGNEDCHAR } objectType;
		    union
		    {
		    	unsigned char ucValue[4];
		    	double dblValue;
		    	signed char sByte;
		    	short shValue;
		    } value;
		};

		enum CommandGroup : byte
		{
			Configure			= 0x50,
			ConfigureResponse		= 0x51,
			Control				= 0x52,
			Request				= 0x54,
			RequestResponse			= 0x55,
			CtrlConfigure			= 0x56,
			CtrlConfigureResponse		= 0x57,
			ErrorResponse			= 0x59
		};
		enum ConfigureCmd : byte
		{
			Mode			=	0x10,
			CycleTime		=	0x11,
			CurrentLimiterOn	=	0x12, //
			CommandTimeOut		=	0x13,
			MaxCurrent		=	0x20, //
			MaxRPM			=	0x21, //
			NominalCurrent		=	0x22, //
			NominalRPM		=	0x23, //
			GearRatio		=	0x30, // ubyte num, ubyte denum
			WheelRadius		=	0x31, //
			EncoderTicksPerRot	=	0x33, // ushort value
			Torque			=	0x34, //
			MotorDirection		=	0x35, //
			MotorMaxTemp		=	0x40, //
			MotorNominalTemp	=	0x41, //
			MotorEnvTemp		=	0x42, //
			MotorWindingTemp	=	0x43, //
			MotorWindingGN		=	0x44, //
			MotorWindingGT		=	0x45, //
			MotorChassisTemp	= 	0x46, //
			MotorChassisGN		= 	0x47, //
			MotorChassisGZ		= 	0x48, //
			SaveConfig		=	0x50, //
			LoadConfig		=	0x51, //
			IOPort			=	0x60, //
			RobotRadius		=	0x70, //
			WheelAngle		=	0x71, //
			ToggleOdoLog		=	0x72,
			SetLogMode		=	0x73  //sbyte type, sbyte pos
		};
		enum ControlCmd : byte
		{
			SetAllPWM			= 0x10, // X (int)pwm1, (int)pwm2, (int)pwm3, (byte)sreq
			SetPWM				= 0x11, // X (byte)motor, (int)pwm, (byte)sreq
			SetAllRPM			= 0x20, // (int)rpm1, (int)rpm2, (int)rpm3, (byte)sreq
			SetRPM				= 0x21, // X (byte)motor, (int)rpm, (byte)sreq
			SetMotionVector			= 0x30, // (int)angle, (int) velo, (int) rotation
			ResetHard			= 0x40,
			ResetGently			= 0x41
		};

		enum RequestCmd : byte
		{
			MotorRPM			= 0x10, // (s16bit)rpm1, (s16bit)rpm2, (s16bit)rpm3
			MotorPWM			= 0x11, // (s16bit)pwm1, (s16bit)pwm2, (s16bit)pwm3]
			MotorVoltage			= 0x12, // xxx????
			MotorCurrent			= 0x13, // s.o.
			MotorTemp			= 0x14, //
			MotorTorque			= 0x15, //
			MotorPower			= 0x16, //
			EncoderTicksRel			= 0x20, // (s32bit)ticks, (s32bit)ticks, (s32bit)ticks3
			EncoderTicksAbs			= 0x21, // n.A.
			BatteryVoltage			= 0x30, // (u8bit)Voltage
			ReadDigitalInputs		= 0x40, // ?
			SetDigitalOutputs		= 0x41, // ?
			ReadAnalogInputs		= 0x42, // ?

			MotionVector			= 0x60, // int, int, int MotionVector
			AverageSleepTime		= 0x61,
			PathVector			= 0x62, //<--(32bitfloat)angle(32bitfloat)trans(32bitfloat)rotation  the one you are looking for
			ReadOdoLog			= 0x63,
			PathAndPWM			= 0x64

		};

		enum CtrlConfigureCmd : byte
		{
			PIDKp				= 0x10, //(int)value
			PIDKi				= 0x11, //(int)value
			PIDb				= 0x12, //(int)value
			PIDKd				= 0x13, //(int)value
			PIDKdi				= 0x14, //(int)value
			Commit				= 0x15,
			DeadBand			= 0x2A, // (int)value
			MaxErrorInt			= 0x2B,
			LinearFactor			= 0x2C,
			SmoothFactor			= 0x2D,
			MaxRotationAccel		= 0x2E,

			RotationErrorW			= 0x31, //(int)value
			RotationErrorAccelW		= 0x32, //(int)value
			RotationErrorVeloW		= 0x33, //(int)value

			LowerAccelBound 		= 0x40, //(int) value
			HigherAccelBound		= 0x41, //(int) value

			FailSafeValues			= 0x50,

			CurrentErrorBound		= 0x60,//short, centi Ampere
			CurrentKp			= 0x61,
			CurrentKi			= 0x62,
			CurrentKd			= 0x63,

			MaxAcceleration			= 0x70,
			MaxDecceleration		= 0x71,
			MaxRotForce			= 0x72
		};

		enum ErrorCmd:byte
		{
			UnknownError			= 0x01, // "Err"
			UnknownCmd			= 0x02,
			ParamError			= 0x10, // "Err Param"
			OutOfRange			= 0x11, // "Out of Range"
			CycleOverTime			= 0x20
		};
		const byte QUOTE = 0x84;
		const byte START_HEADER = 0x81;
		const byte END_HEADER = 0x82;
//		public const byte MAX_COUNTER = 0x80;

		static const int CMD_GROUP_POS;
		static const int CMD_POS;
		static const int DATA_POS;

		std::shared_ptr<std::vector<byte>> getBytes();
		VAROBJECT convertByteToShort(byte data[], int start);
		VAROBJECT convertShortToByte(short data);
		VAROBJECT convertByteToInt(byte data[], int start);
		VAROBJECT convertIntToByte(int data);

		bool isExpectedResponse(CNMCPacket response);
		VAROBJECT giveVarobject(unsigned char uc ,VAROBJECT::o_t type);
		VAROBJECT giveVarobject(int uc ,VAROBJECT::o_t type);
		VAROBJECT giveVarobject(short uc ,VAROBJECT::o_t type);
		VAROBJECT giveVarobject(signed char uc ,VAROBJECT::o_t type);

		void add(byte b);
		void add(VAROBJECT bytes);

		static CNMCPacket getInstance(byte raw[], int size);
		std::shared_ptr<std::vector<byte> > data;
		std::shared_ptr<std::vector<VAROBJECT> > values;
		byte cmd = 0x00;

	protected:
		byte cmdgrp = 0x00;
		byte crc = 0x00;

		void needQuotes(byte b, std::shared_ptr<std::vector<byte>> list);

	};
	class CNMCPacketConfigure : public CNMCPacket
	{
		public:
			CNMCPacketConfigure();
			virtual ~CNMCPacketConfigure();
			CNMCPacketConfigure(byte raw[]);
			void setData(ConfigureCmd cmd, byte val);
			void setData(ConfigureCmd cmd, short val);
			void setData(ConfigureCmd cmd, std::shared_ptr<std::vector<byte>> vals);
			void setData(ConfigureCmd cmd, std::shared_ptr<std::vector<sbyte>> vals);

	};
	class CNMCPacketControl : public CNMCPacket
	{
		public:
			CNMCPacketControl();
			virtual ~CNMCPacketControl();
			CNMCPacketControl(byte raw[]);
			void setData(ControlCmd cmd, short rpm1, short rpm2, short rpm3);
	};

	class CNMCPacketRequest : public CNMCPacket
	{
		public:
			CNMCPacketRequest();
			virtual ~CNMCPacketRequest();
			CNMCPacketRequest(byte raw[]);
			void setData(RequestCmd cmd);
			void setData(RequestCmd cmd, short val);
	};
	class CNMCPacketCtrlConfigureResponse : public CNMCPacket
	{
		public:
			CNMCPacketCtrlConfigureResponse();
			virtual ~CNMCPacketCtrlConfigureResponse();
			CNMCPacketCtrlConfigureResponse(byte raw[]);
	};
	class CNMCPacketCtrlConfigure : public CNMCPacket
	{
		public:
			CNMCPacketCtrlConfigure();
			virtual ~CNMCPacketCtrlConfigure();
			CNMCPacketCtrlConfigure(byte raw[]);
			void setData(CtrlConfigureCmd cmd);
			void setData(CtrlConfigureCmd cmd, short val);
			void setData(CtrlConfigureCmd cmdl, int val);
			void setData(CtrlConfigureCmd cmd, std::shared_ptr<std::vector<short>> vals);
	};

	class CNMCPacketError : public CNMCPacket
	{
		public:
			CNMCPacketError();
			virtual ~CNMCPacketError();
			CNMCPacketError(byte raw[]);
			std::string toString();

	};
	class CNMCPacketRequestResponse : public CNMCPacket
	{
		public:
			CNMCPacketRequestResponse();
			virtual ~CNMCPacketRequestResponse();
			CNMCPacketRequestResponse(byte raw[], int size);
	};
	class CNMCPacketConfigureResponse : public CNMCPacket
	{
		public:
			CNMCPacketConfigureResponse();
			virtual ~CNMCPacketConfigureResponse();
			CNMCPacketConfigureResponse(byte raw[], int size);
	};
} /* namespace msl_driver */

#endif /* CNC_MSLDRIVER_MSL_TMC_MOTION_INCLUDE_CNMCPACKETREQUEST_H_ */
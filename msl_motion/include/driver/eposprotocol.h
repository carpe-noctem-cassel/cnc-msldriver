#ifndef EPOSPROTOCOL_H
#define EPOSPROTPCOL_H 1


#define CAN_MSG_ID_MASK         0x780
#define CAN_NODE_ID_MASK        0x7F

//CANIDs:
#define CAN_ID_NMT          0x000

    #define CAN_NMT_SET_OPERATIONAL 0x01
    #define CAN_NMT_RESET_COMM      0x82
    #define CAN_NMT_RESET_NODE      0x81

//----------SDO Communication:
#define CAN_ID_SDO_WRITE            0x600
#define CAN_ID_SDO_READ             0x600
#define CAN_ID_SDO_RESPONSE         0x580

#define CAN_CMD_UPLOAD_REQUEST      0x40
#define CAN_CMD_UPLOAD_RESPONSE     0x40
#define CAN_CMD_DOWNLOAD_REQUEST    0x20
#define CAN_CMD_DOWNLOAD_RESPONSE   0x60
#define CAN_CMD_SDO_TERMINATION     0x80

#define CAN_SDO_CONTROLWORD         0x6040
#define CAN_SDO_STATUSWORD          0x6041
#define CAN_SDO_SET_OPMODE          0x6060
#define CAN_SDO_GET_OPMODE          0x6061
#define CAN_SDO_ERRORWORD           0x2320

//PDOs:
#define CAN_PDO_1_Tx                0x180
#define CAN_PDO_1_Rx                0x200
#define CAN_PDO_2_Tx                0x280

//---------------EMCY

#define CAN_ID_ERROR                0x80

#define CAN_ID_SYNC                 0x80

//NMT STATUS stuff
#define CAN_ID_NMT_STATUS           0x700



///CAN CMDS:

#define EPOS_SET_PROFILE            0x6060

#define EPOS_MAX_VELO               0x607F
#define EPOS_MAX_ACCEL              0x6083
#define EPOS_MAX_DECCEL             0x6084
#define EPOS_SET_VELO               0x60FF

#define EPOS_SET_CONTROL            0x6040

//GETTERS:

#define EPOS_GET_STATUS             0x6041
#define EPOS_GET_VELO               0x2028
#define EPOS_GET_CURRENT            0x6078

//Adresses for mapping pdos:
#define EPOS_ADDR_VELOCITY_ACTUAL   0x606c0020 //32bit value
#define EPOS_ADDR_CURRENT_ACTUAL    0x60780010 //16bit
#define EPOS_ADDR_STATUS            0x60410010 //16bit
#define EPOS_ADDR_VELOCITY_AVG      0x20270020 //32bit

#define EPOS_ADDR_TARGET_VELOCITY   0x60FF0020 //32bit
#define EPOS_ADDR_CONTROLWORD       0x60400010 //32bit





//VALUES:
#define EPOS_PROFILE_VELOCITY       0x03

#define EPOS_MODE_QUICKSTOP         0x000B
#define EPOS_MODE_SHUTDOWN          0x0006
#define EPOS_MODE_SWITCHON          0x000F
#define EPOS_MODE_RESET_FAULTS      0x0080


#define EPOS_NMT_STATUS_START       0x01
#define EPOS_NMT_STATUS_STOP        0x02
#define EPOS_NMT_STATUS_PREOP       0x80
#define EPOS_NMT_STATUS_RESET       0x81
#define EPOS_NMT_STATUS_RESETCOM    0x82


#endif

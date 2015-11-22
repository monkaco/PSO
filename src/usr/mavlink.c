#include "mavlink_bridge.h"


void create_message(uint8_t system_id, uint8_t component_id, uint8_t chan, uint16_t* msg, const mavlink_pso_data_t* pso_data){
	uint16_t checksum;

	memcpy(&msg[3], pso_data, sizeof(mavlink_pso_data_t));

	msg[2] |= MAVLINK_MSG_ID_PSO_DATA << 8;

	msg[0] |= MAVLINK_STX;
	msg[0] |= MAVLINK_MSG_ID_PSO_DATA_LEN << 8;
	msg[1] |= system_id << 8;
	msg[2] |= component_id;
	msg[1] |= mavlink_get_channel_status(chan)->current_tx_seq;

	mavlink_get_channel_status(chan)->current_tx_seq = (mavlink_get_channel_status(chan)->current_tx_seq+1) & 0xff;

	checksum = create_checksum(&msg[0], MAVLINK_MSG_ID_PSO_DATA_LEN + MAVLINK_CORE_HEADER_LEN);
	accumulate_checksum(MAVLINK_MSG_ID_PSO_DATA_CRC, &checksum);

	msg[3 + MAVLINK_MSG_ID_PSO_DATA_LEN/2] = checksum;
}

uint16_t create_checksum(uint16_t* msg, uint16_t length){
	int i;
    uint16_t crcTmp;
    crc_init(&crcTmp);
    i=1;
    while (length--) {
    	if(i%2==0){
    		accumulate_checksum(msg[i/2] & 0xff, &crcTmp);
    	}else{
    		accumulate_checksum((msg[i/2] >> 8) & 0xff, &crcTmp);
    	}
        ++i;
    }

    return crcTmp;
}

void accumulate_checksum(uint8_t data, uint16_t *crcAccum){
    /*Accumulate one byte of data into the CRC*/
    uint8_t tmp;

    tmp = data ^ (*crcAccum &0xff);
    tmp ^= (tmp<<4) &0xff;
    *crcAccum = (*crcAccum>>8) ^ (tmp<<8) ^ (tmp <<3) ^ (tmp>>4);
}

uint8_t parse_message(uint16_t *msg, mavlink_pso_command_t *command){
	uint16_t checksum = create_checksum(msg, MAVLINK_MSG_ID_PSO_COMMAND_LEN + MAVLINK_CORE_HEADER_LEN);
	accumulate_checksum(MAVLINK_MSG_ID_PSO_COMMAND_CRC, &checksum);
	if(checksum == msg[3 + MAVLINK_MSG_ID_PSO_COMMAND_LEN/2]){
		memcpy(command, &msg[3], sizeof(mavlink_pso_command_t));
		return 1;
	}

	return 0;
}

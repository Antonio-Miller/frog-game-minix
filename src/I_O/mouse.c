#include <lcom/lcf.h>

#include "mouse.h"
#include "video_card.h"

int hook_id_mouse = MOUSE_IRQ;
uint8_t pack = 0;
bool valid_packet = 0;

struct packet mouse_packet;
static uint8_t packet_bytes[3];
static uint8_t packet_index = 0;
int mouse_x = 100, mouse_y = 100; // posição inicial do cursor


int (mouse_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id_mouse;
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != OK)
		return -1;

    return 0;
}

int (mouse_unsubscribe_int)() {
    if(sys_irqrmpolicy(&hook_id_mouse) != OK)
		return -1;
    return 0;
}

int (mouse_read_status)(uint8_t *st) {
    if(util_sys_inb(KBC_ST_REG, st))
        return -1;
    return 0;
}

int (mouse_read_ack)(uint8_t *ack){
	uint8_t st;
	int tries = 0;
	while(tries < MAX_WAIT){
		mouse_read_status(&st);
		tries++;
		if((st & OBF) && (st & AUX)){
			util_sys_inb(KBC_OUT_BUF, ack);
			return 0;
		}
		tickdelay(DELAY_US);
	}
	return -1;
}

void (mouse_ih)() {
    uint8_t st;
    mouse_read_status(&st);
    if ((st & OBF) && (st & AUX)) {
        util_sys_inb(KBC_OUT_BUF, &pack);
		if((packet_index || (pack & FIRST_BYTE_CHECK))  && !(st & (PARITY | TIMEOUT))) {
			valid_packet = 1;
			packet_bytes[packet_index++] = pack;
			if (packet_index == 3) {
				if(valid_packet) {
					// Preencher o struct packet
					mouse_packet.bytes[0] = packet_bytes[0];
					mouse_packet.bytes[1] = packet_bytes[1];
					mouse_packet.bytes[2] = packet_bytes[2];
					mouse_packet.lb = packet_bytes[0] & BIT(0);
					mouse_packet.rb = packet_bytes[0] & BIT(1);
					mouse_packet.mb = packet_bytes[0] & BIT(2);
					mouse_packet.x_ov = packet_bytes[0] & BIT(6);
					mouse_packet.y_ov = packet_bytes[0] & BIT(7);

					// Calcular delta_x e delta_y
					mouse_packet.delta_x = (packet_bytes[0] & BIT(4)) ? (packet_bytes[1] | 0xFF00) : packet_bytes[1];
					mouse_packet.delta_y = (packet_bytes[0] & BIT(5)) ? (packet_bytes[2] | 0xFF00) : packet_bytes[2];

					// Atualizar posição do cursor
					mouse_x += mouse_packet.delta_x;
					mouse_y -= mouse_packet.delta_y; // y cresce para baixo no ecrã

					// Limitar aos limites do ecrã
					if (mouse_x < 0) mouse_x = 0;
					if (mouse_y < 0) mouse_y = 0;
					if (mouse_x > (int)get_h_res()-1) mouse_x = (int)get_h_res()-1;
					if (mouse_y > (int)get_v_res()-1) mouse_y = (int)get_v_res()-1;
				}
				packet_index = 0;
				valid_packet = 1;
			}
		}
		else {
			valid_packet = 0;
			packet_index = 0;
		}
	}
	else {
		valid_packet = 0;
		packet_index = 0;
	}
}

void (mouse_reset_packet)() {
	mouse_packet.bytes[0] = 0;
	mouse_packet.bytes[1] = 0;
	mouse_packet.bytes[2] = 0;
	mouse_packet.lb = 0;
	mouse_packet.rb = 0;
	mouse_packet.mb = 0;
	mouse_packet.x_ov = 0;
	mouse_packet.y_ov = 0;
	mouse_packet.delta_x = 0;
	mouse_packet.delta_y = 0;
}

int (mouse_read_cmd_byte)(uint8_t *cmd){
	uint8_t st;
	int tries = 0;
	while(tries < MAX_WAIT){
		mouse_read_status(&st);
		tries++;
		if(!(st & IBF)){
			sys_outb(KBC_CMD_REG, WRT_BYTE_MOUSE);
			uint8_t ack;
			mouse_read_ack(&ack);
			if(ack != SUCCESS_CMD_MOUSE)
				return -1;
			int tries_s = 0;
			while(tries_s < MAX_WAIT){
				mouse_read_status(&st);
				tries_s++;
				if(!(st & IBF)){
					sys_outb(KBC_CMD_REG, RD_CMD);
					mouse_read_ack(&ack);
					if(ack != SUCCESS_CMD_MOUSE)
						return -1;
					int tries_sec = 0;
					while(tries_sec < MAX_WAIT){
						tries_sec++;
						mouse_read_status(&st);
						if((st & OBF) && (st & AUX)){
							util_sys_inb(KBC_OUT_BUF, cmd);
							break;
						}
						tickdelay(DELAY_US);
					}
					break;
				}
				tickdelay(DELAY_US);
			}
		}
		tickdelay(DELAY_US);
	}
	return 0;
}

int (mouse_write_cmd_byte)(uint8_t cmd){
	uint8_t st;
	int tries = 0;
	while(tries < MAX_WAIT){
		mouse_read_status(&st);
		tries++;
		if(!(st & IBF)){
			sys_outb(KBC_CMD_REG, WRT_BYTE_MOUSE);
			uint8_t ack;
			mouse_read_ack(&ack);
			if(ack != SUCCESS_CMD_MOUSE)
				return -1;
			int tries_s = 0;
			while(tries < MAX_WAIT){
				mouse_read_status(&st);
				tries_s++;
				if(!(st & IBF)){
					sys_outb(KBC_CMD_REG, WT_CMD);
					mouse_read_ack(&ack);
					if(ack != SUCCESS_CMD_MOUSE)
						return -1;
					int tries_sec = 0;
					while(tries_sec < MAX_WAIT){
						mouse_read_status(&st);
						tries_sec++;
						if(!(st & IBF)){
							sys_outb(KBC_W_CMD_BYTE, cmd);
							mouse_read_ack(&ack);
							if(ack != SUCCESS_CMD_MOUSE)
								return -1;
							return 0;
						}
						tickdelay(DELAY_US);
					}
					break;
				}
				tickdelay(DELAY_US);
			}
		}
		tickdelay(DELAY_US);
	}
	return -1;
}


int (m_enable_data_reporting)(){
	return mouse_send_command(ENABLE_DATA_REP);
}

int (m_disable_data_reporting)(){
	return mouse_send_command(DISABLE_DATA_REP);
}

int (mouse_send_command)(uint8_t cmd){
	uint8_t st;
	int tries = 0;
	while(tries < MAX_WAIT){
		mouse_read_status(&st);
		tries++;
		if(!(st & IBF)){
			sys_outb(KBC_CMD_REG, WRT_BYTE_MOUSE);
			int tries_s = 0;
			while(tries < MAX_WAIT){
				mouse_read_status(&st);
				tries_s++;
				if(!(st & IBF)){
					uint8_t ack;
					sys_outb(KBC_W_CMD_BYTE, cmd);
					mouse_read_ack(&ack);
					if(ack != SUCCESS_CMD_MOUSE){
						continue;
					}
					return 0;
				}
				tickdelay(DELAY_US);
			}
		}
		tickdelay(DELAY_US);
	}
	return -1;
}

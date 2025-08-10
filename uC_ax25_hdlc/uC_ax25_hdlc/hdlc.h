/*
 * hdlc.h
 *
 * Created: 26.07.2025 20:50:58
 * Modified: 3/8/2025
  *  Author: DL8MCG
 */ 

#ifndef HDLC_H_
#define HDLC_H_

#define AX25_ADDR_LEN 7
#define MAX_CALLSIGN_LEN 7
#define FLAG 0x7E
#define MAX_FRAME_LEN 512
#define MAX_PAYLOAD_LEN 256

// AX.25 Frame Struktur
typedef struct {
	uint8_t type;                           // AX.25 Version (0x01 = V2)
	char dest_call[MAX_CALLSIGN_LEN];       // Ziel-Rufzeichen
	uint8_t dest_ssid;                      // Ziel-SSID
	char src_call[MAX_CALLSIGN_LEN];        // Quell-Rufzeichen
	uint8_t src_ssid;                       // Quell-SSID
	uint8_t control;                        // Control-Feld
	uint8_t pid;                            // Protocol ID
	char payload[MAX_PAYLOAD_LEN];          // Nutzdaten
	int payload_len;                        // Länge der Nutzdaten
} AX25_Frame_t;

// HDLC Frame Struktur
typedef struct {
	uint8_t data[MAX_FRAME_LEN];           // Encoded HDLC Daten
	int length;                            // Länge des HDLC Frames
} HDLC_Frame_t;

void encode_callsign(uint8_t type, uint8_t* dest, const char* callsign, uint8_t ssid, int last);
int HDLCEncode(const uint8_t* frame, int len, uint8_t* output);

volatile uint8_t packetcnt;

void AX25_Init(
				uint8_t type,
				const char *dest_call,
				uint8_t dest_ssid,
				const char *src_call,
				uint8_t src_ssid,
				uint8_t control,
				uint8_t pid,
				const char *format,    // Format-String für Payload
				...                    // Variadische Argumente für snprintf
			  );
	
int AX25_EncodeHDLC(const AX25_Frame_t* ax25_frame, HDLC_Frame_t* hdlc_frame);

#endif /* HDLC_H_ */
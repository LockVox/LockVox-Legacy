#ifndef CONFIG_H
#define CONFIG_H

#define LOCKVOX_SERVER

#define DEV_DB
#define AUTOCONFIG
#ifdef AUTOCONFIG
    #define IP_ADRESS "127.0.0.1"   //Default Server IP@
    #define AUDIO_PORTBASE 60000 //Baseport for the audio channels ; Just add channel id to portbase for all connections
    #define SERVER_PORTBASE 55000 //Baseport for connection and operation with the server
    #define DEFAULT_SAMP_RATE 8000 //Default sampling rate for audio
    #define DEFAULT_INTERVAL 0.5
#endif //AUTOCONFIG

#define CONFIG_FILE_PATH "server.cfg"

#endif // CONFIG_H

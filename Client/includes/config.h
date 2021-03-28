#ifndef CONFIG_H
#define CONFIG_H


#define LOCKVOX_CLIENT      //Use for common file between server and client

#define AUTO_CONNECT        //Use to be automatically connect to the server @IP_ADDRESS
#ifdef AUTO_CONNECT
    #define IP_ADDRESS "127.0.0.1"  //@IP of the server you want to connect
#endif

#define AUDIO_PORTBASE 60000 //Baseport for the audio channels ; Just add channel id to portbase for all connections
#define SERVER_PORTBASE 55000 //Baseport for connection and operation with the server
#define DEFAULT_SAMP_RATE 8000 //Default sampling rate for audio

//#define AUTO_LOGIN
#ifdef AUTO_LOGIN
    #define USERNAME "test3"
    #define PASSWORD "test"
#endif


#endif // CONFIG_H

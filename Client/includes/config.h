#ifndef CONFIG_H
#define CONFIG_H


#define LOCKVOX_CLIENT      //Use for common file between server and client

#define AUTO_CONNECT        //Use to be automatically connect to the server @IP_ADDRESS
#ifdef AUTO_CONNECT
    #define IP_ADDRESS "127.0.0.1"  //@IP of the server you want to connect
#endif


#define AUTO_LOGIN
#ifdef AUTO_LOGIN
    #define USERNAME "test"
    #define PASSWORD "test"
#endif



#endif // CONFIG_H

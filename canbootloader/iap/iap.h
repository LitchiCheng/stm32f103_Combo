#ifndef __IAP_H__
#define __IAP_H__
#include <stdint.h>  
typedef  void (*iapfun)(void);				
											
void iap_load_app(uint32_t appxaddr);		
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t applen);	
void iap_run();

#endif








































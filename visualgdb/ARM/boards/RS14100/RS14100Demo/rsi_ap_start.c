/**
 * @file    rsi_ap_start.c
 * @version 0.1
 * @date    15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for TCP server socket in AP mode
 *
 *  @section Description  This file contains example application for TCP server socket in AP mode
 *
 *
 */

/**
 * Include files
 * */

//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_bootup_config.h"

#include <string.h>


//! Access point SSID to be created
#define SSID              "REDPINE_AP"

//! Channel number
#define CHANNEL_NO              11

//! Security type
#define SECURITY_TYPE     RSI_OPEN
//! Encryption type
#define ENCRYPTION_TYPE   RSI_CCMP
//! Password
#define PSK               "12345678"
//! Beacon interval
#define  BEACON_INTERVAL        100
//! DTIM interval 
#define DTIM_INTERVAL           4

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x010AA8C0
//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0
//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 
//! Device port number
#define DEVICE_PORT       80

//! Number of packets to receive
#define NUMBER_OF_PACKETS  1000

//! Receive data length
#define RECV_BUFFER_SIZE   1000 

//! Memory length for driver
#define GLOBAL_BUFF_LEN   10000


//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500


//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

uint8_t station_mac[6];

//! This callback function is called when Station is connected.
//! Buffer has the MAC address of the station connected
void   stations_connect_notify_handler(uint16_t status,const uint8_t *buffer, const uint16_t length)
{
  memcpy(station_mac, buffer,6);
  printf("Station connected\n");
}

//! This callback function is called when Station is disconnected.
//! Buffer has the MAC address of the station disconnected
void   stations_disconnect_notify_handler(uint16_t status,const uint8_t *buffer, const uint16_t length)
{
    printf("Station disconnected\n");
}



int32_t rsi_ap_start()
{
  int32_t     server_socket, new_socket;
  struct      rsi_sockaddr_in server_addr, client_addr;
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0, recv_size  = 0, addr_size;
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
  //! buffer to receive data over TCP client socket
  int8_t recv_buffer[RECV_BUFFER_SIZE];

  //! Register callbacks for Station conencted and disconnected events
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, stations_connect_notify_handler);
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, stations_disconnect_notify_handler);


  //! WC initialization
  status = rsi_wireless_init(6, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Configure IP 
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Start Access point
  status =  rsi_wlan_ap_start((int8_t *)SSID, CHANNEL_NO, SECURITY_TYPE, ENCRYPTION_TYPE, (uint8_t *)PSK, BEACON_INTERVAL, DTIM_INTERVAL);
    if(status != RSI_SUCCESS)
    {
      return status;
    }


  //! Create socket
  server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket < 0)
  {
    status = rsi_wlan_get_status();
    return status;
  }

  //! Set server structure
  memset(&server_addr, 0, sizeof(server_addr));  

  //! Set family type
  server_addr.sin_family= AF_INET;

  //! Set local port number
  server_addr.sin_port = htons(DEVICE_PORT);



  //! Bind socket
  status = rsi_bind(server_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  //! Socket listen
  status = rsi_listen(server_socket, 1);
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  printf("Listening on %d.%d.%d.%d:%d\n",
         (ip_addr >> 0) & 0xFF,
         (ip_addr >> 8) & 0xFF,
         (ip_addr >> 16) & 0xFF,
         (ip_addr >> 24) & 0xFF,
         DEVICE_PORT);

  static const char szHeader[] = "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n<html><body><h1>Hello, world</h1>Greetings from RS14100! You have requested the following URL: ";
  static const char szFooter[] = "</body></html>";
  
  static volatile int RequestNumber = 0;
  static volatile char LastURL[128] = {
      0,
  };

  for (;;)
  {
      addr_size = sizeof(server_socket);

      int client_sock = rsi_accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
      RequestNumber++;
      static char szBuf[4096];
      int readPos = 0;
      char *pURL;

      if (client_sock < 0)
      {
          asm("bkpt 255");
          continue;
      }

      //Read the entire HTTP request
      for (;;)
      {
          int done = rsi_recv(client_sock, szBuf + readPos, sizeof(szBuf) - readPos - 1, 0);
          if (done < 0 || done >= (sizeof(szBuf) - readPos))
              done = 0;

          readPos += done;
          szBuf[readPos] = 0;
          if (strstr(szBuf, "\r\n\r\n"))
              break;
          if (!done)
              break;
      }

      pURL = strchr(szBuf, ' ');
      if (pURL)
      {
          char *pURLEnd = strchr(pURL + 1, ' ');
          if (pURLEnd)
          {
              pURL++;
              pURLEnd[0] = 0;
              strncpy(LastURL, pURL, sizeof(LastURL) - 1);

              rsi_send(client_sock, szHeader, sizeof(szHeader) - 1, 0);
              rsi_send(client_sock, pURL, strlen(pURL), 0);
              rsi_send(client_sock, szFooter, sizeof(szFooter) - 1, 0);
          }
      }

      rsi_shutdown(client_sock, 0);
  }

  //! Disconnect the station which is connected
  status = rsi_wlan_disconnect_stations(station_mac);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  return 0;
}

void main_loop(void)
{
  while(1)
  {
    ////////////////////////
    //! Application code ///
    ////////////////////////

    //! event loop 
	rsi_wireless_driver_task();

  }
}

int main()
{
  int32_t status;

#ifdef RSI_WITH_OS

  rsi_task_handle_t wlan_task_handle = NULL;

  rsi_task_handle_t driver_task_handle = NULL;
#endif


  //! Driver initialization
   status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
   if((status < 0) || (status > GLOBAL_BUFF_LEN))
   {
     return status;
   }

   //! Redpine module intialisation
     status = rsi_device_init(LOAD_NWP_FW);
     if(status != RSI_SUCCESS)
     {
       return status;
     }

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_ap_start, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call TCP server application in AP mode
  status = rsi_ap_start();

  //! Application main loop
  main_loop();
#endif
  return status;

}


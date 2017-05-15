/*
 * Copyright 2010-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <string.h>


#include <ti/sysbios/knl/Task.h>
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include "aws_iot_config.h"
#include "config.h"

#include "pin_mux_config.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"

// Driverlib includes
#include "utils.h"
//#include "hw_memmap.h"
#include "hw_common_reg.h"
//#include "hw_types.h"
#include "hw_adc.h"
#include "hw_ints.h"
#include "hw_gprcm.h"
//#include "rom.h"
//#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "uart.h"
//#include "pinmux.h"
#include "pin.h"
#include "adc.h"

char HostAddress[255] = AWS_IOT_MQTT_HOST;
uint32_t port = AWS_IOT_MQTT_PORT;
uint32_t publishCount = 0;
int devicedetails=1,sensordetails=0;
unsigned int  uiChannel;
float pw=0;

float adcread(int ch_no){
if (ch_no == 1)
	uiChannel = ADC_CH_0;
else if (ch_no == 2)
	uiChannel = ADC_CH_1;
else if (ch_no == 3)
	uiChannel = ADC_CH_2;
else if (ch_no == 4)
	uiChannel = ADC_CH_3;

#ifdef CC3200_ES_1_2_1
        //
        // Enable ADC clocks.###IMPORTANT###Need to be removed for PG 1.32
        //
        HWREG(GPRCM_BASE + GPRCM_O_ADC_CLK_CONFIG) = 0x00000043;
        HWREG(ADC_BASE + ADC_O_ADC_CTRL) = 0x00000004;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE0) = 0x00000100;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE1) = 0x0355AA00;
#endif

        //
        // Configure ADC timer which is used to timestamp the ADC data samples
        //
        MAP_ADCTimerConfig(ADC_BASE,2^17);

	//PinTypeADC(PIN_59, 0xFF);

        //
        // Enable ADC timer which is used to timestamp the ADC data samples
        //
        MAP_ADCTimerEnable(ADC_BASE);

        //
        // Enable ADC module
        //
        MAP_ADCEnable(ADC_BASE);

        //
        // Enable ADC channel
        //

        MAP_ADCChannelEnable(ADC_BASE, uiChannel);
	//MAP_UtilsDelay(800000);

	if(MAP_ADCFIFOLvlGet(ADC_BASE, uiChannel)){

	        unsigned long ulSample = MAP_ADCFIFORead(ADC_BASE, uiChannel);
		IOT_INFO("%lu\n",ulSample);
		IOT_INFO("%f...\n",(float)((ulSample >> 2 ) & 0x0FFF));
		IOT_INFO("\n\rVoltage is %f\n\r",(((float)((ulSample >> 2 ) & 0x0FFF))*1.467)/4096);
		pw=(((float)((ulSample >> 2 ) & 0x0FFF))*1.467)/4096;
			
	}
return pw;
}

void NetMACAddressGet(unsigned char *pMACAddress)
{
    unsigned char macAddressLen = SL_MAC_ADDR_LEN;

    //
    // Get the MAC address
    //
    sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macAddressLen, pMACAddress);
	IOT_INFO("Original MAC id is %s",pMACAddress);
}


void MQTTcallbackHandler(AWS_IoT_Client *pClient, char *topicName,
        uint16_t topicNameLen, IoT_Publish_Message_Params *params, void *pData)
{
    IOT_INFO("Subscribe callback");
    IOT_INFO("%.*s\t%.*s",topicNameLen, topicName, (int)params->payloadLen,
            (char *)params->payload);
	
	char controlstring1[100],controlstring2[100],controlstring3[100],controlstring4[100],controlstring5[100];
	sprintf(controlstring1,"%s on",controlfeedname1);
	sprintf(controlstring2,"%s off",controlfeedname1);
	sprintf(controlstring3,"%s on",controlfeedname2);
	sprintf(controlstring4,"%s off",controlfeedname2);
	
	if(!strncmp(params->payload,controlstring1,strlen(controlstring1)))
		{
			GPIOPinWrite(GPIOA3_BASE, 0x10, 0x10);
			IOT_INFO("%s is on",controlfeedname1);
		}
	if(!strncmp(params->payload,controlstring2,strlen(controlstring2)))
		{
		
			GPIOPinWrite(GPIOA3_BASE, 0x10, 0x0);
			IOT_INFO("%s is off",controlfeedname1);
		}

	if(!strncmp(params->payload,controlstring3,strlen(controlstring3)))
		{
			GPIOPinWrite(GPIOA3_BASE, 0x40, 0x40);
			IOT_INFO("%s is on",controlfeedname2);
		}

	if(!strncmp(params->payload,controlstring4,strlen(controlstring4)))
		{
			GPIOPinWrite(GPIOA3_BASE, 0x40, 0x0);
			IOT_INFO("%s is off",controlfeedname2);
		}
}

void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data)
{
    IOT_WARN("MQTT Disconnect");
    IoT_Error_t rc = SUCCESS;

    if (NULL == data) {
        return;
    }

    AWS_IoT_Client *client = (AWS_IoT_Client *)data;
    if (aws_iot_is_autoreconnect_enabled(client)) {
        IOT_INFO("Auto Reconnect is enabled, Reconnecting attempt will start now");
    }
    else {
        IOT_WARN("Auto Reconnect not enabled. Starting manual reconnect...");
        rc = aws_iot_mqtt_attempt_reconnect(client);
        if (NETWORK_RECONNECTED == rc) {
            IOT_WARN("Manual Reconnect Successful");
            IOT_WARN("Manual Reconnect Failed - %d", rc);
        }
        else {
            IOT_WARN("Manual Reconnect Failed - %d", rc);
        }
    }
}

static int livestatus=0;
void runAWSClient(void)
{
	IOT_INFO("aws is starting............");
    IoT_Error_t rc = SUCCESS;
    bool infinitePublishFlag = true;
    char topicName[100] = "sdkTest/sub";
	sprintf(topicName,"%s_%s",UserName,DeviceName);
	IOT_INFO("the topic name is %s\n ",topicName);
    int topicNameLen = strlen(topicName);
	char *pubtopicName = "sdkTest/sub";
    int pubtopicNameLen = strlen(pubtopicName);
	AWS_IoT_Client client;
    IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
    IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;

    IoT_Publish_Message_Params paramsQOS0;
    IoT_Publish_Message_Params paramsQOS1;

    IOT_INFO("\nAWS IoT SDK Version %d.%d.%d-%s\n", VERSION_MAJOR, VERSION_MINOR,
            VERSION_PATCH, VERSION_TAG);


    mqttInitParams.enableAutoReconnect = false; // We enable this later below
    mqttInitParams.pHostURL = HostAddress;
    mqttInitParams.port = port;
    /*
     *  The following cert file variables are not used in this release. All
     *  cert files must exist in the "/certs" directory and be named "ca.der",
     *  "cert.der" and "key.der", as shown in the certflasher application. The
     *  ability to change this will be added in a future release.
     */
    mqttInitParams.pRootCALocation = AWS_IOT_ROOT_CA_FILENAME;
    mqttInitParams.pDeviceCertLocation = AWS_IOT_CERTIFICATE_FILENAME;
    mqttInitParams.pDevicePrivateKeyLocation = AWS_IOT_PRIVATE_KEY_FILENAME;
    mqttInitParams.mqttCommandTimeout_ms = 20000;
    mqttInitParams.tlsHandshakeTimeout_ms = 5000;
    mqttInitParams.isSSLHostnameVerify = true;
    mqttInitParams.disconnectHandler = disconnectCallbackHandler;
    mqttInitParams.disconnectHandlerData = (void *)&client;

    rc = aws_iot_mqtt_init(&client, &mqttInitParams);
    if (SUCCESS != rc) {
        IOT_ERROR("aws_iot_mqtt_init returned error : %d ", rc);
    }

    connectParams.keepAliveIntervalInSec = 10;
    connectParams.isCleanSession = true;
    connectParams.MQTTVersion = MQTT_3_1_1;
    connectParams.pClientID = AWS_IOT_MQTT_CLIENT_ID;
    connectParams.clientIDLen = (uint16_t)strlen(AWS_IOT_MQTT_CLIENT_ID);
    connectParams.isWillMsgPresent = false;

	

    IOT_INFO("Connecting...");
    rc = aws_iot_mqtt_connect(&client, &connectParams);
    if (SUCCESS != rc) {
        IOT_ERROR("Error(%d) connecting to %s:%d", rc, mqttInitParams.pHostURL,
                mqttInitParams.port);
    }

    /*
     *  Enable Auto Reconnect functionality. Minimum and Maximum time of
     *  exponential backoff are set in aws_iot_config.h:
     *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
     *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
     */
    rc = aws_iot_mqtt_autoreconnect_set_status(&client, true);
    if (SUCCESS != rc) {
        IOT_ERROR("Unable to set Auto Reconnect to true - %d", rc);
    }

    IOT_INFO("Subscribing...");
    rc = aws_iot_mqtt_subscribe(&client, topicName, topicNameLen, QOS0,
            MQTTcallbackHandler, NULL);
    if (SUCCESS != rc) {
        IOT_ERROR("Error subscribing (%d)", rc);
    }

    char cPayload[512];
    
	paramsQOS0.qos = QOS0;
    paramsQOS0.payload = (void *)cPayload;
    paramsQOS0.isRetained = 0;

    paramsQOS1.qos = QOS1;
    paramsQOS1.payload = (void *)cPayload;
    paramsQOS1.isRetained = 0;

    if (publishCount != 0) {
       infinitePublishFlag = false;
    }

    while ((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc ||
            SUCCESS == rc) && (publishCount > 0 || infinitePublishFlag)) {
        rc = aws_iot_mqtt_yield(&client, 150);

        if (NETWORK_ATTEMPTING_RECONNECT == rc) {
            /* If the client is attempting to reconnect, skip rest of loop */
            continue;
        }

        IOT_INFO("-->sleep");
        Task_sleep(1000);
       /* Recalculate string len to avoid truncation in subscribe callback */
			if(UserName != "" && DeviceName !=""){
		if ((sensordetails==1)){
			if(((++livestatus)%timePeriod) == 0){
		static char mac_id[13];
		mac_id[13]="/0";
		unsigned char macAddressLen = SL_MAC_ADDR_LEN;
		unsigned char pMACAddress[macAddressLen];

    //
    // Get the MAC address
    //
    sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macAddressLen, pMACAddress);
	IOT_INFO("Original MAC id is %x",pMACAddress);
                
		int a,b,c;
		
		if(GPIOPinRead(sensorpin1_base,sensorpin1_Hexnumber)){
			a=1;
		}
		else{
			a=0;
		}
		
		if(GPIOPinRead(sensorpin2_base,sensorpin2_Hexnumber)){
			b=1;
		}
		else{
			b=0;
		}
		
		if(GPIOPinRead(sensorpin3_base,sensorpin3_Hexnumber)){
			c=1;
		}
		else{
			c=0;
		}
		float d = adcread(analogchannel);
		
		snprintf(cPayload,sizeof(cPayload),"{\n\"feedname1\" : \"%s\",\n\"sensorvalue1\" : \"%d\",\n\"feedname2\" : \"%s\",\n\"sensorvalue2\" : \"%d\",\n\"feedname3\" : \"%s\",\n\"sensorvalue3\" : \"%d\",\n\"feedname4\" : \"%s\",\n\"sensorvalue4\" : \"%f\",\n\"username\" : \"%s\",\n\"devicename\" : \"%s\",\n\"paasmerid\" : \"%x\"\n}",feedname1,a,feedname2,b,feedname3,c,analogfeedname,d,UserName,DeviceName,mac_id);
		
		sprintf(pubtopicName,"%s","paasmer_sensor_details");
		pubtopicNameLen = strlen(pubtopicName);

        paramsQOS1.payloadLen = strlen(cPayload);
        do {
            rc = aws_iot_mqtt_publish(&client, pubtopicName, pubtopicNameLen,
                   &paramsQOS1);
            if (publishCount > 0) 
			

                publishCount--;
            }
        while (MQTT_REQUEST_TIMEOUT_ERROR == rc &&
                (publishCount > 0 || infinitePublishFlag));
		}
		}
		
		else if (devicedetails==1){
		static char mac_id[13];
		mac_id[13]="/0";
		printf("\nmac address is %x\n",mac_id);
		int a,b;
                
		snprintf(cPayload,sizeof(cPayload),"{\n\"feedname1\" : \"%s\",\n\"feedtype1\" : \"sensor\",\n\"feedname2\" : \"%s\",\n\"feedtype2\" : \"sensor\",\n\"feedname3\" : \"%s\",\n\"feedtype3\" : \"sensor\",\n\"feedname4\" : \"%s\",\n\"feedtype4\" : \"sensor\",\n\"feedname5\" : \"%s\",\n\"feedtype5\" : \"control\",\n\"feedname6\" : \"%s\",\n\"feedtype6\" : \"control\",\n\"username\" : \"%s\",\n\"devicename\" : \"%s\",\n\"paasmerid\" : \"%x\"\n}",feedname1,feedname2,feedname3,analogfeedname,controlfeedname1,controlfeedname2,UserName,DeviceName,mac_id);
		
		sprintf(pubtopicName,"%s","paasmer_device_details");
                devicedetails=0;
		sensordetails=1;
		
		pubtopicNameLen = strlen(pubtopicName);

        paramsQOS1.payloadLen = strlen(cPayload);
        do {
            rc = aws_iot_mqtt_publish(&client, pubtopicName, pubtopicNameLen,
                   &paramsQOS1);
            if (publishCount > 0) 
			

                publishCount--;
            }
        while (MQTT_REQUEST_TIMEOUT_ERROR == rc &&
                (publishCount > 0 || infinitePublishFlag));
		}
				}
		    
    }
			
			
    if (SUCCESS != rc) {
        IOT_ERROR("An error occurred in the loop. Error code = %d\n", rc);
    }
    else {
        IOT_INFO("Publish done\n");
}}
	


# CC3200-SDK
**Paasmer IoT SDK** for CC3200 

## Overview

The **PAASMER SDK** for **CC3200 LaunchPad XL** is a collection of source files that enables you to connect to the Paasmer IoT Platform. It includes the trasnport client for **MQTT** with **TLS** support.  It is distributed in source form and intended to be built into customer firmware along with application code, other libraries and TI-RTOS.

## Featuers

The **CC3200-SDK** simplifies access to the Pub/Sub functionality of the **Paasmer IoT** broker via **MQTT**. The SDK has been tested to work on the **CC3200 LaunchPad XL** running TI-RTOS.

## MQTT Connection

The **CC3200-SDK** provides functionality to create and maintain a mutually authenticated TLS connection over which it runs **MQTT**. This connection is used for any further publish operations and allow for subscribing to **MQTT** topics which will call a configurable callback function when these topics are received.

## Pre Requisites

Registration on the portal http://developers.paasmer.co is necessary to connect the devices to the **Paasmer IoT Platfrom** .The SDK has been tested on the CC3200 LaunchPad.

<a name="RequiredHW"></a>
#### Required Hardware

The following device is required for this SDK:
- [CC3200 Launchpad](http://www.ti.com/tool/cc3200-launchxl)

Please ensure that your device has been updated with the latest firmware and or service pack.

<a name="TI-SW"></a>
#### TI Software Installation

The following TI software products must be installed in order to build and run the sample applications. While not strictly required, we recommend that you install these products into a common directory and that you use directory names without any whitespace. This documentation assumes that you haved installed everything in a directory named `home\logname\ti\`.

- Install [Code Composer Studio v6.1.2](http://www.ti.com/tool/ccstudio?keyMatch=code%20composer%20studio) or higher.

- Install [TI-RTOS for CC32xx 2.16.00.08](http://downloads.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/index.html) or higher.


## Installation

* Download the SDK or clone it using the command below.
```
$ git clone github.com/PaasmerIoT/CC3200-SDK.git
$ cd CC3200-SDK
```

* To connect the device to Paasmer IoT Platfrom, the following steps need to be performed.

```
$ sudo ./install.sh
```

* Upon successful completion of the above command, the following commands need to be executed.
```
1) sudo su 
2) source ~/.bashrc 
3) PAASMER-CC3200_THING 
4) PAASMER-CC3200_POLICY 
5) sed -i 's/alias PAASMER-CC3200/#alias PAASMER-CC3200/g' ~/.bashrc 
6) exit 
```

<a name="Enviro-Setup"></a>
## Build Environment Set Up

### Updating Paths To Product Dependencies

1. Edit the `products.mak` file in `<CC3200-SDK_INSTALL_DIR>/` using your favorite text editor.
2. Update the variables `XDC_INSTALL_DIR and TIRTOS_INSTALL_DIR to point to the locations where you installed these products.
3. The variable `TI_ARM_CODEGEN_INSTALL_DIR` should point to the installation location of the TI ARM compiler in your CCS installation.
4. After modification, these variable definitions should look similar to the following if you are working in Windows. (Windows users: note the use of "/" in the path).
    ```
    XDC_INSTALL_DIR = home\logname\ti\xdctools_3_32_00_06_core
    TIRTOS_INSTALL_DIR = home\logname\ti\tirtos_cc32xx_2_16_00_08
    TI_ARM_CODEGEN_INSTALL_DIR = home\logname\ti\ccsv6\tools\compiler\ti-cgt-arm_5.2.5
    ```

* Go to the diectory below.
```
$ cd samples/tirtos/subscribe_publish_sample/cc3200
```

* Edit the config.h file to include the user name(Email), device name, feed names and GPIO pin details.

```
#define UserName "Email Address" //your user name in website

#define DeviceName "" //your device name

#define feedname1 "feed1" //feed name you use in the website

#define sensorpin1_base GPIOA1_BASE //modify with the GPIO Base which you connected the sensor

#define sensorpin1_Hexnumber 0x40   // modify with GPIO Hexnumber for the selected pin

#define feedname2 "feed2" //feed name you use in the website

#define sensorpin2_base GPIOA2_BASE  //modify with the GPIO Base which you connected the sensor

#define sensorpin2_Hexnumber 0x1    // modify with GPIO Hexnumber for the selected pin

#define feedname3 "feed3" //feed name you use in the website

#define sensorpin3_base GPIOA2_BASE //modify with the GPIO Base which you connected the sensor

#define sensorpin3_Hexnumber 0x1    // modify with GPIO Hexnumber for the selected pin

#define analogfeedname "feed4" //feed name you use in the website for analog readings

#define analogchannel 3  // analog channel 3

#define controlfeedname1 "controlfeed1" //feed name you use in the website for control device pins

#define controlpin1_base GPIOA3_BASE //modify with the GPIO Base which you connected the device(eg.: motor)

#define controlpin1_Hexnumber 0x10   // modify with GPIO Hexnumber for the selected pin

#define controlfeedname2 "controlfeed2" //feed name you use in the website for control device pins

#define controlpin2_base GPIOA3_BASE   //modify with the GPIO Base which you connected the device(eg.: motor)

#define controlpin2_Hexnumber 0x40   // modify with GPIO Hexnumber for the selected pin

#define timePeriod 2 //change the time delay as you required for sending sensor values to paasmer cloud

```

* Open the file `wificonfig.h` from the directory `<CC3200-SDK_INSTALL_DIR>/samples/tirtos/subscribe_publish_sample/cc3200`. Search for "USER STEP" and update the WIFI SSID and SECURITY_KEY macros. 
 
* Compile the code and generate output file.

```
$ sudo ./Build.sh 
```
<a name="Setup-CCS"></a>
## Setting Up Code Composer Studio Before Running The Samples

1. Plug the CC3200 Launchpad into a USB port on your PC

2. Open a serial session to the appropriate COM port with the following settings:

    ```
    Baudrate:     9600
    Data bits:       8
    Stop bits:       1
    Parity:       None
    Flow Control: None
    ```

3. Open Code Composer Studio.

4. In Code Composer Studio, open the CCS Debug Perspective - Windows menu -> Open Perspective -> CCS Debug

5. Open the Target Configurations View - Windows menu -> Show View -> Target Configurations

6. Right-click on User Defined. Select New Target Configuration.

7. Use `CC3200.ccxml` as "File name". Hit Finish.

8. In the Basic window, select "Stellaris In-Circuit Debug Interface" as the "Connection", and check the box next to "CC3200" in "Board or Device". Hit Save.

9. Right-click "CC3200.ccxml" in the Target Configurations View. Hit Launch Selected Configuration.

10. Under the Debug View, right-click on "Stellaris In-Circuit Debug Interface_0/Cortex_M4_0". Select "Connect Target".

<a name="Run-TOOL"></a>
## Running The Certificate Flasher Tool

All samples rely on a set of certificates from AWS. As a result, the certificates need to be stored once into flash memory prior to running the samples. To flash the certificates, simply run the flasher tool you have previously [built](#Build-TOOL) using this procedure:

1. Select Run menu -> Load -> Load Program..., and browse to the file `certflasher.out` in `<CC3200-SDK_INSTALL_DIR>/PAASMER-CC3200-$no/tirtos/certs/cc3200`. Hit OK. This will load the program onto the board.

2. Run the application by pressing F8. The output in the CCS Console looks as follows:

    ```
    Flashing ca certificate file ...
    Flashing client certificate file ...
    Flashing client key file ...
    done.
    ```
3. Hit Alt-F8 (Suspend) to halt the CPU.

<a name="Run-SAMPLE"></a>
## Running the Sample

1. Disconnect and reconnect the CC3200's USB cable to power cycle the hardware, and then reconnect in CCS.  For best results, it is recommended to do this before loading and running an application every time, in order to reset the CC3200's network processor.

2. Select Run menu -> Load -> Load Program..., and browse to the file `subscribe_publish_sample.out` in `<CC3200-SDK_INSTALL_DIR>/PAASMER-CC3200-$no/tirtos/subscribe_publish_sample/cc3200`. Hit OK. This will load the program onto the board. (The same procedure applies to other samples by substituting `subscribe_publish_sample`)

3. Run the application by pressing F8.

* The device would now be connected to the Paasmer IoT Platfrom and publishing sensor values are specified intervals.

## Support

The support forum is hosted on the GitHub, issues can be identified by users and the Team from Paasmer would be taking up requstes and resolving them. You could also send a mail to support@paasmer.co with the issue details for quick resolution.

## Note

The Paasmer IoT CC3200-SDK utilizes the features provided by AWS-IOT-SDK for C.
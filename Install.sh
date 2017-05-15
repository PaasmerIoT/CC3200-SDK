#!/bin/bash

path=`pwd`
#RANDOM=$$
RAN=$(shuf -i 10000-100000 -n 1);
no=$RAN
#no=11224
cat > .Install.log << EOF3
Logfile for Installing Paasmer...
EOF3

logname=$(who | awk '{print $1}');

echo $path >> .Install.log
user=$(echo $USER)
user='/home/'$user
echo $user >> .Install.log
echo "--> Installing...\n" >> .Install.log
sudo chmod -R 777 ./*

echo "--> Installing requerments......." >> .Install.log
sudo apt-get update >> .Install.log
sudo apt-get install -y expect >> .Install.log
sudo apt-get install -y mysql-client-core-5.7 >> .Install.log
sudo apt-get install -y python2.7 libssl-dev python-pip >> .Install.log
sudo apt-get install -y python-dev >> .Install.log
sudo apt-get install -y xterm >> .Install.log
#cd ~

sudo pip install awscli
echo "Configuring data..." >> .Install.log

sudo mkdir -p /root/.aws
sudo chmod -R 777 /root/.aws
sudo mkdir -p $path/certs
sudo chmod -R 777 $path/certs

cat > /root/.aws/config << EOF1
[default]
region = us-west-2
EOF1
echo "U2FsdGVkX1+WF++BqX9N+Bfu/jsDgfM9rxd77LO3I8xVxgLBbNmglZprOCtcyvJs
Jteh6FPrLMKb4r8uSq6C/w==" > .old.txt
accesskey=$(cat .old.txt | openssl enc -aes-128-cbc -a -d -salt -pass pass:asdfghjkl);

keyid=$(echo "U2FsdGVkX19XbOtwglyiBxjyEME74FjnlS5KrbdvXHQGbUC/BulYsgg+a35BR64W" | openssl enc -aes-128-cbc -a -d -salt -pass pass:asdfghjkl);

echo "[default]
aws_secret_access_key = $accesskey
aws_access_key_id = $keyid
" > /root/.aws/credentials


echo "-----BEGIN CERTIFICATE-----
MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB
yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL
ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp
U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW
ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0
aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL
MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW
ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln
biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp
U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y
aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1
nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex
t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz
SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG
BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+
rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/
NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E
BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH
BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy
aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv
MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE
p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y
5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK
WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ
4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N
hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq
-----END CERTIFICATE-----" > $path/certs/rootCA.crt;

endpoint=$(sudo su - root -c"aws iot describe-endpoint" | grep "endpoint" | awk '{print $2}');
echo $endpoint >> .Install.log

touch $path/certs/output.txt

PAASMER=PAASMER-CC3200-$no;
sudo mkdir -p $path/$PAASMER;
echo $PAASMER >> .Install.log
Thingjson=$(sudo su - root -c "aws iot create-thing --thing-name $PAASMER");
echo $Thingjson >> .Install.log

touch $path/certs/output.txt
sudo su - root -c "aws iot create-keys-and-certificate --set-as-active --certificate-pem-outfile $path/certs/$PAASMER-certificate.pem.crt --public-key-outfile $path/certs/$PAASMER-public.pem.key --private-key-outfile $path/certs/$PAASMER-private.pem.key" > $path/certs/output.txt

sudo chmod -R 777 ./*
cat $path/certs/output.txt >> .Install.log

out=$(sudo cat $path/certs/output.txt | grep "certificateArn" | awk '{print $2}')

#ARN=$(echo "${out::-1}")
ARN=$(echo $out | sed 's/,$//')
echo $ARN >> .Install.log

sudo su - root -c "aws iot create-policy --policy-name PAASMER-CC3200-$no --policy-document '{ \"Version\": \"2012-10-17\", \"Statement\": [{\"Action\": [\"iot:*\"], \"Resource\": [\"*\"], \"Effect\": \"Allow\" }] }'" >> .Install.log

certpem=$(while read -r line; do   echo "\"$line\"" |sed -n '/CERTIFICATE/!p' ; done < "$path/certs/PAASMER-CC3200-$no-certificate.pem.crt");
echo $certpem >> .Install.log
privatekey=$(while read -r line; do   echo "\"$line\"" |sed -n '/PRIVATE/!p' ; done <"$path/certs/PAASMER-CC3200-$no-private.pem.key");
echo $privatekey >> .Install.log
rootcert=$(while read -r line; do   echo "\"$line\"" |sed -n '/CERTIFICATE/!p' ; done <"$path/certs/rootCA.crt");
echo $rootcert >> .Install.log

chrlen=${#PAASMER}

echo "#define AWS_IOT_MY_THING_NAME 	\"$PAASMER\"" > configure.h
echo "#define AWS_IOT_MQTT_CLIENT_ID 	\"$PAASMER\"" >> configure.h
echo "#define SIZEOFID $chrlen" >> configure.h
echo "const char root_ca_pem[] = $rootcert ;" >> configure.h;
echo "const char client_private_key_pem[] = $privatekey ;" >> configure.h;
echo "const char client_cert_pem[] = $certpem ;" >> configure.h;
echo "char deviceID[] = \"PAASMER-CC3200-$no\";" >> configure.h;
echo "char *pubtopName = \"PAASMER-CC3200-$no/sub\";" >> configure.h;

#sudo sed -i 's/#define AWS_IOT_MY_THING_NAME/\/\/#define AWS_IOT_MY_THING_NAME/g' $path/samples/tirtos/subscribe_publish_sample/cc3200/aws_iot_config.h
#sudo sed -i 's/#define AWS_IOT_MQTT_CLIENT_ID/\/\/#define AWS_IOT_MQTT_CLIENT_ID/g' $path/samples/tirtos/subscribe_publish_sample/cc3200/aws_iot_config.h

#sudo sed -i '/#define SRC_SHADOW_IOT_SHADOW_CONFIG_H_/a #define AWS_IOT_MY_THING_NAME \"'$PAASMER'\"' $path/samples/tirtos/subscribe_publish_sample/cc3200/aws_iot_config.h
#sudo sed -i '/#define SRC_SHADOW_IOT_SHADOW_CONFIG_H_/a #define AWS_IOT_MQTT_CLIENT_ID \"'$PAASMER'\"' $path/samples/tirtos/subscribe_publish_sample/cc3200/aws_iot_config.h


sudo su - root -c "echo \"alias PAASMER-CC3200_THING='sudo aws iot attach-thing-principal --thing-name PAASMER-CC3200-$no --principal $ARN'\" >> /root/.bashrc"
sudo su - root -c "echo \"alias PAASMER-CC3200_POLICY='sudo aws iot attach-principal-policy --policy-name PAASMER-CC3200-$no --principal $ARN'\" >> /root/.bashrc"
echo "Added to PAASMER-CC3200 alias...\n" >> .Install.log


echo '#!/bin/bash
path=$(pwd);
logname="'$logname'"
cd $(pwd)/sampl*/tirtos/certs/cc3200/
make clean && ~/ti/xdctools_*/gmake all;
cd ../../subscribe_publish_sample/cc3200/
make clean && ~/ti/xdctools_*/gmake all;
echo "Done Configuring and building output file...";
cd ../../../../
sudo chmod -R 777 ./*
sudo mkdir -p $path/Build

echo "Just check the output files are created in PAASMER-CC3200-$no folder...!";

/home/$logname/ti/ccsv6/utils/tiobj2bin/tiobj2bin "$path/samples/tirtos/subscribe_publish_sample/cc3200/subscribe_publish_sample.out" "$path/'$PAASMER'/subscribe_publish_sample.bin" "/home/$logname/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/ofd6x" "/home/$logname/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/hex6x" "/home/$logname/ti/ccsv6/utils/tiobj2bin/mkhex4bin";

/home/$logname/ti/ccsv6/utils/tiobj2bin/tiobj2bin "$path/samples/tirtos/certs/cc3200/certflasher.out" "$path/'$PAASMER'/certflasher.bin" "/home/$logname/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/ofd6x" "/home/$logname/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/hex6x" "/home/$logname/ti/ccsv6/utils/tiobj2bin/mkhex4bin"


sudo cp samples/tirtos/subscribe_publish_sample/cc3200/subscribe_publish_sample.out ./'$PAASMER'/
sudo cp samples/tirtos/certs/cc3200/certflasher.out ./'$PAASMER'/
sudo chmod -R 777 ./*
' > $path/Build.sh

sudo chmod -R 777 ./*

echo "************************************************************"
echo "-----------------------------------------------------------"
echo "-->  Run below commands.."
echo "-->  1) sudo su "
echo "-->  2) source ~/.bashrc "
echo "-->  3) PAASMER-CC3200_THING "
echo "-->  4) PAASMER-CC3200_POLICY "
echo "-->  5) sed -i 's/alias PAASMER-CC3200/#alias PAASMER-CC3200/g' ~/.bashrc "
echo "-->  6) exit "
echo "**************************************************************";


sudo scp -i ".awskeypair.pem" certs/PAASMER-CC3200-$no-* ec2-user@ec2-52-41-46-86.us-west-2.compute.amazonaws.com:/var/www/html/certs/ >> .Install.log
echo "File Transfered successfully...." >> .Install.log

echo $PAASMER >> .Install.log

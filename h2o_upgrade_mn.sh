#!/bin/bash

BINPATH=$(which h2od | sed "s/h2od//g")
USESYSTEMD=$(systemctl list-unit-files|grep h2od|wc -l)

##---Check if root use may need sudo in some spots
if [ $(id -u) -ne 0 ]
then
  SUDO='sudo'
else
  SUDO=''
fi

if sudo test -d "/root/.h2ocore"; then
	DATPATH="/root/.h2ocore"
else 
	if test -d "$HOME/.h2ocore"; then
		DATPATH="$HOME/.h2ocore"
	else
		echo -e "\nUnable to locate H2O data file path."
		exit 1
	fi
fi

if pgrep -x "h2od" > /dev/null
then
	MNPKEY=$($BINPATH/h2o-cli masternode genkey)
	
	echo "Trying to stop h2o service..."

	if  [ "$USESYSTEMD" -eq "1" ];
	then
		$SUDO systemctl stop h2o
	else
		$BINPATH/h2o-cli stop
	fi
	
	sleep 6

	if pgrep -x "h2od" > /dev/null
	then
		echo -e "\nCannot stop masternode."
		echo -e "\nPlease shutdown H2O masternode before installing."
		echo -e "\nUse the command h2o-cli stop \n\n"
		exit 1
	fi
else
	MNPKEY=$(egrep "^masternodeprivkey=" $DATPATH/h2o.conf |awk -F '=' '{print $2}')
fi

##---Download new H2O Wallet and uncompress
echo "Downloading upgraded h2o wallet..."
wget https://github.com/h2ocore/h2o/releases/download/v0.12.1.7/Linux64-H2O-cli-01217.tgz
tar -zxf Linux64-H2O-cli-01217.tgz
rm -f Linux64-H2O-cli-01217.tgz

##---Check that files exist
f1="h2od"
f2="h2o-cli"
f3="h2o-tx"

if [ ! -f $f1 ]; then
    echo "{$f1} not found!"
    echo "Stopping...."
    exit 0
fi
if [ ! -f $f2 ]; then
    echo "{$f2} not found!"
    echo "Stopping...."
    exit 0   
fi
if [ ! -f $f3 ]; then
    echo "{$f3} not found!"
    echo "Stopping...."
    exit 0
fi

echo "Moving {$f1} {$f2} {$f3} to $BINPATH..."

$SUDO mv -f $f1 $f2 $f3 $BINPATH

$SUDO rm -f $DATPATH/banlist.dat
$SUDO rm -f $DATPATH/fee_estimates.dat
$SUDO rm -f $DATPATH/governance.dat
$SUDO rm -f $DATPATH/mncache.dat
$SUDO rm -f $DATPATH/mnpayments.dat
$SUDO rm -f $DATPATH/netfulfilled.dat
$SUDO rm -f $DATPATH/peers.dat

$SUDO cp $DATPATH/h2o.conf $DATPATH/h2o.conf.$(date +%Y%m%d%H%M%S).saved 
$SUDO egrep -v "^masternodeprivkey=" $DATPATH/h2o.conf >/tmp/h2o.conf.new
echo -e "\nmasternodeprivkey=$MNPKEY\n" >>/tmp/h2o.conf.new
$SUDO mv -f /tmp/h2o.conf.new $DATPATH/h2o.conf

echo -e "************************************************************************"
echo -e "***"
echo -e "***   A new masternode private key has been generated"
echo -e "***   Please update your 'masternode.conf' file with this new key:"
echo -e "***"
echo -e "***                $MNPKEY"
echo -e "***"
echo -e "************************************************************************\n\n"

##---Restart H2O Masternode
echo "Trying to restart masternode..."
if  [ "$USESYSTEMD" -eq "1" ];
then
    $SUDO systemctl start h2o
else
    $BINPATH/h2od
fi

sleep 3

if ! pgrep -x "h2od" > /dev/null
then
    echo "Could not restart H2O masternode :'("
    exit 1
fi

echo "Checking version..."
sleep 2

##---Check for success for failure of upgrade
if  h2o-cli getinfo | grep -m 1 '"protocolversion": 70209,'
then
        echo "H2O Mastenode has been successfully updated"
else
        echo "H2O masternode Install failed :'( "
fi

echo -e "\n\nUse\n\th2o-cli mnsync status\n"
echo -e "and\n\th2o-cli masternode status\n"
echo -e "to verify if masternode is active"

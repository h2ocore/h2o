#!/bin/bash

BINPATH=$(which h2od | sed "s/h2od//g")
USESYSTEMD=$(systemctl list-unit-files|grep -i h2o|wc -l)

##---Check if root use may need sudo in some spots and set home path
if [ $(id -u) -ne 0 ]
then
  SUDO='sudo'
  if test -d "$HOME/.h2ocore"; then
                DATPATH="$HOME/.h2ocore"
        else
                echo -e "\nUnable to locate H2O data file path."
                exit 1
        fi
else
  DATPATH="/root/.h2ocore"
  SUDO=''
fi

#generate masternode key and stop h2o node
if pgrep -x "h2od" > /dev/null
then
    
    echo "Trying to stop h2o service..."
    if  [ "$USESYSTEMD" > 0 ];  
    then
        SVCNAME=$(systemctl list-unit-files|grep -i h2o|awk '{print $1}'|head -n 1)
        $SUDO systemctl stop $SVCNAME
    else
        echo "${BINPATH}h2o-cli stop"
        $BINPATH\h2o-cli stop
    fi
    
    sleep 6

    if pgrep -x "h2od" > /dev/null
    then
        echo -e "\nCannot stop masternode."
        echo -e "\nPlease shutdown H2O masternode before installing."
        echo -e "\nUse the command h2o-cli stop \n\n"
        exit 1
    fi
#else
    #MNPKEY=$(egrep "^masternodeprivkey=" $DATPATH/h2o.conf |awk -F '=' '{print $2}')
fi


echo "In order to proceed with the installation, please paste Masternode genkey by clicking right mouse button. Once masternode genkey is visible in the terminal please hit ENTER.";
read MNPKEY

##---Download new H2O Wallet and uncompress
echo "Downloading upgraded h2o wallet..."
wget -O Linux64-H2O-cli-01218.tgz https://github.com/h2ocore/h2o/releases/download/v0.12.1.8/Linux64-H2O-cli-01218.tgz
tar -zxf Linux64-H2O-cli-01218.tgz
rm -f Linux64-H2O-cli-01218.tgz

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
printf "\nmasternodeprivkey=$MNPKEY\n" >>/tmp/h2o.conf.new
#printf "\naddnode=140.82.52.45:13355\naddnode=104.207.145.111:13355\naddnode=108.61.219.28:13355\naddnode=149.28.232.198:13355\n"
#printf "\naddnode=80.210.127.1:13355\naddnode=80.210.127.2:13355\naddnode=80.210.127.3:13355\n\n\"


$SUDO mv -f /tmp/h2o.conf.new $DATPATH/h2o.conf

#printf "\n************************************************************************"
#printf "\n***"
#printf "\n***   A new masternode private key has been generated"
#printf "\n***   Please update your 'masternode.conf' file with this new key:"
#printf "\n***"
#printf "\n***                $MNPKEY"
#printf "\n***"
#printf "\n************************************************************************\n\n"

##---Restart H2O Masternode
echo "Trying to restart masternode..."
#if  [ "$USESYSTEMD" -eq "1" ];
if  [ "$USESYSTEMD" > 0 ];
then
    echo $SVCNAME
    $SUDO systemctl start $SVCNAME
else
    $BINPATH\h2od
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
        exit 0
fi

sleep 5
echo "Syncing...";
echo "Take a coffee break this will take a few minutes maybe more...";
until h2o-cli mnsync status | grep -m 1 '"IsSynced": true'; do sleep 5; done > /dev/null 2>&1
echo "Sync complete. You masternode is running!!";


printf "\n************************************************************************"
printf "\n***"
printf "\n***   Start your masternode in your collateral wallet."
printf "\n***   You can do this by clicking start alias"
printf "\n***   Once started runing h2o-cli masternode status should ouput:"
printf "\n***   Masternode successfully activated"
printf "\n***"
printf "\n***"
printf "\n************************************************************************\n\n"

printf "\n\nUse\n\th2o-cli mnsync status\n"
printf "and\n\th2o-cli masternode status\n"
printf "to verify if masternode is active"
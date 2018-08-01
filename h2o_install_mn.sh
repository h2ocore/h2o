#!/bin/bash
if [ $(id -u) -ne 0 ]
then
  SUDO='sudo'
else
  SUDO=''
fi

echo "Installing required packages";

#Swap part
$SUDO dd if=/dev/zero of=/mnt/myswap.swap bs=1M count=4000
$SUDO mkswap /mnt/myswap.swap
$SUDO chmod 0600 /mnt/myswap.swap
$SUDO swapon /mnt/myswap.swap

$SUDO add-apt-repository ppa:bitcoin/bitcoin -y
$SUDO apt-get -y update
$SUDO apt-get -y upgrade
$SUDO apt-get -y dist-upgrade
$SUDO apt-get -y install git curl nano wget pwgen
$SUDO apt-get -y install build-essential libtool automake autoconf autotools-dev autoconf pkg-config libssl-dev libgmp3-dev libevent-dev bsdmainutils libboost-all-dev libzmq3-dev libminiupnpc-dev libdb4.8-dev libdb4.8++-dev
$SUDO apt-get -y update

echo "Done installing";
YOURIP=$(curl -s4 api.ipify.org)
PSS=$(pwgen -1 20 -n)

cd $HOME
echo "Getting H2O client";
mkdir $HOME/h2o
git clone https://github.com/h2ocore/h2o h2o
cd $HOME/h2o
chmod +x autogen.sh
chmod +x share/genbuild.sh
./autogen.sh
./configure --disable-tests --disable-gui-tests
make
$SUDO make install

echo "In order to proceed with the installation, please paste Masternode genkey by clicking right mouse button. Once masternode genkey is visible in the terminal please hit ENTER.";
read MNKEY

mkdir $HOME/.h2ocore

echo "rpcuser=user"                   > /$HOME/.h2ocore/h2o.conf
echo "rpcpassword=$PSS"              >> /$HOME/.h2ocore/h2o.conf
echo "rpcallowip=127.0.0.1"          >> /$HOME/.h2ocore/h2o.conf
echo "maxconnections=500"            >> /$HOME/.h2ocore/h2o.conf
echo "daemon=1"                      >> /$HOME/.h2ocore/h2o.conf
echo "server=1"                      >> /$HOME/.h2ocore/h2o.conf
echo "listen=1"                      >> /$HOME/.h2ocore/h2o.conf
echo "rpcport=13356"                 >> /$HOME/.h2ocore/h2o.conf
echo "externalip=$YOURIP:13355"      >> /$HOME/.h2ocore/h2o.conf
echo "masternodeprivkey=$MNKEY"      >> /$HOME/.h2ocore/h2o.conf
echo "masternode=1"                  >> /$HOME/.h2ocore/h2o.conf
echo " "                             >> /$HOME/.h2ocore/h2o.conf
echo "addnode=108.61.219.28:13355"   >> /$HOME/.h2ocore/h2o.conf
echo "addnode=140.82.52.45:13355"    >> /$HOME/.h2ocore/h2o.conf
echo "addnode=104.207.145.111:13355" >> /$HOME/.h2ocore/h2o.conf
echo "addnode=80.210.127.1:13355"    >> /$HOME/.h2ocore/h2o.conf
echo "addnode=80.210.127.2:13355"    >> /$HOME/.h2ocore/h2o.conf
echo "addnode=80.210.127.3:13355"    >> /$HOME/.h2ocore/h2o.conf

echo "Starting H2O client";
h2od --daemon
sleep 5
echo "Syncing...";
until h2o-cli mnsync status | grep -m 1 '"IsSynced": true'; do sleep 1 ; done > /dev/null 2>&1
echo "Sync complete. You masternode is running!! you can start your masternode later with: h2od --daemon";111
echo "You can stop your masternode with: h2o-cli stop"

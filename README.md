H2O 0.12.1.8

===============================

Website: http://h2ocore.org/ 

http://h2oproject.co (old website, do not use)

Masternode Install Guide: http://h2ocore.org/H2O_CNode.pdf

Install Script: 
cd
wget https://raw.githubusercontent.com/h2ocore/h2o/master/h2o_install_mn.sh
chmod +x h2o_install_mn.sh
./h2o_install_mn.sh

Upgrade Script: 
cd
wget https://raw.githubusercontent.com/h2ocore/h2o/master/h2o_upgrade_mn.sh
chmod +x h2o_upgrade_mn.sh
./h2o_upgrade_mn.sh


H2o coin Mining Pools
ArcPool: https://arcpool.com/explorer/H2O
Minepool.online: https://ihostmn.com/


VPS Suggestions:
Vultr:       https://goo.gl/3PDyPy
Linode:    https://goo.gl/xiJj1j
Google VPS get $300 credit:    https://goo.gl/RuRybL
ArubaCloud:   https://goo.gl/Es1TzW


Few words about H2O project
---------------------------

H2O coin is the first cryptocurrency with its aim on improving water management and incrase awareness of water-related problems around the world. 
In H2O project we have identified 4 fields of improvement:

-Delivering clean water for drinking and sanitation to people in need
-Cleaning oceans from plastic waste that kills wildlife
-Preventing underground water pollution by corporation and factories
-Increasing awareness among developed nations to save and respect water


*****Update July 22nd 2018****
Very exciting!!! We have released our first major update to the network. This release has some minor bug fixes and some major changes. 
This update will allow us to use sporks to update the masternode collateral from 1 to 15k.
We will also be changing the rewards from 300 to 150 at block 65k. 
Other things updated are the DNS seed and enabled IPv6 compatibility. 
We apprecaite the chance to be able to serve the community!
ENJOY YOUR NEW WALLET!!!

*****Update June 3rd 2018****
In an effort to make this coin a success and allow it reach it's true potential the community has taken control of this code.
We will be releasing more information about this coin soon.

Build
-------

Dependencies

	 git curl wget pwgen build-essential libtool automake autoconf autotools-dev autoconf pkg-config libssl-dev 
	 libgmp3-dev libevent-dev bsdmainutils libboost-all-dev libzmq3-dev libminiupnpc-dev

Install Berkley DB 4.8

	wget http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz
	tar -xzvf db-4.8.30.NC.tar.gz && cd db-4.8.30.NC/build_unix
	../dist/configure --enable-cxx
	make &&	sudo make install
	cd ../../ .. && rm  db-4.8.30.NC


License
-------

H2O Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

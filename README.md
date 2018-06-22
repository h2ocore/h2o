H2O 0.12.1.5
===============================

http://h2oproject.co (old website)


Few words about H2O project
---------------------------

H2O coin is the first cryptocurrency with its aim on improving water management and incrase awareness of water-related problems around the world. 
In H2O project we have identified 4 fields of improvement:

-Delivering clean water for drinking and sanitation to people in need
-Cleaning oceans from plastic waste that kills wildlife
-Preventing underground water pollution by corporation and factories
-Increasing awareness among developed nations to save and respect water

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

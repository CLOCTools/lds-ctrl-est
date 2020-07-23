
sudo apt-get install liblapack-dev
sudo apt-get install libblas-dev
sudo apt-get install libboost-dev

wget http://sourceforge.net/projects/arma/files/armadillo-9.900.2.tar.xz
tar -xvf armadillo-9.900.2.tar.xz
cd armadillo-9.900.2
./configure
make
sudo make install
cd ..

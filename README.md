# opti.coms
DataFox library-bindings for python3.

## Requirements

    sudo apt-get install libpython3-dev python3-dev libboost-python-dev
    sudo pip3 install python3-pika

## Build & Install

    cd module
    python3 setup.py build
    sudo python3 setup.py install
    cd ..

## Test

    ./examples/direct/get_serial.py

## Use

    terminal1 $ ./bin/identitycard-transmission-worker.py
    
    terminal2 $ ./examples/queue/transmit-and-wait-for-response.py

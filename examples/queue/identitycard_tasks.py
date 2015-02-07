#!/usr/bin/env python
import pika, json, logging

logging.basicConfig(
    level=logging.INFO,
    format="%(filename)s:%(lineno)d: %(message)s")

log = logging.getLogger('IdentityCardTransmitter')

log.info('connecting to message-broker')
connection = pika.BlockingConnection(
    pika.ConnectionParameters('localhost'))

log.info('declaring queue')
channel = connection.channel()
channel.queue_declare(queue='opticoms.transmit')

targetDevices = ['10.6.100.107',]
identitycardList = ['0030074700811', '0030074229747',]

log.info('publishing %d transmission-tasks with %d identity-cards each',
    len(targetDevices), len(identitycardList))

for targetDevice in targetDevices:
    msg = json.dumps({
        'targetDevice': targetDevice,
        'identitycardList': identitycardList,
        'ttl': 3
    })
    log.info('publishing message: %s', msg)
    channel.basic_publish(exchange='',
                          routing_key='opticoms-transmit',
                          body=msg)

log.info('closing connection')
connection.close()

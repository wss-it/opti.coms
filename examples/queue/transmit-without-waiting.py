#!/usr/bin/env python3
import pika, json, logging

logging.basicConfig(
    level=logging.INFO,
    format="%(filename)s:%(lineno)d: %(message)s")

log = logging.getLogger('IdentityCardTransmitter')

log.debug('connecting to message-broker')
connection = pika.BlockingConnection(
    pika.ConnectionParameters('localhost'))


log.debug('declaring queues')
channel = connection.channel()
channel.queue_declare(queue='opticoms.transmit', durable=True)
channel.queue_declare(queue='opticoms.transmit.reply')

targetDevices = ['10.6.100.107','10.6.100.108','10.6.100.109',]
identitycardList = [
    {'ID': '0030074700811', 'Name': 'Peter Körner'},
    {'ID': '0030074229747', 'Name': 'Dennis Sepeur'}
]


log.info('publishing %d transmission-tasks with %d identity-cards each',
    len(targetDevices), len(identitycardList))

for targetDevice in targetDevices:
    body = {
        'targetDevice': targetDevice,
        'identitycardList': identitycardList,
        'retryCount': 3,
    }

    log.info('issuing task for: %s', body['targetDevice'])
    channel.basic_publish(exchange='',
                          routing_key='opticoms.transmit',
                          properties=pika.BasicProperties(
                            delivery_mode=2, # make message persistent
                            reply_to='opticoms.transmit.reply',
                            correlation_id=targetDevice,
                            content_type='application/json',
                          ),
                          body=json.dumps(body))

log.debug('closing connection')
connection.close()

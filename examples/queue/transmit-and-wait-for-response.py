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
response_queue = channel.queue_declare(exclusive=True).method.queue


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
                            reply_to=response_queue,
                            correlation_id=targetDevice,
                            content_type='application/json',
                          ),
                          body=json.dumps(body))

def handle_response(ch, method, properties, body):
    body = json.loads(body.decode('utf8'))

    if body['success']:
        log.info("received success-response for %s",
                 body['targetDevice'])
        targetDevices.remove(body['targetDevice'])

    elif 'retryCount' in body and body['retryCount'] > 0:
        log.info("received non-final fail-response for %s, %d tries remaining",
                 body['targetDevice'],
                 body['retryCount'])

    else:
        log.info("received final fail-response for %s",
                 body['targetDevice'])
        targetDevices.remove(body['targetDevice'])


    if len(targetDevices) == 0:
        channel.stop_consuming()


log.debug('setting up consumer')
channel.basic_consume(handle_response,
                      queue=response_queue,
                      no_ack=True)

log.debug('start consuming')
channel.start_consuming()

log.debug('closing connection')
connection.close()

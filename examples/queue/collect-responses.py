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
channel.queue_declare(queue='opticoms.transmit.reply')


def handle_response(ch, method, properties, body):
    body = json.loads(body.decode('utf8'))

    if body['success']:
        log.info("received success-response for %s",
                 body['targetDevice'])

    elif 'retryCount' in body and body['retryCount'] > 0:
        log.info("received non-final fail-response for %s, %d tries remaining",
                 body['targetDevice'],
                 body['retryCount'])

    else:
        log.info("received final fail-response for %s",
                 body['targetDevice'])




log.debug('setting up consumer')
channel.basic_consume(handle_response,
                      queue='opticoms.transmit.reply',
                      no_ack=True)

log.debug('start consuming')
channel.start_consuming()

log.debug('closing connection')
connection.close()

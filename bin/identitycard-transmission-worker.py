#!/usr/bin/env python3
import pika, json, logging, time, pydfcom

logging.basicConfig(
    level=logging.INFO,
    format="%(filename)s:%(lineno)d: %(message)s")


log = logging.getLogger('IdentityCardTransmitter')

log.debug('connecting to message-broker')
connection = pika.BlockingConnection(pika.ConnectionParameters(
        host='localhost'))


log.debug('declaring queue')
channel = connection.channel()
channel.queue_declare(queue='opticoms.transmit', durable=True)

def callback(ch, method, properties, body):
    body = json.loads(body.decode('utf8'))

    log.info("Received %r", body['targetDevice'])

    if 'retryCount' in body:
        body['retryCount'] = body['retryCount'] - 1

    success = False
    try:
        time.sleep(2)
        raise RuntimeError('bla')

        log.info('  connect(%s, …)', body['targetDevice'])
        pydfcom.connect(body['targetDevice'], 8000, 3000)

        log.info('  read_list_descriptions() = …')
        desc = pydfcom.read_list_descriptions();
        log.info('  read_list_descriptions() = %s', desc)

        log.info('  pydfcom.push_list(…)')
        pydfcom.push_list(
            0,           # List-ID
            desc[0],     # List-Description
            body['identitycardList'])  # Data

        log.info('  disconnect()')
        pydfcom.disconnect()
        success = True

    except:
        log.warn('  failed.')
        pass

    log.info("Processed %r", body['targetDevice'])
    ch.basic_ack(delivery_tag=method.delivery_tag)

    if not success and 'retryCount' in body and body['retryCount'] > 0:
        log.info('RetryCount is %d, re-injecing task', body['retryCount'])

        ch.basic_publish(exchange='',
                         routing_key='opticoms.transmit',
                         properties=properties,
                         body=json.dumps(body))


    response = {
        'targetDevice': body['targetDevice'],
        'success': success,
        'retryCount': body['retryCount'],
    }

    if(properties.reply_to):
        ch.basic_publish(exchange='',
                         routing_key=properties.reply_to,
                         properties=pika.BasicProperties(
                             correlation_id=properties.correlation_id,
                             content_type='application/json',
                         ),
                         body=json.dumps(response))

log.debug('setting qos')
channel.basic_qos(prefetch_count=1)

log.debug('setting up consumer')
channel.basic_consume(callback,
                      queue='opticoms.transmit')

log.debug('start consuming')
channel.start_consuming()

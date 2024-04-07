
const mqttClient = mqtt.connect(mqttOptions);

    // MQTT Message received event
    mqttClient.on('message', (topic, message) => {
        console.log(`Received message on topic: ${topic}, Message: ${message.toString()}`);
        // Handle incoming MQTT messages here
        if (topic === 'sensors/temperature') {
            const temperature = parseFloat(message.toString());
            document.getElementById('temperatureDisplay').innerText = `Temperature: ${temperature}`;
        } else if (topic === 'sensors/pir') {
            const pirState = parseInt(message.toString());
            document.getElementById('pirStateDisplay').innerText = `PIR State: ${pirState}`;
        }
    });
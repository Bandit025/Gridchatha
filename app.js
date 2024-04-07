const express = require('express');
const mqtt = require('mqtt');
const mysql = require('mysql2');
const app = express();
const port = 3000;
const path = require('path');
const http = require('http');
const fs = require('fs');


app.use(express.json());


// // MySQL database connection
const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: '',
    database: 'aun'
});


db.connect((err) => {
    if (err) {
        console.error('Error connecting to database:', err);
        return;
    }
    console.log('Connected to MySQL database');
});

// // MQTT Broker connection options
const mqttOptions = {
    host: '192.168.199.111', // MQTT Broker IP
    port: 1883, // MQTT Broker Port
    // username: 'your_mqtt_username', // Uncomment if MQTT broker requires authentication
    // password: 'your_mqtt_password', // Uncomment if MQTT broker requires authentication
};

// // Connect to MQTT Broker
const mqttClient = mqtt.connect(mqttOptions);

// // MQTT Message received event
mqttClient.on('message', (topic, message) => {
    console.log(`Received message on topic: ${topic}, Message: ${message.toString()}`);
    // Handle incoming MQTT messages here
    if (topic === 'sensors/temperature') {
        const temperature = parseFloat(message.toString());
        insertData(temperature);
    } else if (topic === 'sensors/pir') {
        const pirState = parseInt(message.toString());
        insertData(null, pirState);
    }
});
function sendTemperatureToClient(temperature) {
    // Send temperature data to all connected clients using SSE
    server.clients.forEach(client => {
        client.write(`data: ${temperature}\n\n`);
    });
}

// app.post('/api/insert/',(req,res) => {
//     const {temperature,pirState} = req.body;

//     const query = "INSERT INTO `data`(`temperature`, `pirState`) VALUES (?,?)";
//     db.query(query, [temperature, pirState], (err, results) => {
//         if (err) {
//             console.error("Error inserting data:", err);
//             res.status(500).json({error:"Internal Server Error"});
//         } 
//         res.json({
//             msg: "Data inserted successfully!",
//             insertedId: results.insertId
//         })
//         console.log("Data inserted successfully");
// })
// })

// app.post('/api/insert/',(req,res) => {})
function insertData(temperature = null, pirState = null) {
    if (temperature !== null || pirState !== null) {
        const query = "INSERT INTO `data`(`temperature`, `pirState`) VALUES (?,?)";
        db.query(query, [temperature, pirState], (err, results) => {
            if (err) {
                console.error("Error inserting data:", err);
                res.status(500).json({error:"Internal Server Error"});
            } 
        console.log("insert success");
        })
             
        
    }
}

// // Subscribe to MQTT topics
mqttClient.on('connect', () => {
    mqttClient.subscribe('sensors/temperature');
    mqttClient.subscribe('sensors/pir');
});


app.use(express.static(path.join(__dirname,"/public/")));


app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});




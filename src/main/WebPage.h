String webpage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LAWN MOWER BOT</title>
    <style>
        body {
            background-color: rgb(0, 0, 0);
            color: rgb(135, 255, 247);
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        header, footer {
            background-color: rgb(71, 92, 174);
            margin-right: 50px;
            opacity: 50%;
            width: 100%;
            height: 45px;
            display: flex;
            justify-content: space-around;
            align-items: center;
        }
        footer { 
            margin-top: 450px; 
            border-radius: 0 0 25px 25px; 
        }
        header { 
            border-radius: 25px 25px 0 0;
        }

        #motorspeedslider {
            appearance: none;
            background: none;
            cursor: pointer;
        }
        #modetracker {
            float: right;
        }
        #leftbox {
            float: left;
            background-color: rgb(7, 0, 100);
            opacity: 50%;
            height: 450px;
            width: 460px;
        }
        #buttonbox :hover { 
            background-color: rgb(80, 95, 224); 
        }
        #buttonbox :active { 
            background-color: rgb(10, 26, 172); 
            transform: scale(0.95); 
        }

        #left, #right, #backward, #forward { 
            background-color: rgb(43, 61, 226); 
            cursor: pointer; 
            padding: 50px; 
            border: none; 
        }
        #left, #right { 
            height: 250px; 
            width: 125px; 
            margin-top: 20px; 
        }
        #forward, #backward { 
            margin-right: 5px; 
            margin-left: 5px; 
            height: 120px; 
            width: 160px; 
        }

        #left {
            float: left;
            margin-left: 20px;
            margin-right: 0px;
        }
        #right {
            float: right;
            margin-right: 20px;
            margin-left: 0px;
        }
        #forward {
            float: top;
            margin-top: 20px;
            margin-bottom: 5px;
        }
        #backward {
            float: bottom;
            margin-top: 5px;
            margin-bottom: 20px;
        }

        #bodytext {
            float: left;
            padding: 15px;
        }
        .sliderbox {
            float: right;
            margin-right: 20px;
            padding-top: 15px;
        }
        #motorspeedslider { 
            width: 250px; 
        }
        
        #bladebutton {
            margin-left: 20px;
            width: 420px;
            height: 80px;
            background: rgb(43, 150, 226);
            border: none;
            cursor: pointer;
        }
        #bladebutton:active { 
            background: rgb(93, 166, 219); 
        }
        #bladebutton.pressed { 
            background: rgb(16, 87, 137); 
            color: rgb(135, 255, 247); 
        }

        .active {
            background-color: rgb(10, 26, 172) !important;
            transform: scale(0.95);
        }
        
        .connection-status {
            position: absolute;
            top: 10px;
            right: 10px;
            padding: 5px 10px;
            border-radius: 4px;
            font-size: 12px;
        }
        
        .connected {
            background-color: #DFF2BF;
            color: #4F8A10;
        }
        
        .disconnected {
            background-color: #FFBABA;
            color: #D8000C;
        }
        
        footer {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
            padding: 10px;
        }
    </style>
</head>
<body>
    <div id="connectionStatus" class="connection-status disconnected">
        Disconnected
    </div>
    
    <header> 
        <p> X: <span id='X'>0</span></p>
        <p> Y: <span id='Y'>0</span></p>
    </header>
    
    <div id='leftbox'>
        <div id='buttonbox'>
            <button id='forward'></button>
            <button id='left'></button>
            <button id='right'></button>
            <button id='backward'></button>
            <button id='bladebutton'></button>
        </div>
    </div>
    
    <footer>
        <div>RPM1: <span id='RPM_1'>0</span></div>
        <div>OBJECT DETECTED: <span id='OBJECT_DISTANCE'>0</span></div>
        <div>RPM2: <span id='RPM_2'>0</span></div>
        <div>STATUS: <span id='STATUS'></span></div>
    </footer>

    <script>
        document.addEventListener('DOMContentLoaded', () => {
            // Constants for better maintainability
            const CMD_PREFIX = 'CMD_';
            const CMD_SUFFIX_ON = '_on';
            const CMD_SUFFIX_OFF = '_off';
            const MOTORS_OFF_COMMAND = `${CMD_PREFIX}motors${CMD_SUFFIX_OFF}`;
            
            // WebSocket connection
            const socket = new WebSocket(`ws://${window.location.hostname}:81/`);
            const connectionStatus = document.getElementById('connectionStatus');
            
            // UI elements
            const buttons = {
                left: document.getElementById('left'),
                right: document.getElementById('right'),
                forward: document.getElementById('forward'),
                backward: document.getElementById('backward'),
                blade: document.getElementById('bladebutton')
            };
            
            const statusElements = {
                rpm1: document.getElementById('RPM_1'),
                rpm2: document.getElementById('RPM_2'),
                status: document.getElementById('STATUS'),
                objectDistance: document.getElementById('OBJECT_DISTANCE'),
                x: document.getElementById('X'),
                y: document.getElementById('Y')
            };

            // Key to button mapping
            const keyControls = {
                'w': 'forward',
                'a': 'left',
                's': 'backward',
                'd': 'right',
                'ArrowUp': 'forward',
                'ArrowLeft': 'left',
                'ArrowDown': 'backward',
                'ArrowRight': 'right'
            };

            // WebSocket event handlers
            socket.onopen = () => {
                console.log('WebSocket connection established');
                connectionStatus.textContent = 'Connected';
                connectionStatus.className = 'connection-status connected';
            };

            socket.onclose = () => {
                console.log('WebSocket connection closed');
                connectionStatus.textContent = 'Disconnected';
                connectionStatus.className = 'connection-status disconnected';
            };

            socket.onerror = (error) => {
                console.error('WebSocket error:', error);
                connectionStatus.textContent = 'Connection Error';
                connectionStatus.className = 'connection-status disconnected';
            };

            socket.onmessage = (event) => {
                try {
                    const messages = JSON.parse(event.data);
                    console.log('Received:', messages);

                    // Update status elements
                    updateStatusElement(messages, 'RPM_1', statusElements.rpm1);
                    updateStatusElement(messages, 'RPM_2', statusElements.rpm2);
                    updateStatusElement(messages, 'STATUS', statusElements.status);
                    updateStatusElement(messages, 'OBJ_DISTANCE', statusElements.objectDistance);
                    updateStatusElement(messages, 'X', statusElements.x);
                    updateStatusElement(messages, 'Y', statusElements.y);
                    
                } catch (error) {
                    console.error('Error parsing JSON:', error, 'Raw data:', event.data);
                }
            };

            // Blade button handler
            buttons.blade.addEventListener('click', () => {
                const isPressed = buttons.blade.classList.toggle('pressed');
                const command = isPressed ? 
                    `${CMD_PREFIX}blade${CMD_SUFFIX_ON}` : 
                    `${CMD_PREFIX}blade${CMD_SUFFIX_OFF}`;
                sendCommand(command);
            });

            // Movement button handlers
            Object.entries(buttons).forEach(([direction, element]) => {
                if (direction !== 'blade') {
                    element.addEventListener('mousedown', () => 
                        sendCommand(`${CMD_PREFIX}${direction}${CMD_SUFFIX_ON}`));
                    element.addEventListener('mouseup', () => 
                        sendCommand(MOTORS_OFF_COMMAND));
                }
            });

            // Keyboard event handlers
            document.addEventListener('keydown', (event) => {
                const direction = keyControls[event.key];
                if (direction && buttons[direction]) {
                    buttons[direction].classList.add('active');
                    sendCommand(`${CMD_PREFIX}${direction}${CMD_SUFFIX_ON}`);
                }
            });

            document.addEventListener('keyup', (event) => {
                const direction = keyControls[event.key];
                if (direction && buttons[direction]) {
                    buttons[direction].classList.remove('active');
                    sendCommand(MOTORS_OFF_COMMAND);
                }
            });

            /**
             * Helper function to send commands via WebSocket
             * @param {string} command - The command to send
             */
            function sendCommand(command) {
                if (socket.readyState === WebSocket.OPEN) {
                    socket.send(command);
                    console.log('Sent:', command);
                } else {
                    console.error('WebSocket is not open. Ready state:', socket.readyState);
                }
            }

            /**
             * Helper function to update status elements
             * @param {Object} data - The data object
             * @param {string} key - The key to look for in the data
             * @param {HTMLElement} element - The element to update
             */
            function updateStatusElement(data, key, element) {
                if (data[key] !== undefined) {
                    element.textContent = data[key];
                }
            }
        });
    </script>
</body>
</html>

)=====";

String webpage = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>LAWN MOWER BOT</title>
    </head>

    <style>
        body {
            background-color: rgb(0, 0, 0);
            color: rgb(135, 255, 247);
        }
        header,footer{
            background-color: rgb(71, 92, 174);
            margin-right: 50px;
            opacity: 50%;
            width: 100%;
            height: 45px;
        }
        footer { margin-top: 450px; border-radius: 0 0 25px 25px; }
        header { border-radius: 25px 25px 0 0;}

        #motorspeedslider{
            appearance :none;
            background: none;
            cursor: pointer;
        }
        #modetracker {
            float: right;
            float: top;
        }
        #leftbox{
            float:left;
            background-color: rgb(7, 0, 100);
            opacity: 50%;
            height: 450px;
            width: 460px;
        }
        #buttonbox :hover { background-color: rgb(80, 95, 224); }
        #buttonbox :active{ background-color: rgb(10, 26, 172); transform: scale(0.95); }

        #left,#right,#backward,#forward{ background-color: rgb(43, 61, 226); cursor: pointer; padding: 50px; border: none; }
        #left,#right{ height: 250px; width: 125px; margin-top: 20px; }
        #forward,#backward { margin-right: 5px; margin-left: 5px; height: 120px; width: 160px; }

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
        #motorspeedslider { width: 250px; }
        
        #bladebutton {
            margin-left: 20px;
            width: 420px;
            height: 80px;
            background: rgb(43, 150, 226);
            border: none;
            cursor: pointer;
        }
        #bladebutton:active { background: rgb(93, 166, 219); }
        #bladebutton.pressed { background: rgb(16, 87, 137); color: rgb(135, 255, 247); }

    </style>

    <body>
        <header> 
        </header>
        <div id='leftbox'>
            <div id='buttonbox'>
                <button id='left'></button>
                <button id='right'></button>
                <button id='forward'></button>
                <button id='backward'></button>
                <button id='bladebutton'></button>
            </div>
            <div class="container">
                <input type='range' id='motorspeedslider' min='0' max='100' value ='50'>
                <div id='output'>0</div>
            </div>
        </div>
        <footer></footer>
    </body>

    <script>
        document.addEventListener('DOMContentLoaded', () => {
            const Socket = new WebSocket(`ws://${window.location.hostname}:81/`);
            const buttons = {                                   // press buttons on screen
                left: document.getElementById('left'),
                right: document.getElementById('right'),
                forward: document.getElementById('forward'),
                backward: document.getElementById('backward'),
                blade: document.getElementById('bladebutton')
            };

            const keyControls = { // Key to button mapping (WASD and arrow keys)
                'w': 'forward',
                'a': 'left',
                's': 'backward',
                'd': 'right',
                'ArrowUp': 'forward',
                'ArrowLeft': 'left',
                'ArrowDown': 'backward',
                'ArrowRight': 'right'
            };

            buttons.blade.addEventListener('click', () => {
                const isPressed = buttons.blade.classList.toggle('pressed');
                const cmd = isPressed ? 'CMD_blade_on' : 'CMD_blade_off';
                sendCommand(cmd);
            });

            Object.entries(buttons).forEach(([direction, element]) => {
                element.addEventListener('mousedown', () => sendCommand(`CMD_${direction}_on`));
                element.addEventListener('mouseup', () => sendCommand('CMD_motors_off'));
            });

            document.addEventListener('keydown', (e) => { // Keyboard event listeners
                const direction = keyControls[e.key];
                if (direction) {
                    buttons[direction].classList.add('active');
                    sendCommand(`CMD_${direction}_on`);
                }
            });
            document.addEventListener('keyup', (e) => {
                const direction = keyControls[e.key];
                if (direction) {
                    buttons[direction].classList.remove('active');
                    sendCommand('CMD_motors_off');
                }
            });

            Socket.onmessage = (event) => {     // Handle WebSocket messages
                Object.values(buttons).forEach(btn => btn.innerHTML = event.data);
                console.log(event.data);
            };

            function sendCommand(cmd) {     // Helper function to send commands
                if (Socket.readyState === WebSocket.OPEN) {
                    Socket.send(cmd);
                }
            }

            // Add some CSS for visual feedback
            const style = document.createElement('style');
            style.textContent = `
                .active {
                    background-color: rgb(10, 26, 172) !important;
                    transform: scale(0.95);
                }
            `;

            document.head.appendChild(style);
        });
    </script>
</html>


)=====";


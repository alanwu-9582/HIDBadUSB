#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* webpage = R"=====(

<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32-WirelessUSB Dashboard</title>
    <script>
        var Constants = {
            waitPrefix: "WAS/",
            waitHead: "WT/",
            waitSuffix: "WAE/",
            connamdEnd: ";;",
            multiKeyPartion: "--",
            multiKeyPrefix: "$",
            mousePrefix: "@",
            mediaPrefix: "MDI/",
            systemPrefix: "SYS/"
        };

        var Joystick = {
            active: false,
            center: { x: 150, y: 150 },
            radius: 100,
            stickRadius: 30,
            stickPos: { x: 150, y: 150 },
            lastStickPos: { x: 150, y: 150 },
            commandInterval: null,

            initiJoysitck: function() {
                var multiplierDisplay = document.getElementById("multiplierValue");
                var multiplierInput = document.getElementById("joystickMultiplier");
                multiplierInput.oninput = function() {
                    multiplierDisplay.textContent = this.value;
                };

                this.draw();
                var canvas = document.getElementById("joystick");
                canvas.addEventListener("mousedown", function(e) { Joystick.onMouseDown(e); });
                canvas.addEventListener("mousemove", function(e) { Joystick.onMouseMove(e); });
                canvas.addEventListener("mouseup", function() { Joystick.onMouseUp(); });
                canvas.addEventListener("mouseleave", function() { Joystick.onMouseUp(); });

                canvas.addEventListener("touchstart", function(e) {
                    e.preventDefault();
                    Joystick.onMouseDown(e);
                });
                canvas.addEventListener("touchmove", function(e) {
                    e.preventDefault();
                    Joystick.onMouseMove(e);
                });
                canvas.addEventListener("touchend", function() { Joystick.onMouseUp(); });
            },

            draw: function() {
                var canvas = document.getElementById("joystick");
                var ctx = canvas.getContext("2d");
                ctx.clearRect(0, 0, canvas.width, canvas.height);

                ctx.beginPath();
                ctx.arc(this.center.x, this.center.y, this.radius, 0, 2 * Math.PI);
                ctx.fillStyle = "rgba(200, 200, 200, 0.5)";
                ctx.fill();
                ctx.stroke();

                ctx.beginPath();
                ctx.arc(this.stickPos.x, this.stickPos.y, this.stickRadius, 0, 2 * Math.PI);
                ctx.fillStyle = "rgba(100, 100, 100, 1)";
                ctx.fill();
                ctx.stroke();
            },

            move: function(e) {
                var canvas = document.getElementById("joystick");
                var rect = canvas.getBoundingClientRect();
                var x, y;

                if (e.touches) {
                    x = e.touches[0].clientX - rect.left;
                    y = e.touches[0].clientY - rect.top;
                } else {
                    x = e.clientX - rect.left;
                    y = e.clientY - rect.top;
                }

                var dx = x - this.center.x;
                var dy = y - this.center.y;
                var distance = Math.sqrt(dx * dx + dy * dy);

                if (distance > this.radius - this.stickRadius) {
                    var angle = Math.atan2(dy, dx);
                    this.stickPos.x = this.center.x + (this.radius - this.stickRadius) * Math.cos(angle);
                    this.stickPos.y = this.center.y + (this.radius - this.stickRadius) * Math.sin(angle);
                } else {
                    this.stickPos.x = x;
                    this.stickPos.y = y;
                }

                this.stickPos.x = this.lastStickPos.x + (this.stickPos.x - this.lastStickPos.x) * 0.1;
                this.stickPos.y = this.lastStickPos.y + (this.stickPos.y - this.lastStickPos.y) * 0.1;

                if (this.stickPos.x !== this.lastStickPos.x || this.stickPos.y !== this.lastStickPos.y) {
                    clearInterval(this.commandInterval);
                    this.commandInterval = null;
                }

                this.lastStickPos.x = this.stickPos.x;
                this.lastStickPos.y = this.stickPos.y;

                var normalizedX = (this.stickPos.x - this.center.x) / (this.radius - this.stickRadius);
                var normalizedY = (this.stickPos.y - this.center.y) / (this.radius - this.stickRadius);
                var multiplier = document.getElementById("joystickMultiplier").value;
                var command2Send = `${Constants.mousePrefix}MOVE${Math.round(normalizedX.toFixed(2) * 10 * multiplier)},${Math.round(normalizedY.toFixed(2) * 10 * multiplier)},0,0`;

                this.draw();

                if (this.active && !(normalizedX === 0 && normalizedY === 0)) {
                    if (!this.commandInterval) {
                        this.commandInterval = setInterval(function() {
                            // console.log(command2Send);
                            CommandSender.sendCommand(command2Send);
                        }, 10);
                    }
                }
            },

            onMouseDown: function(e) {
                this.active = true;
                this.move(e);
            },

            onMouseMove: function(e) {
                if (this.active == true) {
                    this.move(e);
                }
            },

            onMouseUp: function() {
                clearInterval(this.commandInterval);
                this.commandInterval = null;
                if (this.active) CommandSender.sendCommand('@MOVE0,0,0,0');
                this.active = false;
                this.stickPos.x = this.center.x;
                this.stickPos.y = this.center.y;
                this.draw();
            }
        };

        var Slider = {
            scrolling: false,
            touchStartY: 0,

            initSlider : function() {
                var sliderSimulator = document.getElementById('sliderSimulator')
                sliderSimulator.addEventListener('wheel', function(e) { Slider.simulateScroll(e); });
                sliderSimulator.addEventListener('touchstart', function(e) { Slider.handleTouchStart(e); });
                sliderSimulator.addEventListener('touchmove', function(e) { Slider.handleTouchMove(e); });
            },

            simulateScroll: function(e) {
                e.preventDefault();
                var sliderSimulator = document.getElementById('sliderSimulator')
                
                var multiplier = document.getElementById("joystickMultiplier").value;
                var direction = e.deltaY > 0 ? 1 : -1;
                CommandSender.sendCommand(`${Constants.mousePrefix}MOVE0,0,${direction * 10 * multiplier},0`)

                if (!this.scrolling) {
                    this.scrolling = true;

                    if (e.deltaY > 0) {
                        sliderSimulator.classList.add('scrolling-down');
                        sliderSimulator.classList.remove('scrolling-up');
                    } else {
                        sliderSimulator.classList.add('scrolling-up');
                        sliderSimulator.classList.remove('scrolling-down');
                    }

                    clearTimeout(sliderSimulator.stopScroll);
                    sliderSimulator.stopScroll = setTimeout(() => {
                        this.scrolling = false;
                        sliderSimulator.classList.remove('scrolling-up', 'scrolling-down');
                    }, 300);
                }
            },

            
            handleTouchStart: function(e) {
                e.preventDefault()
                this.touchStartY = e.touches[0].clientY;
            },

            handleTouchMove: function(e) {
                e.preventDefault()
                if (!this.touchStartY) {
                    return;
                }

                const touchY = e.touches[0].clientY;
                const deltaY = this.touchStartY - touchY;

                this.simulateScroll({ deltaY: deltaY });

                this.touchStartY = touchY;
                alert("touchMove")
            }
        }

        var StatusBar = {
            update: function(status) {
                document.getElementById("response").innerHTML = status;
            },

            refreshICState: function() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4) {
                        if (this.status == 200) {
                            var response = this.responseText.split(",");
                            
                            document.getElementById("ICTempValue").textContent = response[0];
                            document.getElementById("heapValue").textContent = response[1];
                            document.getElementById("uptimeValue").textContent = response[2];
                        }
                    }
                };
                xhttp.open("GET", "/icstate", true);
                xhttp.send();
            }
        };

        var CommandSender = {
            sendHttpRequest: function(command) {
                return new Promise((resolve, reject) => {
                    var xhttp = new XMLHttpRequest();
                    xhttp.onreadystatechange = function() {
                        if (this.readyState == 4) {
                            if (this.status == 200) {
                                StatusBar.update(this.responseText);
                            }
                        }
                    };
                    xhttp.open("GET", "/command?commandInput=" + command, true);
                    xhttp.send();

                    resolve();
                });
            },

            sendCommand:async  function(command) {
                var commandInput = command || document.getElementsByName("commandInput")[0].value;
                
                var commands = commandInput.split(new RegExp(Constants.waitPrefix + "|" + Constants.waitSuffix)).filter(Boolean);
                // console.log(commands);
                for (let cmd of commands) {
                    if (cmd.startsWith(Constants.waitHead)) {
                        var time = parseInt(cmd.substring(3).trim());
                        StatusBar.update("Waiting for " + time + "ms...");
                        await new Promise(resolve => setTimeout(resolve, time));


                    } else {
                        StatusBar.update("Sending command: " + cmd);
                        await this.sendHttpRequest(cmd);
                    }
             }   

            },

            sendFromSelect: function() {
                var select = document.getElementById("scriptDropdown");
                var selectedValue = select.value;
                this.sendCommand(selectedValue);
            },

            refreshICState: function() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4) {
                        if (this.status == 200) {
                            StatusBar.update();
                        }
                    }
                };
                xhttp.open("GET", "/command?commandInput=" + command, true);
                xhttp.send();

                resolve();
            }
        };

        var TextInserter = {
            insert: function(text) {
                var inputField = document.getElementsByName("commandInput")[0];
                inputField.value += text;
            },

            insertFromSelect: function(from) {
                var select = document.getElementById(from);
                var selectedValue = select.value;
                this.insert(selectedValue);
            },

            addWaitCommand: function () {
                var waitTime = document.getElementById("waitTime").value;
                this.insert(Constants.waitPrefix + Constants.waitHead + waitTime + Constants.waitSuffix);
            },

            clear : function() {
                document.getElementsByName("commandInput")[0].value = "";
            }
        };

        var KeyTable = {
            tableHeader : [
                `KEY ${Constants.multiKeyPrefix}`, 
                `MOUSE ${Constants.mousePrefix}`, 
                `MEDIA ${Constants.mediaPrefix}`, 
                `SYSTEM ${Constants.systemPrefix}`
            ],

            tableData : [
                ["LEFT_CTRL", "LEFT_SHIFT", "LEFT_ALT", "LEFT_GUI", "RIGHT_CTRL", "RIGHT_SHIFT", "RIGHT_ALT", "RIGHT_GUI", "UP_ARROW", "DOWN_ARROW", "LEFT_ARROW", "RIGHT_ARROW", "MENU", "SPACE", "BACKSPACE", "TAB", "RETURN", "ESC", "INSERT", "DELETE", "PAGE_UP", "PAGE_DOWN", "HOME", "END", "NUM_LOCK", "CAPS_LOCK", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "PRINT_SCREEN", "SCROLL_LOCK", "PAUSE"],
                ["LEFT", "RIGHT", "MIDDLE", "BACKWARD", "FORWARD", "ALL"],
                ["VOLUME_UP", "VOLUME_DOWN", "MUTE", "PLAY_PAUSE", "STOP", "BRIGHTNESS_UP", "BRIGHTNESS_DOWN"],
                ["POWER_OFF", "STANDBY", "WAKE_HOST"]
            ],

            initTable : function() {
                var table = document.createElement("table");
                var thead = document.createElement("thead");
                var tbody = document.createElement("tbody");

                var tr = document.createElement("tr");
                this.tableHeader.forEach(function(header) {
                    var th = document.createElement("th");
                    th.textContent = header;
                    tr.appendChild(th);
                });
                thead.appendChild(tr); 
                table.appendChild(thead);

                var maxDataLength = Math.max(...this.tableData.map(arr => arr.length));

                for (var i = 0; i < maxDataLength; i++) {
                    var tr = document.createElement("tr");
                    for (var j = 0; j < this.tableHeader.length; j++) {
                        var td = document.createElement("td");

                        td.textContent = this.tableData[j][i] || "";;
                        td.type = this.tableHeader[j].split(" ")[1];

                        td.addEventListener("click", function() {
                            KeyTable.cellClicked(this)
                        });

                        tr.appendChild(td);
                    }
                    tbody.appendChild(tr);
                }

                table.appendChild(tbody);
                document.getElementById("keyTable").appendChild(table);
            },

            cellClicked : function(cell) {
                var instantRun = document.getElementById("InstantRun").checked;
                if (instantRun) CommandSender.sendCommand(cell.type + cell.textContent);
                else this.insertFromClicked(cell);
            },

            insertFromClicked : function(cell) {
                var currentText = document.getElementsByName("commandInput")[0].value;
                if (currentText.length < 1) {
                    TextInserter.insert(cell.type + cell.textContent);
                    return;
                }

                var currentCommand = currentText.split(new RegExp(Constants.waitPrefix + "|" + Constants.waitSuffix + "|;;"))
                var lastCommand = currentCommand[currentCommand.length - 1];
                new RegExp(Constants.waitPrefix + "|" + Constants.waitSuffix + "|;;")
                if (lastCommand.startsWith(Constants.multiKeyPrefix) && cell.type === Constants.multiKeyPrefix) {
                    TextInserter.insert(Constants.multiKeyPartion + cell.textContent);
                } else {
                    var prefix = lastCommand !== '' ? Constants.connamdEnd : '';
                    TextInserter.insert(prefix + cell.type + cell.textContent);
                }
            }
        };

        function initialize() {
            KeyTable.initTable();
            Joystick.initiJoysitck();
            Slider.initSlider();
        }

        window.onload = function() {
            initialize();
        };

        window.sendCommandFromExternal = function(command) {
            CommandSender.sendCommand(command).then(() => {
                console.log("Command sent successfully");
            }).catch(err => {
                console.error("Failed to send command:", err);
            });
        };
    </script>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding-top: 20px;
            padding-bottom: 20px;
        }
        
        h1 {
            text-align: center;
            color: #333;
            font-size: 2em;
        }
    
        .container {
            max-width: 800px;
            margin: 0.5em auto;
            background: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
            width: 100%;
        }

        .flex-container {
            display: flex;
            align-items: center;
            flex-wrap: wrap;
        }
    
        input[type="text"], input[type="number"], select {
            width: calc(100% - 22px);
            padding: 10px;
            margin: 5px 0;
            border: 1px solid #ccc;
            border-radius: 4px;
            font-size: 16px;
            transition: border 0.3s;
        }
    
        input[type="text"]:focus, input[type="number"]:focus, select:focus {
            border-color: #007bff;
            outline: none;
        }

        input[type="file"] {
            display: none;           
        }

        .custom-file-upload {
            display: inline-block;
            padding: 10px 20px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            transition: background-color 0.3s ease;
        }

        .custom-file-upload:hover {
            background-color: #45a049;
        }

        .file-name {
            margin: 10px;
            font-size: 14px;
            color: #555;
        }

        input[type="range"] {
            -webkit-appearance: none;
            width: 100%;
            height: 8px;
            background: #ddd;
            border-radius: 0;
            outline: none;
            transition: background 0.3s;
        }
    
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 30px;
            height: 15px;
            background: #2857a7;
            cursor: pointer;
            border-radius: 5px;
        }
    
        input[type="range"]::-moz-range-thumb {
            width: 20px;
            height: 8px;
            background: #2857a7;
            cursor: pointer;
        }
    
        input[type="range"]:hover {
            background: #ccc;
        }

        .switch-container {
            display: flex;
            align-items: center;
            gap: 1em;
            font-family: Arial, sans-serif;
            font-size: 1rem;
            margin-bottom: 1em;
        }

        .switch {
            position: relative;
            display: inline-block;
            width: 3.75em;
            height: 2.125em;
        }

        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
            border-radius: 2em;
        }

        .slider:before {
            position: absolute;
            content: "";
            height: 1.625em;
            width: 1.625em;
            left: 0.25em;
            bottom: 0.25em;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }

        input:checked + .slider {
            background-color: #2857a7;
        }

        input:checked + .slider:before {
            transform: translateX(1.625em);
        }
    
        button {
            background-color: #2857a7;
            color: white;
            border: none;
            padding: 10px 15px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            margin-right: 10px;
            transition: background-color 0.3s;
            width: auto;
        }
    
        button:hover {
            background-color: #282188;
        }

        #refreshButton {
            height: 100%;
            margin-left: auto;
        }
    
        #response {
            background-color: #e7f1ff;
            padding: 10px;
            border: 1px solid #cce5ff;
            border-radius: 4px;
            margin-top: 10px;
            white-space: pre-wrap;
        }
    
        #joystick {
            border-radius: 15px;
            background-color: #fff;
            margin-right: 20px;
        }
    
        .styled-label {
            font-family: 'Arial', sans-serif;
            font-size: 16px;
            color: #333;
            font-weight: bold;
            letter-spacing: 0.5px;
            display: inline-block;
        }

        select {
            width: calc(100%);
            padding: 10px;
            margin: 5px 0;
            border: 1px solid #ccc;
            border-radius: 4px;
            font-size: 16px;
            transition: border 0.3s;
        }
    
        table {
            width: 100%;
            border-collapse: collapse;
            border-radius: 10px;
            overflow: hidden;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }
        th, td {
            border: 1px solid #ddd;
            padding: 12px;
            text-align: left;
            transition: background-color 0.3s;
        }
        th {
            background-color: #2857a7;
            color: white;
        }
        tr:nth-child(even) {
            background-color: #f2f2f2;
        }
        tr:hover {
            background-color: #f1f1f1;
        }
        td {
            vertical-align: top;
        }

        .table-container {
            overflow-x: auto;
            max-height: 20em;
            overflow-y: auto; 
        }

        .slider-simulator-container {
            text-align: center;
            margin: 10px;
        }

        .slider-simulator {
            width: 40px;
            height: 300px;
            background-color: #2857a7;
            border-radius: 10px;
            margin: 10px auto;
            position: relative;
            transition: background-color 0.3s ease;
        }

        .scrolling-up {
            animation: slide-up 0.5s infinite;
        }

        .scrolling-down {
            animation: slide-down 0.5s infinite;
        }

        @keyframes slide-up {
            0% {
                background-color: #2857a7;
                transform: translateY(0);
            }
            25% {
                transform: translateY(-20px);
            }
            50% {
                background-color: #282188; 
                transform: translateY(-5px);
            }
            75% {
                transform: translateY(0);
            }
            100% {
                background-color: #2857a7;
                transform: translateY(0);
            }
        }

        @keyframes slide-down {
            0% {
                background-color: #2857a7;
                transform: translateY(0);
            }
            25% {
                transform: translateY(20px);
            }
            50% {
                background-color: #282188;
                transform: translateY(5px);
            }
            75% {
                transform: translateY(0);
            }
            100% {
                background-color: #2857a7;
                transform: translateY(0);
            }
        }
    
        @media (max-width: 600px) {
            h1 {
                font-size: 1.5em;
            }
    
            .container {
                padding: 15px;
            }
    
            button {
                width: 100%;
                margin: 5px 0;
            }
    
            input[type="text"], input[type="number"], select {
                font-size: 14px;
            }
        }
    
        @media (max-width: 400px) {
            h1 {
                font-size: 1.2em;
            }
    
            input[type="text"], input[type="number"], select {
                font-size: 12px;
            }
    
            button {
                padding: 8px 10px;
            }
        }
    </style>
    
</head>
<body>
    <h1>HID-BadUSB Dashboard</h1>

    <div id="commandBox" class="container">
        <label for="commandInput" class="styled-label">Command:</label>
        <input type="text" name="commandInput">
        <button id='sendCommandFromExternal' onclick="CommandSender.sendCommand();">Submit</button>
        <button onclick="TextInserter.clear()">Clear</button><br><br>
        <div id="response">Ready For Command</div><br>

        <select id="symbolDropdown">
            <option value=";;">CommandEnd</option>
            <option value="--">MultiKeyPartion</option>
            <option value="$">MultiKey</option>
            <option value="@">Mouse</option>
            <option value="MDI/">Media</option>
            <option value="SYS/">System</option>
        </select>
        <button onclick="TextInserter.insertFromSelect('symbolDropdown')">AddSymbol</button><br><br>
    </div>

    <div id="quickCommand" class="container"> 
        <div class="switch-container">
            <label class="switch">
                <input type="checkbox" id="InstantRun">
                <span class="slider"></span>
            </label>
            <label for="InstantRun" class="styled-label">InstantRun</label>
        </div>

        <div class="table-container"><div id="keyTable"></div></div>
        
    </div>

    <div id="scriptArea" class="container">
        <label for="waitTime" class="styled-label">Wait Time (ms):</label>
        <input type="number" id="waitTime" min="0" value="1000">
        <button onclick="TextInserter.addWaitCommand()">Add Wait Command</button><br><br>

        <select id="scriptDropdown">
            <option value="$LEFT_ALT--F4">CloseTab</option>
            <option value="$LEFT_GUI--rWAS/WT/100WAE/https://www.youtube.com/watch?v=dQw4w9WgXcQWAS/WT/500WAE/$RETURN">RickRoll</option>
        </select>
        <div class="flex-container">
            <button onclick="CommandSender.sendFromSelect()">RunScript</button>
            <label class="custom-file-upload">
                <input type="file" id="fileInput" accept=".json">
                Upload Script
            </label>
        
            <div id="fileName" class="file-name">No file selected</div>
        </div>
    </div>
    
    <div id="cursorControl" class="container">
        <label for="joystickMultiplier" class="styled-label">Cursor Speed:</label>
        <input type="range" id="joystickMultiplier" min="1" max="10" value="1">
        <span id="multiplierValue">1</span><br><br>
        
        <div class="flex-container">
            <canvas id="joystick" width="300" height="300" style="border:1px solid #000;"></canvas>
            <div class="slider-simulator-container">
                <div id="sliderSimulator" class="slider-simulator"></div>
            </div>
        </div>
    </div>

    <div id="ICstate" class="container">
        <div class="flex-container">
            <div>
                <p>IC Temperature: <span id="ICTempValue">Unknown</span> °C</p>
                <p>Free Heap: <span id="heapValue">Unknown</span> Bytes</p>
                <p>Uptime: <span id="uptimeValue">Unknown</span> Miliseconds</p>
            </div>
            
            <button id="refreshButton" onclick="StatusBar.refreshICState()">Refresh</button>
        </div>
    </div>

    <footer style="padding: 10px;">
        HID-BadUSB MIT License | <a href="https://github.com/alanwu-9582">alanwu-9582</a>
    </footer>
</body>
</html>


)=====";

#endif

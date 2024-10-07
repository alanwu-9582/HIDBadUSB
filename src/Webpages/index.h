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
            waitSuffix: "WAE/"
        };

        var Joystick = {
            active: false,
            center: { x: 150, y: 150 },
            radius: 100,
            stickRadius: 30,
            stickPos: { x: 150, y: 150 },
            lastStickPos: { x: 150, y: 150 },
            commandInterval: null,

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
                var command2Send = `@MOVE${Math.round(normalizedX.toFixed(2) * 10 * multiplier)},${Math.round(normalizedY.toFixed(2) * 10 * multiplier)},0,0`;

                this.draw();

                if (this.active && !(normalizedX === 0 && normalizedY === 0)) {
                    if (!this.commandInterval) {
                        this.commandInterval = setInterval(function() {
                            console.log(command2Send);
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
                CommandSender.sendCommand('@MOVE0,0,0,0');

                this.active = false;
                this.stickPos.x = this.center.x;
                this.stickPos.y = this.center.y;
                this.draw();
            }
        };

        var StatusBar = {
            update: function(status) {
                document.getElementById("response").innerHTML = status;
            }
        };

        var CommandSender = {
            sendHttpRequest: function(command) {
                return new Promise((resolve, reject) => { // 返回一個 Promise 以便可以使用 await
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
                console.log(commands);
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
                var select = document.getElementById("dropdown");
                var selectedValue = select.value;
                this.sendCommand(selectedValue);
            }
        };

        var TextInserter = {
            insert: function(text) {
                var inputField = document.getElementsByName("commandInput")[0];
                inputField.value += text;
            },

            insertFromSelect: function() {
                var select = document.getElementById("dropdown");
                var selectedValue = select.value;
                this.insert(selectedValue);
            },

            addWaitCommand: function () {
                var waitTime = document.getElementById("waitTime").value;
                this.insert(Constants.waitPrefix + Constants.waitHead + waitTime + Constants.waitSuffix);
            }
        };

        window.onload = function() {
            Joystick.draw();
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
            padding: 20px;
        }
        
        h1 {
            text-align: center;
            color: #333;
            font-size: 2em; /* 預設字體大小 */
        }
    
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
            width: 100%;
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
            width: auto; /* 根據內容自適應 */
        }
    
        button:hover {
            background-color: #282188;
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
            border-radius: 15px; /* 圓角 */
            background-color: #fff; /* 背景顏色 */
        }
    
        label {
            margin-top: 15px;
            display: block;
            font-size: 16px;
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
    <h1>ESP32-WirelessUSB Dashboard</h1>


    <input type="text" name="commandInput">
    <button id='sendCommandFromExternal' onclick="CommandSender.sendCommand();">Submit</button><br><br>
    <div id="response">Ready For Command</div><br>

    <button onclick="TextInserter.insert(';;')">CommandEnd</button>
    <button onclick="TextInserter.insert('--')">ActionEnd</button>
    <button onclick="TextInserter.insert('$')">MultiKey</button>
    <button onclick="TextInserter.insert('@')">Mouse</button>
    <button onclick="TextInserter.insert('MDI/')">Media</button>
    <button onclick="TextInserter.insert('SYS/')">System</button><br>

    <label for="waitTime">Wait Time (ms):</label>
    <input type="number" id="waitTime" min="0" value="1000">
    <button onclick="TextInserter.addWaitCommand()">Add Wait Command</button><br><br>

    <select id="dropdown">
        <option value="$LEFT_ALT--F4">CloseTab</option>
        <option value="$LEFT_GUI--rWAS/WT/100WAE/https://www.youtube.com/watch?v=dQw4w9WgXcQWAS/WT/500WAE/$RETURN">RickRoll</option>
        <option value="@LEFT">MOUSE_LEFT</option>
        <option value="$LEFT_GUI">WINDOWS</option>
        <option value="$LEFT_SHIFT">SHIFT</option>
        <option value="$LEFT_CTRL">CTRL</option>
    </select>
    <button onclick="TextInserter.insertFromSelect()">AddAction</button>
    <button onclick="CommandSender.sendFromSelect()">RunAction</button><br><br>

    <label for="joystickMultiplier">Cursor Speed:</label>
    <input type="range" id="joystickMultiplier" min="1" max="10" value="1">
    <span id="multiplierValue">1</span><br><br>
    
    <canvas id="joystick" width="300" height="300" style="border:1px solid #000;"></canvas><br><br>

    <table>
        <tr>
            <th>KEY</th>
            <th>MOUSE</th>
            <th>MEDIA</th>
            <th>SYSTEM</th>
        </tr>
        <tr>
            <td>
                LEFT_CTRL<br>
                LEFT_SHIFT<br>
                LEFT_ALT<br>
                LEFT_GUI<br>
                RIGHT_CTRL<br>
                RIGHT_SHIFT<br>
                RIGHT_ALT<br>
                RIGHT_GUI<br>
                UP_ARROW<br>
                DOWN_ARROW<br>
                LEFT_ARROW<br>
                RIGHT_ARROW<br>
                MENU<br>
                SPACE<br>
                BACKSPACE<br>
                TAB<br>
                RETURN<br>
                ESC<br>
                INSERT<br>
                DELETE<br>
                PAGE_UP<br>
                PAGE_DOWN<br>
                HOME<br>
                END<br>
                NUM_LOCK<br>
                CAPS_LOCK<br>
                F1<br>
                F2<br>
                F3<br>
                F4<br>
                F5<br>
                F6<br>
                F7<br>
                F8<br>
                F9<br>
                F10<br>
                F11<br>
                F12<br>
                F13<br>
                F14<br>
                F15<br>
                F16<br>
                F17<br>
                F18<br>
                F19<br>
                F20<br>
                F21<br>
                F22<br>
                F23<br>
                F24<br>
                PRINT_SCREEN<br>
                SCROLL_LOCK<br>
                PAUSE
            </td>
            <td>
                LEFT<br>
                RIGHT<br>
                MIDDLE<br>
                BACKWARD<br>
                FORWARD<br>
                ALL
            </td>
            <td>
                VOLUME_UP<br>
                VOLUME_DOWN<br>
                MUTE<br>
                PLAY_PAUSE<br>
                STOP<br>
                BRIGHTNESS_UP<br>
                BRIGHTNESS_DOWN
            </td>
            <td>
                POWER_OFF<br>
                STANDBY<br>
                WAKE_HOST
            </td>
        </tr>
    </table>

    <script>
        var multiplierInput = document.getElementById("joystickMultiplier");
        var multiplierDisplay = document.getElementById("multiplierValue");
        multiplierInput.oninput = function() {
            multiplierDisplay.textContent = this.value;
        };
    </script>
</body>
</html>


)=====";

#endif

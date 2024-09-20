#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* webpage = R"=====(

<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32-WirelessUSB Dashboard</title>
    <script>
        function sendCommand() {
            var commandInput = document.getElementsByName("commandInput")[0].value;
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    // 更新頁面中的某個元素，而不重新整理頁面
                    document.getElementById("response").innerHTML = this.responseText;
                }
            };
            // 發送 GET 請求到 /command 路徑，並將輸入參數附加到 URL 中
            xhttp.open("GET", "/command?commandInput=" + commandInput, true);
            xhttp.send();
            return false; // 防止表單提交並重新載入頁面
        }
    </script>
</head>
<body>
    <h1>ESP32-WirelessUSB Dashboard</h1>

    <form onsubmit="return sendCommand();">
        <input type="text" name="commandInput">
        <input type="submit" value="Submit">
    </form><br>

    <!-- 顯示伺服器回應 -->
    <div id="response"></div>

</body>
</html>

)=====";

#endif
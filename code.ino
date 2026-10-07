#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "WI-FI";

WebServer server(80);

#define L1 23
#define L2 22
#define L3 21
#define L4 19
#define FAN 18

String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta name="theme-color" content="#07111f">
<title>MH2 Smart Home</title>

<style>

*{
    box-sizing:border-box;
    margin:0;
    padding:0;
}

:root{
    --bg:#050914;
    --card:rgba(14,24,42,.72);
    --border:rgba(255,255,255,.08);
    --cyan:#00eaff;
    --blue:#3977ff;
    --green:#00ff9d;
    --text:#f4f8ff;
    --muted:#7d8ca5;
}

body{
    min-height:100vh;
    font-family:Arial,Helvetica,sans-serif;
    color:var(--text);
    background:
        radial-gradient(circle at 10% 10%,rgba(0,234,255,.13),transparent 28%),
        radial-gradient(circle at 90% 85%,rgba(57,119,255,.16),transparent 32%),
        linear-gradient(145deg,#030711,#07101e 50%,#050914);
    padding:20px;
}

.app{
    width:100%;
    max-width:900px;
    margin:auto;
}

.topbar{
    display:flex;
    justify-content:space-between;
    align-items:center;
    gap:15px;
    margin-bottom:22px;
}

.brand{
    display:flex;
    align-items:center;
    gap:13px;
}

.logo{
    width:52px;
    height:52px;
    border-radius:17px;
    display:flex;
    justify-content:center;
    align-items:center;
    font-size:26px;
    background:linear-gradient(135deg,var(--cyan),var(--blue));
    box-shadow:0 0 30px rgba(0,234,255,.2);
}

.brand h1{
    font-size:21px;
    letter-spacing:.3px;
}

.brand p{
    color:var(--muted);
    font-size:12px;
    margin-top:4px;
}

.connection{
    display:flex;
    align-items:center;
    gap:8px;
    padding:10px 14px;
    border:1px solid rgba(0,255,157,.18);
    border-radius:14px;
    background:rgba(0,255,157,.05);
    color:#9bffda;
    font-size:12px;
}

.connection-dot{
    width:8px;
    height:8px;
    border-radius:50%;
    background:var(--green);
    box-shadow:0 0 12px var(--green);
}

.hero{
    position:relative;
    overflow:hidden;
    padding:30px;
    margin-bottom:20px;
    border:1px solid var(--border);
    border-radius:26px;
    background:
        linear-gradient(135deg,rgba(0,234,255,.08),rgba(57,119,255,.04)),
        var(--card);
    backdrop-filter:blur(20px);
    box-shadow:0 25px 70px rgba(0,0,0,.35);
}

.hero::before{
    content:"";
    position:absolute;
    width:180px;
    height:180px;
    right:-70px;
    top:-80px;
    border-radius:50%;
    background:rgba(0,234,255,.12);
    filter:blur(30px);
}

.hero-content{
    position:relative;
    z-index:1;
}

.hero-label{
    color:var(--cyan);
    font-size:11px;
    font-weight:bold;
    letter-spacing:2px;
    text-transform:uppercase;
    margin-bottom:9px;
}

.hero h2{
    font-size:32px;
    margin-bottom:8px;
}

.hero p{
    color:var(--muted);
    font-size:14px;
    line-height:1.6;
    max-width:520px;
}

.stats{
    position:relative;
    z-index:1;
    display:grid;
    grid-template-columns:repeat(3,1fr);
    gap:12px;
    margin-top:25px;
}

.stat{
    padding:15px;
    border:1px solid var(--border);
    border-radius:16px;
    background:rgba(255,255,255,.035);
}

.stat span{
    display:block;
    color:var(--muted);
    font-size:11px;
    margin-bottom:6px;
}

.stat strong{
    font-size:20px;
}

.section-head{
    display:flex;
    align-items:center;
    justify-content:space-between;
    margin:26px 3px 14px;
}

.section-head h3{
    font-size:18px;
}

.section-head span{
    color:var(--muted);
    font-size:12px;
}

.controls{
    display:grid;
    grid-template-columns:repeat(2,1fr);
    gap:14px;
}

.device{
    position:relative;
    overflow:hidden;
    padding:20px;
    border:1px solid var(--border);
    border-radius:22px;
    background:var(--card);
    backdrop-filter:blur(18px);
    transition:.3s ease;
}

.device:hover{
    transform:translateY(-3px);
    border-color:rgba(0,234,255,.22);
}

.device.active{
    border-color:rgba(0,255,157,.35);
    background:
        linear-gradient(145deg,rgba(0,255,157,.11),rgba(0,234,255,.035)),
        var(--card);
    box-shadow:0 15px 40px rgba(0,255,157,.06);
}

.device-top{
    display:flex;
    justify-content:space-between;
    align-items:flex-start;
}

.device-info h4{
    font-size:16px;
    margin-bottom:6px;
}

.state{
    display:flex;
    align-items:center;
    gap:6px;
    color:var(--muted);
    font-size:11px;
}

.state-dot{
    width:6px;
    height:6px;
    border-radius:50%;
    background:#65738a;
}

.device.active .state{
    color:var(--green);
}

.device.active .state-dot{
    background:var(--green);
    box-shadow:0 0 10px var(--green);
}

.icon{
    width:48px;
    height:48px;
    display:flex;
    justify-content:center;
    align-items:center;
    border-radius:15px;
    background:#101a2d;
    font-size:23px;
    transition:.3s;
}

.device.active .icon{
    background:rgba(0,255,157,.14);
    box-shadow:0 0 25px rgba(0,255,157,.12);
}

.device-bottom{
    display:flex;
    align-items:center;
    justify-content:space-between;
    gap:15px;
    margin-top:25px;
}

.power{
    font-size:11px;
    color:var(--muted);
}

.toggle{
    position:relative;
    width:62px;
    height:34px;
    border:0;
    border-radius:50px;
    background:#1b263b;
    cursor:pointer;
    transition:.3s;
}

.toggle span{
    position:absolute;
    width:26px;
    height:26px;
    left:4px;
    top:4px;
    border-radius:50%;
    background:#7e8ca2;
    transition:.3s;
    box-shadow:0 3px 10px rgba(0,0,0,.3);
}

.device.active .toggle{
    background:var(--green);
}

.device.active .toggle span{
    left:32px;
    background:#03140d;
}

.fan-card{
    grid-column:1 / -1;
}

.loading{
    text-align:center;
    padding:30px;
    color:var(--muted);
}

.footer{
    margin-top:25px;
    padding:22px 10px;
    text-align:center;
    border-top:1px solid var(--border);
    color:#58677e;
    font-size:11px;
}

.footer strong{
    color:#8393aa;
}

.toast{
    position:fixed;
    left:50%;
    bottom:25px;
    transform:translate(-50%,20px);
    opacity:0;
    pointer-events:none;
    padding:12px 18px;
    border:1px solid rgba(255,255,255,.1);
    border-radius:14px;
    background:rgba(10,18,32,.94);
    backdrop-filter:blur(15px);
    color:#fff;
    font-size:12px;
    box-shadow:0 15px 40px rgba(0,0,0,.35);
    transition:.3s;
}

.toast.show{
    opacity:1;
    transform:translate(-50%,0);
}

@media(max-width:600px){

    body{
        padding:12px;
    }

    .topbar{
        align-items:flex-start;
    }

    .connection{
        padding:8px 10px;
    }

    .connection span{
        display:none;
    }

    .hero{
        padding:24px 20px;
        border-radius:22px;
    }

    .hero h2{
        font-size:27px;
    }

    .stats{
        gap:8px;
    }

    .stat{
        padding:12px 10px;
    }

    .stat strong{
        font-size:17px;
    }

    .controls{
        grid-template-columns:1fr;
    }

    .fan-card{
        grid-column:auto;
    }

}

@media(max-width:360px){

    .brand h1{
        font-size:18px;
    }

    .logo{
        width:46px;
        height:46px;
    }

    .hero h2{
        font-size:24px;
    }

}

</style>
</head>

<body>

<div class="app">

    <div class="topbar">

        <div class="brand">

            <div class="logo">🏠</div>

            <div>
                <h1>MH2 Smart Home</h1>
                <p>ESP32 Wireless Control System</p>
            </div>

        </div>

        <div class="connection">
            <span class="connection-dot"></span>
            <span>ESP32 Connected</span>
        </div>

    </div>


    <section class="hero">

        <div class="hero-content">

            <div class="hero-label">
                Control Center
            </div>

            <h2>Smart Home Dashboard</h2>

            <p>
                Control your lights and fan wirelessly
                through your ESP32 local network.
            </p>

        </div>

        <div class="stats">

            <div class="stat">
                <span>DEVICES</span>
                <strong>5</strong>
            </div>

            <div class="stat">
                <span>ACTIVE</span>
                <strong id="active-count">0</strong>
            </div>

            <div class="stat">
                <span>STATUS</span>
                <strong id="system-status">READY</strong>
            </div>

        </div>

    </section>


    <div class="section-head">

        <h3>Devices</h3>

        <span id="device-label">
            0 of 5 active
        </span>

    </div>


    <div class="controls">


        <div class="device" id="card-l1">

            <div class="device-top">

                <div class="device-info">

                    <h4>Light 1</h4>

                    <div class="state">
                        <span class="state-dot"></span>
                        <span id="state-l1">OFF</span>
                    </div>

                </div>

                <div class="icon">💡</div>

            </div>

            <div class="device-bottom">

                <div class="power">
                    GPIO 23
                </div>

                <button class="toggle" onclick="toggleDevice('l1')">
                    <span></span>
                </button>

            </div>

        </div>


        <div class="device" id="card-l2">

            <div class="device-top">

                <div class="device-info">

                    <h4>Light 2</h4>

                    <div class="state">
                        <span class="state-dot"></span>
                        <span id="state-l2">OFF</span>
                    </div>

                </div>

                <div class="icon">💡</div>

            </div>

            <div class="device-bottom">

                <div class="power">
                    GPIO 22
                </div>

                <button class="toggle" onclick="toggleDevice('l2')">
                    <span></span>
                </button>

            </div>

        </div>


        <div class="device" id="card-l3">

            <div class="device-top">

                <div class="device-info">

                    <h4>Light 3</h4>

                    <div class="state">
                        <span class="state-dot"></span>
                        <span id="state-l3">OFF</span>
                    </div>

                </div>

                <div class="icon">💡</div>

            </div>

            <div class="device-bottom">

                <div class="power">
                    GPIO 21
                </div>

                <button class="toggle" onclick="toggleDevice('l3')">
                    <span></span>
                </button>

            </div>

        </div>


        <div class="device" id="card-l4">

            <div class="device-top">

                <div class="device-info">

                    <h4>Light 4</h4>

                    <div class="state">
                        <span class="state-dot"></span>
                        <span id="state-l4">OFF</span>
                    </div>

                </div>

                <div class="icon">💡</div>

            </div>

            <div class="device-bottom">

                <div class="power">
                    GPIO 19
                </div>

                <button class="toggle" onclick="toggleDevice('l4')">
                    <span></span>
                </button>

            </div>

        </div>


        <div class="device fan-card" id="card-fan">

            <div class="device-top">

                <div class="device-info">

                    <h4>Cooling Fan</h4>

                    <div class="state">
                        <span class="state-dot"></span>
                        <span id="state-fan">OFF</span>
                    </div>

                </div>

                <div class="icon">🌀</div>

            </div>

            <div class="device-bottom">

                <div class="power">
                    GPIO 18
                </div>

                <button class="toggle" onclick="toggleDevice('fan')">
                    <span></span>
                </button>

            </div>

        </div>


    </div>


    <div class="footer">
        <strong>MH2 Smart Home</strong> • ESP32 Control System
    </div>

</div>


<div class="toast" id="toast">
    Device updated
</div>


<script>

const states = {
    l1:false,
    l2:false,
    l3:false,
    l4:false,
    fan:false
};

function toggleDevice(device){

    const card = document.getElementById("card-" + device);

    card.style.pointerEvents = "none";

    fetch("/" + device)

    .then(response => {

        if(!response.ok){
            throw new Error("Request failed");
        }

        states[device] = !states[device];

        updateUI(device);

        showToast(
            device.toUpperCase() +
            " turned " +
            (states[device] ? "ON" : "OFF")
        );

    })

    .catch(error => {

        console.error(error);

        document.getElementById("system-status").textContent = "ERROR";

        showToast("Connection failed");

        setTimeout(() => {
            document.getElementById("system-status").textContent = "READY";
        },2000);

    })

    .finally(() => {

        setTimeout(() => {
            card.style.pointerEvents = "auto";
        },200);

    });

}


function updateUI(device){

    const card =
        document.getElementById("card-" + device);

    const state =
        document.getElementById("state-" + device);

    if(states[device]){

        card.classList.add("active");

        state.textContent = "ON";

    }
    else{

        card.classList.remove("active");

        state.textContent = "OFF";

    }

    updateStats();

}


function updateStats(){

    let active = 0;

    Object.values(states).forEach(value => {

        if(value){
            active++;
        }

    });

    document.getElementById("active-count").textContent =
        active;

    document.getElementById("device-label").textContent =
        active + " of 5 active";

}


function showToast(message){

    const toast =
        document.getElementById("toast");

    toast.textContent = message;

    toast.classList.add("show");

    clearTimeout(window.toastTimer);

    window.toastTimer =
        setTimeout(() => {

            toast.classList.remove("show");

        },1800);

}

</script>

</body>
</html>
)rawliteral";


void setup(){

    Serial.begin(115200);

    pinMode(L1,OUTPUT);
    pinMode(L2,OUTPUT);
    pinMode(L3,OUTPUT);
    pinMode(L4,OUTPUT);
    pinMode(FAN,OUTPUT);

    digitalWrite(L1,LOW);
    digitalWrite(L2,LOW);
    digitalWrite(L3,LOW);
    digitalWrite(L4,LOW);
    digitalWrite(FAN,LOW);

    WiFi.softAP(ssid);

    Serial.println();
    Serial.println("==============================");
    Serial.println("       MH2 SMART HOME");
    Serial.println("==============================");
    Serial.print("WiFi Name: ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("==============================");

    server.on("/",[](){

        server.send(
            200,
            "text/html",
            html
        );

    });

    server.on("/l1",[](){

        digitalWrite(
            L1,
            !digitalRead(L1)
        );

        server.send(
            200,
            "text/plain",
            "OK"
        );

    });

    server.on("/l2",[](){

        digitalWrite(
            L2,
            !digitalRead(L2)
        );

        server.send(
            200,
            "text/plain",
            "OK"
        );

    });

    server.on("/l3",[](){

        digitalWrite(
            L3,
            !digitalRead(L3)
        );

        server.send(
            200,
            "text/plain",
            "OK"
        );

    });

    server.on("/l4",[](){

        digitalWrite(
            L4,
            !digitalRead(L4)
        );

        server.send(
            200,
            "text/plain",
            "OK"
        );

    });

    server.on("/fan",[](){

        digitalWrite(
            FAN,
            !digitalRead(FAN)
        );

        server.send(
            200,
            "text/plain",
            "OK"
        );

    });

    server.begin();

    Serial.println("Web Server Started!");

}


void loop(){

    server.handleClient();

}

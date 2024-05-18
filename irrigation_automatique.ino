#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char *ap_ssid = "bbi"; // Nom du point d'accès WiFi
const char *ap_password = "12345678"; // Mot de passe du point d'accès WiFi
IPAddress ap_ip(192, 168, 1, 10); // Adresse IP fixe pour le point d'accès WiFi

WebServer server(80);
const int dhtPin = 5; // Broche du capteur DHT11
DHT dht(dhtPin, DHT11);

const int pumpPin = 4; // Broche pour la pompe d'irrigation
const int humidityThreshold = 30; // Seuil d'humidité pour activer la pompe

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW); // Assurer que la pompe est éteinte au démarrage

  // Configuration du point d'accès WiFi
  WiFi.softAPConfig(ap_ip, ap_ip, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Point d'accès WiFi créé !");
  Serial.print("Adresse IP du point d'accès WiFi: ");
  Serial.println(ap_ip);

  // Définir les gestionnaires d'URL pour le serveur
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.begin();
  Serial.println("Serveur Web démarré !");
}

void handleRoot() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String htmlPage = R"=====(
<html lang="fr">
<head>
 <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <script>
    function updateData() {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
          var data = JSON.parse(xhr.responseText);
          document.getElementById('temperature').innerHTML = data.temperature + ' °C';
          document.getElementById('humidity').innerHTML = data.humidity + ' %';
        }
      };
      xhr.open('GET', '/data', true);
      xhr.send();
    }
    setInterval(updateData, 2000); // Actualisez les données toutes les 2 secondes
  </script>
</head>
<body>
    <section id="section-wrapper">
        <div class="box-wrapper">
            <div class="info-wrap">
                <h2 class="info-title">BIENVENUE</h2>
                <h3 class="info-sub-title">Arrosage Intelligent pour votre Jardin.<br> Economisez de l'eau, gardez un jardin sain <br> Et controlez tout à distance</h3><br>
            
            <br><br><br><br><br>
               
            </div>
            <div class="form-wrap">
                <form action="#" method="POST">
                    <h2 class="form-title">Smart Irrigation </h2>
                    <div class="form-fields">



                        <div class="form-group" style="padding: 19px;">
                          <span class="fname">Humidité:</span>
                          <span id="humidity">0 %</span>
                        </div>

                        <div class="form-group" style="padding: 19px;">
                            <span class="fname">Température:</span>
                            <span id="temperature">0 °C</span>
                        </div>
                       
                       
                    </div>
                </form>
            </div>
        </div>
    </section>
    <style>
        *{
    padding: 0;
    margin: 0;
    box-sizing: border-box;
}
.fa-moon
{
    display: none;
}
.fname
{
    margin: 20px;
}
body{background-image: url('pexels-photo-94847.jpeg');
            background-repeat: no-repeat;
            background-size: cover;
        }
body, input,textarea, a{
    font-family: 'Roboto', sans-serif;
}
#section-wrapper{
    width: 100%;
    padding: 60px;
    animation: slideBottom .6s ease both;
}
@keyframes slideBottom
              {
                0%  
                    {
                        opacity: 0;
                        transform: translateY(-90px);
                    }
                100%  
                    {
                        opacity: 1;
                        transform: translateY(0);
                    }
              }
.box-wrapper{
    position: relative;
    display: table;
    width:1100px;
    margin:auto;
    height: 441px;
    margin-top:35px;
    border-radius: 30px;
}
.info-wrap{
    width:35%;
    height: 350px;
    padding: 40px;
    float: left;
    display: block;
    border-radius: 30px 0px 0px 30px;
    background-image: linear-gradient(120deg, #84fab0 0%, #8fd3f4 100%);
    color: #fff;
}
.info-title{
    text-align: left;
    color: #000000;
    font-size: 28px;
    letter-spacing: 0.5px;
}

.info-sub-title{
    color: #000000;
    font-size: 18px;
    font-weight: 300;
    margin-top: 17px;
    letter-spacing: 0.5px;
    line-height: 26px;
}
.info-details {
    list-style: none;
    margin: 60px 0px;
}
.info-details li {
    margin-top: 25px;
    font-size: 18px;
    color: #fff;
}
.info-details li i {
    background: #F44770;
    padding: 12px;
    border-radius: 50%;
    margin-right: 5px;
}
h3
    {
        font-size: 23px;
        font-weight: bold;
    }
.info-details li a {
    color: #fff;
    text-decoration: none;
}
.info-details li a:hover {
    color: #F44770;
    
}
.social-icons {
    list-style: none;
    text-align: center;
    margin: 20px 0px;
    margin-top: -30px;
}
.social-icons li {
    display: inline-block;
}
.social-icons li i {
    background: #F44770;
    color: #fff;
    padding: 15px;
    font-size: 15px;
    border-radius: 22%;
    margin: 0px 5px;
    margin-top: -70px;
    cursor: pointer;
    transition: all .5s;
}
.social-icons li i:hover {
    background: #fff;
    color: #000000;
    transform: translateY(-10px) scale(1.1);
}
.form-wrap{
    width:65%;
    float: right;
    height: 350px;
    padding:40px 25px 35px 25px;
    border-radius: 0px 30px 30px 0px;
    background: #ecf0f3;
}
.form-title{
    text-align: center;
    margin-left: 23px;
    font-size: 28px;
    letter-spacing: 0.5px;
}
.form-fields{
    display: table;
    width: 100%;
    padding: 15px 5px 5px 5px;
}
.form-fields input,
.form-fields textarea{
    border: none;
    outline:none;
    background: none;
    font-size: 18px;
    color:#555;
    padding: 20px 10px 20px 5px;
    width:100%
}
.form-fields textarea{
    height: 150px;
    resize: none;
}
.form-group{
    width: 90%;
    padding: 0px 30px;
    margin: 2rem 0rem;
    border-radius: 25px;
    box-shadow: inset 8px 8px 8px #cbced1, inset -8px -8px 8px #ffffff;
}
.form-fields{
    text-align: center;
}
.submit-button{
    width: 96%;
    height: 60px;
    margin: 0px 12px;
    border-radius: 30px;
    font-size: 20px;;
    font-weight: 700;
    outline: none;
    border:none;
    cursor: pointer;
    color:#fff;
    text-align: center;
    background: #F44770;
    box-shadow: 3px 3px 8px #b1b1b1, -3px -3px 8px #ffffff;
    transition: .5s;
}
.submit-button:hover{
    background: #31279d;
    transform: translateY(-10px) scale(1.1);
}
/* Responsive css */
@media only screen and (max-width: 767px) {
    .box-wrapper{
        width: 100%;
    }

    .form-wrap{
        width: 100%;
        height: inherit;
        float: none;  
    }
    .info-wrap{
        border-radius: 30px 30px 0px 0px;
        height: 150px;
    }
    .form-wrap{
        border-radius: 0px 0px 30px 30px;
    }
    .form-group{
        width: 100%;
        float: none;
        margin: 25px 0px;
        margin-left: 0px;
    }
    .form-fields .form-group:last-child, 
    .submit-button{
        width: 100%;
    }
    .submit-button{
        margin: 10px 0px;
    }
   
}
@media (max-width:384px)
{
    #section-wrapper
        {
            margin-left: 9px;
            padding-left: 5px;
            
        }  
        footer ,header
        {
            width: 114vw;
            
        } 
}
    </style>
</body>
</html>
  )=====";
  server.send(200, "text/html", htmlPage);
}

void handleData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
  server.send(200, "application/json", jsonData);
}

void loop() {
  delay(2000); // Attendre quelques secondes entre chaque lecture

  // Lire la température et l'humidité depuis le capteur
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Vérifier si la lecture a réussi
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur lors de la lecture du capteur DHT11 !");
  } else {
    // Afficher les valeurs sur le moniteur série
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidite: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Contrôle de la pompe en fonction de l'humidité
    if (humidity < humidityThreshold) {
      Serial.println("Humidité faible, activation de la pompe.");
      digitalWrite(pumpPin, HIGH); // Allumer la pompe
      delay(60000); // Arroser pendant 1 minute
      digitalWrite(pumpPin, LOW); // Éteindre la pompe
      Serial.println("Pompe arrêtée après 1 minute.");
    }
  }

  server.handleClient();
}

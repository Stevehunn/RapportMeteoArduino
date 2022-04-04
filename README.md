# RapportMeteoArduino

Nous avons fait deux versions de code pour Arduino.
Dans la première version, les deux cartes utilisent une connexion wifi pour communiquer avec Thingspeak. Dans la seconde version, on utilise une communication Lora entre les deux Arduino pour que l'une d'entre elle transmet par la suite les données à Thingspeak.

Première version :

Le ficher tmpHumLum.ino est un code arduino pour récupérer les données des capteurs :
-Température
-Humidité
-Luminosité

Le ficher pollution.ino est un code arduino pour récupérer les données des capteurs :
-Pollution CO2
-Pollution TVOC

Deuxième version :

Le ficher recepteurLoRa.ino est un code arduino pour récupérer les données des capteurs :
-Température
-Humidité
-Luminosité

Le ficher poluLoRa.ino est un code arduino pour récupérer les données des capteurs :
-Pollution CO2
-Pollution TVOC


Lien pour télécharger l'apk de l'application Android :
https://mega.nz/file/9E9EnIJT#lxrkJHqQhtYDqUcRmGHgzgUwdpEaiQzUlDxbYzfNkrA

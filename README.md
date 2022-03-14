# README
Diese Anleitung beschreibt die nötigen Schritte zur Inbetriebnahme der Analyse-Lösung. Da es sich um mehrere unabhängige Komponenten handelt, werden verschiedene Applikationen genutzt.

Die zu untersuchende Applikation kann in den Quellcode des Analyse-Tools integriert werden.

```
quellcode/
├── pfyl.........................Datenerhebung & -sammlung
│   ├── lib
│   ├── src
│   │   ├── common
│   │   ├── device
│   │   │   ├── stm..............STM32F746 Referenzimplementierung
│   │   │   └── x86_64
│   │   │       └── uart.........UART Receiver
│   │   ├── processors
│   │   ├── sinks
│   │   └── sources
│   └── tests....................Tests
│       ├── processors
│       └── sinks
├── pfyl-analyzer................Datenverarbeitung
└── pfyl-viewer..................Visualisierung
```


## pfyl
pfyl (performant analysis) ist die Lösung die auf dem Embedded Device die Daten erhebt und an einen Host sendet. Der Quellcode befindet sich im Ordner `quellcode/pfyl`. Alle weiteren Pfade beziehen sich relativ zum genannten Projektpfad.

### Vorraussetzungen
Die Lösung wurde nur auf einem Linux Rechner kompiliert und getestet. In Klammern wird die mindestens benötigte Version angegeben. Folgende Programme sind zwingend erforderlich:
* cmake (3.1.4) 
* make
* gcc

Zum Testen wird folgendes benötigt:
* CppUTest (3.8) 

Für die Referenzimplementierung auf Basis des STM32F746NG wird benötigt:
* FreeRTOS
* STM32CubeF7 MCU Firmware Package (1.15)
* uGFX (2.8)

### Tests
Die Bibliothek wurde *test driven* entwickelt. Dieser Teil kann übersprungen werden, falls die Tests nicht gestartet werden sollen. Die Tests befinden sich in `quellcode/pfyl/tests`.
Um diese auszuführen muss im Ordner `quellcode/pfyl` folgendes ausgeführt werden:
```
cmake .
make
./test-pfyl
```
#### Konfiguration
In `quellcode/pfyl/CMakeLists.txt` muss der Pfad der Testing Bibliothek gesetzt werden. Dazu stehen folgende Variablen zur Verfügung:
```
set(CPPUTEST_PROJECT_DIR ${CppUTest_Project_LOCATION})
set(HOME_LIB_PATH ${CppUTest_LIB_LOCATION})
set(PFYL_HOME ${PROJECT_SOURCE_DIR})
```
Die ersten beiden Variablen müssen entsprechend gesetzt werden und hängen mit der CppUTest Bibliothek zusammen. 
PFYL_HOME muss nur gesetzt werden wenn das Projekt nicht aus dem Hauptordner kompiliert wird.

### STM32F746 Refenzimplementierung 
Um die Daten an einen Host senden zu können müssen sie zunächst auf einem Embedded Device erhoben werden. Um die Beispielapplikation kompilieren zu können müssen die o.g. Vorraussetzungen erfüllt werden und die Pfade zu den Abhängigkeiten korrekt gesetzt werden.
Der Quellcode befindet sich in `quellcode/pfyl/src/device/stm`
#### Konfiguration
In `quellcode/pfyl/src/device/stm` befindet sich die Datei CMakeLists.txt. Darin müssen die Pfade zu den Abhängigkeiten definiert werden:
```
set(PFYL_HOME ${PFYL_Base_Dir})
set(STM32Cube_DIR ${FirmwarePackagePath})
set(STM32BSP_INCLUDE_DIR ${BSPHeaderPath})
set(STM32BSP_SOURCES ${BSPSourcesPath})
set(uGFX_DIR ${PFYL_HOME}/lib/ugfx)
```
Um die Abhängigkeiten zu den Sources, Processors und Sinks auflösen zu können, muss PFYL_HOME auf den Projektpfad gesetzt werden.
Das heißt für PFYL_HOME muss der absolute Pfad von `quellcode/pfyl` angegeben werden.

#### Kompilierung
Ist die Konfiguration der Pfade abgeschloßen, kann die Applikation folgendermaßen kompiliert werden:
```
cd quellcode/pfyl/src/device/stm
cmake .
make
```
Anschließend kann die Binary `src/device/stm/f7-device` geflasht werden.

### Hinweise für die Integration als Bibliothek
<a name="bib"></a>
Eine weitere Variante ist das Kompilieren zu einer statischen Library und der späteren Integration in die Applikation. 
Es gilt jedoch zu beachten, dass der hier verwendete Kompilierprozess alle Abhängigkeiten und notwendigen *compiler flags* setzt um die Funktionalität des Analyse-Tools zu ermöglichen. Ein Beispiel dafür ist die `wrap` Option des GCC Linkers. Dies erlaubt das Substituieren der Speicheroperationen.


## UART Receiver
Um die Daten über UART zu empfangen muss die Applikation in `quellcode/pfyl/device/x86_64/uart` kompiliert werden. Dies ist zur Zeit nur unter Linux möglich, da die termios Bibliothek genutzt wird.

### Vorraussetzungen

#### InfluxDB Client
Um in die Zeitreihendatenbank schreiben zu können muss die Client Library kompiliert werden. Eine README mit einer Installationsbeschreibung  befindet sich in `quellcode/pfyl/src/lib/influxdb-cxx/README.md`. 
Benötigt werden folgende Abhängigkeiten:
* libcurl4-openssl-dev
* libcurl4

Die Liste enthält der Namen aus dem Debian Repository. Anschließend kann die Library kompiliert werden:
```
cd quellcode/pfyl/src/lib/influxdb-cxx
mkdir build
cd build
cmake ..
sudo make install
```

Außerdem muss die InfluxDB Datenbank installiert werden. Dazu [die folgende Anleitung](https://docs.influxdata.com/influxdb/v1.7/introduction/installation/) befolgen.

Die wichtigsten Instruktionen daraus für Ubuntu 18.04:
```
wget -qO- https://repos.influxdata.com/influxdb.key | sudo apt-key add -
source /etc/lsb-release

echo "deb https://repos.influxdata.com/${DISTRIB_ID,,} ${DISTRIB_CODENAME} \
    stable" | sudo tee /etc/apt/sources.list.d/influxdb.list

sudo apt-get update && sudo apt-get install influxdb
sudo service influxdb start
```

### Konfiguration
Ähnlich wie zuvor muss auch hier der absolute Basis-Projektpfad angegeben werden. 
In der Datei CMakeLists.txt in `quellcode/pfyl/device/x86_64/uart` muss der absolute Pfad von `quellcode/pfyl` gesetzt werden:
```
set(PFYL_HOME ${BASE_PROJECT_DIR})
```
In der Datei `quellcode/pfyl/device/x86_64/uart/uart_client.cpp` in Zeile 30-31 müssen folgende Variable korrekt gesetzt werden:
```
const char* dev = "/dev/ttyACM0";
const char *influxDbConnection = "http://localhost:8086/?db=mydb";
```
In der ersten Variable wird der File Descriptor Pfad zum Board angegeben und in der zweiten die HTTP-Schnittstelle zur InfluxDB, welche bereits installiert sein sollte.

#### UART Geschwindigkeit
In der Datei `quellcode/pfyl/device/x86_64/uart/uart_client.cpp` in Zeile 51-52 müssen folgende Variable korrekt gesetzt werden:
```
cfsetispeed(&SerialPortSettings, B1000000); // Lesegeschwindigkeit
cfsetospeed(&SerialPortSettings, B1000000); // Schreibgeschwindigkeit
```

### Kompilierung
```
cd quellcode/pfyl/device/x86_64/uart
cmake .
make
```
Das Binary befindet sich in `./cmake-build-debug/uart`.

## ITM
Die ITM Verarbeitung ist eine performante alternative zu UART. 
### Vorraussetzung
Folgende Abhängigkeiten werden benötigt:
* openOCD
* [libswo](https://gitlab.zapb.de/zapb/libswo)

Optional zum Anzeigen der Nachrichten:
* [swodec](https://gitlab.zapb.de/zapb/swodec)

### Starten von OpenOCD
Nun muss openOCD gestartet werden um alle ITM Ports auszulesen. 
```
openocd
    -f board/stm32f7discovery.cfg
    -c gdb_port 3333
    -c telnet_port 4444
    -c tcl_port 6666
    -c "tpiu config internal itm.fifo uart off 168000000"
    -c "itm ports 1
```
Die Datei itm.fifo wird nun mit dem Inhalt aus den ITM ports beschrieben. Diese Daten werden vom pfyl-analyzer ausgewertet. 

## pfyl-analyzer
Die Datenverarbeitung erfolgt mittels einer Python Server Andwendung, die sowohl die Daten verarbeitet als auch über eine HTTP-Schnittstelle zur Verfügung stellt. Der Quellcode befindet sich in `quellcode/pfyl-analyzer`
### Vorraussetzung
Folgende Anwendungen werden benötigt:
* python3 (3.6)
* mysql

Die mysql Datenbank wird für das Persistieren der statischen Analysedaten benötigt.

Es wird die Verwendung von `pip` empfohlen. Mit `pip install ${package_name}` können die folgenden erforderlichen Pakete installiert werden: 
* flask
* influxdb
* matplotlib
* mysql-connector-python

### Konfiguration
In der Datei `quellcode/pfyl-analyzer/src/server.py` müssen in Zeile 12-13 die korrekten Pfade gesetzt werden:
```
TOOLCHAIN_PATH = "" # absoluter Pfad zur GCC ARM Toolchain
BINARY_PATH = "" # absoluter Pfad von quellcode/pfyl/src/device/stm/f7-device
``` 

Außerdem müssen in `quellcode/pfyl-analyzer/src/db.py` die Verbindungsdaten ggf. angepasst werden.
```
    cnx = mysql.connector.connect(
        host="localhost",
        port=3306,
        user="root",
        password="mypass")
```
Anschließend kann der Server in `quellcode/pfyl-analyzer` mit folgendem Befehl gestartet werden:
`python3 src/server.py`

## pfyl-viewer
Die Visualisierung erfolgt über eine Web-Anwendung zu finden im Verzeichnis `quellcode/pfyl-viewer`.
### Vorraussetzungen
Folgende Anwendungen werden benötigt:
* Node Package Manager (npm) 

Die Abhängigkeiten können folgendermaßenb installiert werden:
```
cd quellcode/pfyl-viewer
npm install
```
Anschließend kann die Applikation mit `npm run start` gestartet werden. Die Anwendung ist dann über die folgende Adresse erreichbar: `http://localhost:3000/`

## Start-Reihenfolge
Da die einzelnen Komponenten der Lösung abhängig voneinander sind sollte folgende Start-Reihenfolge eingehalten werden:
1. InfluxDB starten
2. mysql Datenbank starten
3. UART-Host oder ITM starten
4. pfyl-analyzer & pfyl-viewer starten
5. Flashen und Starten des Embedded Device

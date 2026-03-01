# M&M sorteermachine

## Materiaal

![Materiaal](doc/materiaal.jpg)


### Te 3D-printen onderdelen

- [center_pin.stl](3D-files/center_pin.stl)
- [central_compartment.stl](3D-files/central_compartment.stl)
- [container.stl](3D-files/container.stl)
- [final_divider.stl](3D-files/final_divider.stl)
- [funnel_cap.stl](3D-files/funnel_cap.stl)
- [lid.stl](3D-files/lid.stl)
- [limit_switch_holder.stl](3D-files/limit_switch_holder.stl)
- [lower_disk.stl](3D-files/lower_disk.stl)
- [recipient.stl](3D-files/recipient.stl) (6 keer)
- [secondary_funnel.stl](3D-files/secondary_funnel.stl)
- [sensor_housing.stl](3D-files/sensor_housing.stl)
- [sorter-base.stl](3D-files/sorter-base.stl)
- [sorter-topring.stl](3D-files/sorter-topring.stl)
- [top_layer.stl](3D-files/top_layer.stl)
- [upper_disk.stl](3D-files/upper_disk.stl)
- [washer-08mm.stl](3D-files/washer-08mm.stl) (3 keer)
- [washer-10mm.stl](3D-files/washer-10mm.stl) (3 keer)
- [washer-14mm.stl](3D-files/washer-14mm.stl) (6 keer)

### Electronica componenten

- Arduino uno of Nano
- Breadboard of Nano expansion board
- RGB led 5mm
- Witte led 5mm
- Weerstand 220 ohm (4 keer)
- Weerstand 10k
- Microswitch [SS-5GL](https://www.amazon.com.be/s?k=micro+switch+SS-5GL)
- Servo motor [Microservo 9G + accessoires](https://www.amazon.com.be/Micro-9g-Servo/s?k=Micro+9g+Servo)
- Stepper motor [5V 28BYJ-48 met ULN2003 drivershield](https://www.amazon.com.be/s?k=stepper+28BYJ-48+ULN2003)
- Dupont breadboardkabels
- Montagekabels

### Montage

- Bout M4 X 50 (3 stuks)
- Bout M4 X 40 (3 stuks)
- Bout M3 x 5 of M3 x 8 (4 stuks)
- Bout M2.5 x 20 (3 stuks)
- Moer M2.5 (2 stuks)
- Rondel M2.5 (1 stuk)
- Vijsjes die bij de servokit zitten

### Code

Laad deze repo in Visual Studio Code.
Alle code staat in [main.cpp](src/main.cpp)

## Electronicaschema

![draadschema](doc/draadschema.png)

Ofwel gebruik je arduino (uno of nano) en een breadboard, ofwel een arduino nano met [expansion board](https://www.amazon.com.be/s?k=arduino+nano+expansion+board) (zoals bij de opnames).

## Montage bovenste gedeelte

Het bovenste gedeelte is gebaseerd op de [M&M sorter](https://www.instructables.com/Arduino-MM-Color-Sorter/) van Jackofalltrades_.

Als het onduidelijk is hoe de onderdelen tegen elkaar moeten geplaatst worden, dit is het schema van de onderdelen van het bovenste gedeelte:

![bovenste delen](doc/schema-bovenste-delen.png)

### Servo + behuizing onderste sorteerder

Bij de servo zitten vijsjes en ook een wit kruis dat je op de servo kan vijzen. Je moet dit een beetje bijknippen zoals op de foto's hieronder. Op die manier past het in de 3D-print.

![Servo stap 1](doc/servo-montage-01.jpg)
![Servo stap 2](doc/servo-montage-02.jpg)
![Servo stap 3](doc/servo-montage-03.jpg)
![Servo stap 4](doc/servo-montage-04.jpg)

### Witte led behuizing

Gebruik een 220 Ohm weerstand voor de witte led.
Soldeer draadjes aan de led om de led en de weerstand te kunnen verbinden.
Lijm de led daarna vast in de 3D-print.

![Witte led behuizing](doc/witte-led-behuizing.jpg)

### Bovenste sorteerder

Gebruik een M2.5 x 20 bout en rondel

![bovenste sorteerder 01](doc/bovenste-sorteerder-01.jpg)
![bovenste sorteerder 02](doc/bovenste-sorteerder-02.jpg)
![bovenste sorteerder 03](doc/bovenste-sorteerder-03.jpg)

### Bovenste en onderste sorteerder samen

Gebruik drie M4 X 50 bouten, drie 8mm washers en drie 14mm washers.

![bovenste sorteerder 04](doc/bovenste-sorteerder-04.jpg)
![bovenste sorteerder 05](doc/bovenste-sorteerder-05.jpg)
![Montage klaar van bovenste en onderste sorteerder](doc/bovenste-sorteerder-06.jpg)

### Controlekamer

de controlekamer bestaat uit een RGBled (zorg dat de pin van iedere kleur een weerstand van 220 Ohm heeft) en de licht sensor (LDR sensor met zijn schakeling, zie [het draadschema](doc/draadschema.png) hiervoor).

![Controlekamer](doc/controlekamer.jpg)

Duw de gemonteerde controle zachtjes op zijn plaats. Hij moet stevig vastzitten en geen licht van buitenaf doorlaten.

Mogelijks helpt het om wat extra plakband aan te brengen zodat hij goed vast zit.

![montage controlekamer](doc/controlekamer-montage.jpg)
![controlekamer gemonteerd](doc/controlekamer-montage-02.jpg)
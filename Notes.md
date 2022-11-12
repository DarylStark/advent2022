# Notes

-   Gesoldeerd
    -   Gele knop: 19
    -   Rode knop: 18

-   Breadboard
    -   4
    -   13

-   Kabels van de LED strip:
    -   Groen: +
    -   Wit: -
    -   Bruin: data (GPIO 5)

## Status

_2022-11-03_: Kaartje doet het niet meer. Wellicht ergens een zekering ofzo doorgebrand want hij gaat niet meer aan. Het schermpje krijgt wel nog stroom. Meerdere kabels geprobeerd, maar doet het niet. Moet opnieuw worden gesoldeerd.

| Device        | Pin | Waarheen | Kleur     |
| ------------- | --- | -------- | --------- |
| Gele knop     | +   | GPIO 19  |           |
| Gele knop     | -   | GND      |           |
| Rode knop     | +   | GPIO 13  |           |
| Rode knop     | -   | GND      |           |
| Volgende knop | +   | GPIO 4   |           |
| Volgende knop | -   | GND      |           |
| Vorige knop   | +   | GPIO 18  |           |
| Vorige knop   | -   | GND      |           |
| LCD           | GND | GND      | Wit/groen |
| LCD           | VCC | 5v       | Groen     |
| LCD           | SDA | GPIO 21  | Wit/blauw |
| LCD           | SCL | GPIO 22  | Blauw     |
# Popis semestralnej prace
Semestralna praca k predmetu principy operacnych systemov na temu download manager, rok 2022/23. Vypracovali Maros Majba a Barbora Simunekova

## Zadanie semestralnej prace
Klientska aplikácia, ktorá dovolí používateľovi pripojiť sa na vzdialený server a pomocou štandardných protokolov stiahnuť obsah dostupný na serveri. Manažér musí podporovať nasledujúce protokoly: http, https, ftp a ftps, pričom umožní:
- naplánovať čas, kedy sa má sťahovanie začat
- ukončiť sťahovanie 
- spravovať lokálne adresáre 
- spravovať históriu sťahovania
- definovať prioritu sťahovania
- v prípade súčasného sťahovania viacerých súborov ich naozaj sťahovať súčasne a nie postupne.

Pri protokoloch, ktoré to dovoľujú, aplikáciatiež umožní pozastaviť alebo obnoviť sťahovanie súboru.

Poznámka: aplikácia  nesmie  byť  implementovaná  spoužitím  nástroja **Wget**  a iných  podobných  nástrojov slúžiacich na sťahovanie internetového obsahu.

## Poznatky ku zadaniu
- Jedna sa o program, kde budu medzi sebou komunikovat server a klient.
- Server a klient budu predstavovat 2 sockety.
- Klient bude vyuzivat OpenSSL na pouzitie ftps a htttps protokolov.
- Jedno stahovanie bude jedno vlakno na servery, aby sa povolilo stahovanie viacero suborov naraz, nie za sebou.
- Dalsie atributy spomenute v zadani budu riesene cez buffer, ktory bude server zdielat so svojimi vlaknami.

# Poziadavky na spustenie
Aplikacia bude obsahovat graficke rozhranie vytvorene v prostredi gtkmm. Pre spravne fungovanie aplikacie je potrebna instalacia nasledovnych kniznic:
- GTK4: https://www.gtk.org/docs/installations
- GTKMM: https://gtkmm.org/en/download.html

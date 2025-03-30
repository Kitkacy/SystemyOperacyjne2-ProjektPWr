# SystemyOperacyjne2-ProjektPWr

## Opis projektu

Projekt implementuje problem **jedzących filozofów**. Jego celem jest przedstawienie zagadnień takich jak wielowątkowość w procesie programu, współdzielenie zasobów przez wątki, deadlocki między wątkami oraz zademonstorwanie radzenia sobie z nimi.

### Opis problemu

Problem jedzących filozofów zakłada, że:
- Istnieje `N` filozofów siedzących przy okrągłym stole. (Gdzie `N` jest argumentem podawanym programowi)
- Filozof może znajdować się w 3 stanach:
    1. **Myślenie** – filozof nie korzysta z widelców.
    2. **Głód** – filozof próbuje podnieść dwa widelce.
    3. **Jedzenie** – filozof korzysta z obu widelców.
- Każdy filozof potrzebuje dwóch widelców (lewego i prawego), aby jeść.
- Widelce są współdzielone między sąsiadującymi filozofami.
- Podczas jedzenia, filozof podnosi oba widelce, i dokłada je dopiero po skończeniu posiłku.
- Filozof nie może podnieśc widelca znajdującego się już w ręce jego sąsiada. 

Rozwiązanie problemu polega na:
- Nie wystąpieniu zakleszczenie (deadlock), gdy wszyscy filozofowie próbują jednocześnie podnieść widelce.
- Nie wystąpieniu zagłodzenie (starvation), czyli sytuacji, w której filozof nigdy nie dostanie obu widelców.

### Jak rozwiązano problem?

- **Sekcje krytyczne**:
  - Podnoszenie widelców (mutexy na każdy widelce).
  - Wyświetlanie stanu filozofów na ekranie (użycie mutexu o nazwie `mutexForTerm`).
  Mutex o nazwie `mutexForTerm` zapewnia, że tylko jeden wątek na raz może modyfikować wyjście na ekran, a mutex w wektorze `forks` umożliwia zajmowanie filozofowi widelca na wyłączność podczas żywienia. 

- **Rozwiązanie problemu zakleszczeń**:
  - Filozofowie o parzystych indeksach podnoszą najpierw lewy widelec, a następnie prawy.
  - Filozofowie o nieparzystych indeksach podnoszą najpierw prawy widelec, a następnie lewy.
  W ten sposób unikamy sytuacji w której każdy czekałby na swojego sąsiada w pętli.

## Instrukcja uruchomienia

1. Sklonuj repozytorium:
   ```
   git clone https://github.com/Kitkacy/SystemyOperacyjne2-ProjektPWr.git
   cd SystemyOperacyjne2-ProjektPWr
   ```

2. Skompiluj projekt za pomocą `Makefile`:
   ```
   make all
   ```

3. Uruchom program, z domyślną liczbą filozofów (pięciorgiem):
   ```
   make run
   ```
   Lub, uruchom plik wykonywalny podając za argument w miejsce "N" swoją własną ilość filozofów:
   ```
   ./main N
   ```

4. Aby wyczyścić pliki wykonywalne:
   ```
   make clean
   ```

## Przykładowy wygląd działania programu

Po uruchomieniu programu, terminal wyświetla stany filozofów w czasie rzeczywistym:
```
Press Ctrl+C to exit.
=============================================
=============Dining Philosophers=============
=============================================

💭 Philosopher 0 is THINKING.
🥪 Philosopher 1 is HUNGRY.
🍴 Philosopher 2 is EATING.
...
```
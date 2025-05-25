# SystemyOperacyjne2-ProjektPWr

## Opis projektu

Projekt implementuje **aplikację czatu opartą na socketach**. Jego celem jest przedstawienie zagadnień takich jak wielowątkowość w procesie programu, synchronizacja wątków za pomocą mutexów oraz komunikacja sieciowa przy użyciu socketów.

## Opis problemu

**Aplikacja czatu oparta na socketach zakłada, że:**
- Istnieje serwer, który obsługuje wielu klientów.
- Każdy klient ma swój unikalny identyfikator (nazwę).
- Serwer odbiera wiadomości od klientów i rozsyła je do wszystkich pozostałych klientów.
- Klienci mogą dołączać i opuszczać czat w dowolnym momencie.
- Komunikacja musi być synchronizowana, aby uniknąć problemów z współbieżnym dostępem do zasobów.

**Funkcje aplikacji:**
- Obsługa wielu klientów jednocześnie
- Rozsyłanie wiadomości w czasie rzeczywistym
- Bezpieczna obsługa wiadomości z wykorzystaniem mutexów
- Wsparcie dla nazw użytkowników
- Powiadomienia o dołączeniu i opuszczeniu czatu przez użytkowników
- Dwa rodzaje klientów: tekstowy (konsolowy) oraz graficzny (GUI)

### Jak rozwiązano problem?

**Sekcje krytyczne**:
  - Dostęp do listy wiadomości (mutex `message_mutex`).
  - Dostęp do listy klientów (mutex `clients_mutex`).
  - Powiadamianie o nowych wiadomościach (zmienna warunkowa `message_cv`).
  
**Rozwiązanie problemu współbieżności**:
  - Serwer tworzy oddzielny wątek dla każdego podłączonego klienta.
  - Każdy klient posiada wątek do odbierania wiadomości.
  - Wykorzystanie mutexów do synchronizacji dostępu do współdzielonych zasobów.
  - Wykorzystanie zmiennych warunkowych do sygnalizowania nowych wiadomości.

## Instrukcja uruchomienia

(Zakłada się że program będzie uruchamiany na systemie operacyjnym **Linux**)

1. Sklonuj repozytorium:
   ```bash
   git clone https://github.com/Kitkacy/SystemyOperacyjne2-ProjektPWr.git
   cd SystemyOperacyjne2-ProjektPWr/Etap2
   ```

2. Skompiluj projekt za pomocą `Makefile`:
   ```bash
   make all
   ```

3. Uruchom serwer w jednym terminalu:
   ```bash
   ./server
   ```

4. Uruchom jednego lub więcej klientów tekstowych w osobnych terminalach:
   ```bash
   ./client
   ```

5. Alternatywnie, uruchom klienta z graficznym interfejsem:
   ```bash
   ./gui_client/gui_client
   ```

6. Po uruchomieniu klienta wpisz swoją nazwę użytkownika i zacznij czatować.

7. Aby wyjść z aplikacji klienta tekstowego, wpisz `quit` i naciśnij Enter.
   W przypadku klienta graficznego, zamknij okno aplikacji.

8. Aby wyczyścić pliki wykonywalne:
   ```
   make clean
   ```

## Przykładowy wygląd działania programu

### Terminal serwera:
```
Server started on port 8080
Waiting for connections...
Wicek has joined the chat!
Dawid has joined the chat!
Wicek: Cześć wszystkim!
Dawid: Hej, co słychać?
Wicek has left the chat.
```

### Terminal klienta:
```
Connected to chat server at 127.0.0.1:8080
Enter your name: Dawid
Start chatting (type 'quit' to exit):
> Wicek has joined the chat!
> Wicek: Cześć wszystkim!
> Hej, co słychać?
> Wicek has left the chat.
```

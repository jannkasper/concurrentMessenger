1. INSTRUKCJA KOMPILACJI
	- uruchomienie Makefile w terminalu

2. INSTRUKCJA URUCHOMIENIA
	- do sprawdzenia działania programu potrzebne 3 terminale [./s ./k ./k ]
	- w celu uruchomienia serwera wywołanie komendy: ./s
	- w celu uruchomienia klienta wywołanie komendy: ./k

3. ZAWARTOŚĆ PLIKU Jan.Kasper.114885_s.c
	- serwer nie podlega obsłudze przez użytkownika
	- serwer pracuje na otrzymanych informacjach od klientów
	- serwer wyświetla wynik wykonanych operacji
	- pracę serwera możemy zakończyć poprawnie wywołując ENETER


   ZAWARTOŚĆ PLIKU Jan.Kasper.114885_k.c
     *logowanie
	- logowanie za pomocą ID [int] oraz LOGIN [char[10] - tylko litery alfabetu]
	- blokada logowania po 3 próbach błędów w ID oraz LOGIN 
	- uruchomienie asynchronicznego odbierania wiadomości za pomocą '1' [inaczej '0']
	- wywołanie dowolnej funkcji użytkownika poprzez wybranie [int] z zakresu 1-7
     *funkcja [1][6]
	- wprowadzanie subskrypcji w formie pojedynczego znaku [char]
     *funkcja [2] 
	- w trakcie wysyłania wiadomości określa się subskrypcje [char]
	- w trakcie wysyłania wiadomości nadaje się priorytet w skali 0-10 [int]
	- wysyłana wiadomość ma ograniczoną pojemność [char[100]]
     *funkcja [3]
	- ponowne wyświetlenie MENU     
     *funkcja [4]
	- blokowanie użytkownika poprzez wprowadzenie dokładnego jego LOGINU
     *funkcja [5]
	- wyświetlenie synchroniczne wiadomości zgodnie z priorytetem nadania
     *funkcja [7]
	- uruchomione procesy oraz potok użytkownika usuwany przy zamykaniu systemu

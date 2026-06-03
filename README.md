Opis działania projkektu
Dwie płytki wemos D1 R1 są połączone z serwerem
//W ramach projektu serwer jest symulowany przez hotspot
//Adresy serwera i bramy domyślenj uzyskane z hotspota
//W celu testowania należy sprawdzić adresacje uzyskaną przez źródło internetu i podmianę adresacji na dane zgodne z tą adresacją
Użytkownik loguję się na serwer poprzez podanie adresu IP (172.16.54.100)
Użytkownik zostaje połączony na serwer przez płytkę MAIN
W trakcie działanie obu płytek płytka BACKUP wysyła cykliczne zapytania /heartbeat w celu uzyskaania informacji, czy płytka MAIN działa.
W sytuacji w której płytka MAIN nie zwróci odpowiedzi płytka BACKUP przejmuję rolę MAIN, użytkownik zostaję z nią automatycznie podłączony bez potrzeby ponownego wpisywania adresu
W momencie powrotu płytki MAIN do życia serwer BACKUP zstępuje z roli serwera głównego, którą ponownie przejmuję płytka MAIN a użytkownik zostaję na nią automatycznie przełączony

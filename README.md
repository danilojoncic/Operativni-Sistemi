# Operativni-Sistemi
## Ispod su dati kratki opisi zahtjeva svakog od domacih radova koji su vezani za xv6 operativni sistem na predmetu OS na RAF-u 2022/2023 godine, za precizne i tacne zahtjeve i opise domacih radova, pogledajte pdf fajlove ispod.

### D1
-zadatak jeste da se napravi vizuelni meni,koriscenjem kojeg se ispis na konzoli blokira, meni treba da nudi opcije mijenjanja boja teksta i pozadine na iducim ispisima,interakcija sa menijem se izvrsava preko taster 'w' i 's' za prolazak kroz meni, i kombinaciju taster 'ALT' + 'c' za paljenje i gasenje menija, potrebno je izvrsiti promijenu na kbd fajlovima koji rade sa tastaturom radi tastera 'ALT"

### D2
1. Povecati filesystem i omoguciti duplo indirektno povezivanje kod blokova u strukturama inode i dinode,
2. Pravljenje sistemskih poziva i korisnickih programa (encr,decr,setkey,setecho) koji omogucuju enkripciju i dekripciju fajlova na osnovu njihovih major vrijednosti strukture inode
3. Pravljenje korisnickog programa blockwriter koji omogucuje pravljenje fajlova proizvoljne velicine u blockovima i proizvoljnog imena


### D3
1. Dodati shared strukture kao dio procesa
2. Napraviti user programe DALLE(parent) ,LISA (child) i COMMA (child) 
3. Omoguciti komunikaciju izmedju user programa pomocu sistemskih poziva koji im pruzaju pristup dijeljenoj memoriji roditelj procesa
4. Modifikovati fork() i exec() pozive i napraviti nove sistemske pozive sa svrhom dijeljenja memorije
5. Odraditi precizno stranicenje shared struktura
6. Segmentisati virtualnu memoriju procesa tako da shared strukture se upisuju u poseban opseg

### Veliko upozorenje!
Naime, moj kod za 3. domaci je zapravo zaobisao citav zahtjev za izmjenu stranicenja i  dio sa parent-child process memorijom.Kod koji stoji zakomentarisan u nuznim fajlovima za izvrsavanje ovih zahtjeva zapravo vrlo cesto pravi page-fault, crashuje ili restartuje xv6 terminal, koristeci mnogobrojne printove unutar koda se vidi da je kod na 90% onoga sto treba da bude ali pravi gresku, na osnovu tog nedostatka i zurbe sa rokovima, zadatak je uradjen na drugaciji nacin. Naime da bi se omogucila komunikacija izmedju procesa umjesto dijeljenja memorije (kod je zakomentarisan za to) napravljeni su novi sistemski pozivi koji rade nalik getter i setter metodama u Javi, samo sto ono sto ti pozivi rade jeste zapravo mijenjanje jednog int flag-a u kernel fajlu koji omogucava da COMMA salje komande LISA user programu.

### Kako pokrenuti?
1. Koristiti Linux Ubuntu 22.04 (VirtualBox emulator za Windows ili WSL2)
2. Preuzeti QEMU 
3. Uraditi git pull repozetorijuma 
4. U preuzetom folderu uci u odgovarajuci podfolder domaceg (`cd domaci1`)
5. Unutar datog foldera uraditi komandu `make qemu`








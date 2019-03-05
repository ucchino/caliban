Caliban.                                                                  
                                                                          
  Un fault simulator che adotta un algoritmo denominato "GODFATHER" ideato da Pier Paolo Ucchino.

Si basa pesantemente sull'accellerazione hardware data dai barrel shifter (è un circuito digitale progettato per eseguire operazioni di bit shift su un data word (costituito da un numero specifico di bit) in un tempo corrispondente ad un ciclo di clock). Il parallelismo e' dato dalla word e permette di valutare in un colpo solo 32 circuiti identici ma con valori dei FF diversi. Non ho provato su circuiti a 64 bit.

Il programma e' stato scritto a quattro mani durante la primavera del 1997 e il Natale dello stesso anno. L'ideazione e la messa a punto nei dettagli dell'algoritmo ha richiesto un mese circa di approfondimenti vari. A tal proposito vorrei menzionare colui grazie al quale Caliban e' nato... Massimiliano Schillaci detto Max o M0... Solo grazie alla sua enorme pazienza sono riuscito a mettere insieme un qualcosa che ha prestazioni di tutto rispetto. E' solo grazie alle sue spiegazioni che sono riuscito a capire come funziona concettualmente un FaultSimulator.

- Sfruttamento delle risorse hardware specifiche (core)

- Ordinamento dinamico di guasti che eccitano molti FF

- Non necessita di accorgimenti di alcun tipo in caso di 2 FF connessi direttamente fra loro (contrariamente a quanto fa HOPE oppure il Molokh versione diagnostica)

- Contempla il caso di ingressi sincronizzati (PI connessi direttamente ad un FF)

Il codice visto con gli occhi di oggi e' un po' rozzo e puo' essere migliorato.

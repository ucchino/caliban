Caliban.                                                                  
                                                                          
  Un fault simulator che adotta un algoritmo denominato "GODFATHER"       
ideato dalla mente malata di Pier Paolo Ucchino (s73259@cclix1.polito.it).
Il programma e' stato scritto a quattro mani e piedi durante la primavera 
del 1997 e il Natale dello stesso anno. L'ideazione e la messa a punto nei
dettagli dell'algoritmo ha richiesto un mese circa di scervellamenti vari.
A tal proposito vorrei menzionare colui grazie al quale Caliban e' nato...
Massimiliano Schillaci detto Max o M0... Solo grazie alla sua enorme, anzi
(conoscendomi) direi titanica, pazienza sono riuscito a mettere insieme un
qualcosa che funzionicchi abbastanza... E' solo grazie alle sue spiegazioni*
che sono riuscito a capire come funziona concettualmente un FaultSimulator

- Sfruttamento delle risorse hardware specifiche (core)

- Ordinamento dinamico di guasti che eccitano molti FF

- Non necessita di accorgimenti di alcun tipo in caso
  di 2 FF connessi direttamente fra loro (contrariamente
  a quanto fa HOPE oppure il Molokh versione diagnostica)

- Contempla il caso di ingressi sincronizzati (PI connessi
  direttamente ad un FF)

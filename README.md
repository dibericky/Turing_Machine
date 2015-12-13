# Macchine di Turing per Arduino

Con Arduino è possibile creare una specie di MdTU, capace infatti di simulare una Macchina di Turing.
Arduino è una piattaforma fisica open-source, basata su un microcontrollore e programmabile in linguaggio C-like.  Arduino presenta vari Pin, a cui si collegano i vari dispositivi che si vogliono integrare nel circuito.
 
In questo progetto ho utilizzato Arduino Mega, simile ad Arduino UNO ma con un maggior numero di Pin.

#	Come costruirla
Ovviamente non è possibile utilizzare un nastro infinito, quindi ho sostituito il nastro infinito con un insieme finito di led (15)
0 = led spento
1 = led acceso
La testina è stata sostituita da una fotoresistenza, capace di rilevare la luminosità, e da un motore passo-passo, che muoverà la testina con la fotoresistenza lungo il “nastro” di led.
Il nastro è rappresentato, a livello logico, da un array di lunghezza pari al numero di led.
led[15] = {led1,led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12, led13, led14, led15};
led1, led2, led3…ledN saranno i pin su arduino a cui sono collegati i led corrispondenti.
Quindi ogni variabile ledN avrà come valore il pin corrispondente.
int led1 = 1;
Così da poter accendere/spegnere il led richiamando il led su cui è la testina con la sintassi:
led[pos]
dove pos è una variabile intera che corrisponde alla posizione della testina.
Esempio:
pos = 5;      significa che la testina è sul led numero 5.

La funzione di transizione f è rappresentata da una matrice Rx5, dove R rappresenta il numero di quintuple
array_istruzioni: { 
  [stato_attuale, leggi, scrivi, stato_succ, move],
  [stato_attuale, leggi, scrivi, stato_succ, move],
  [stato_attuale, leggi, scrivi, stato_succ, move]
  }
Per accendere un led si usa la funzione di arduino digitalWrite(PIN, VALORE);
Dove PIN corrisponde al pin a cui è collegato l’oggetto a cui si vuole passare un certo valore di tensione ( HIGH, cioè 1, o LOW, cioè 0).
Quindi per far accendere il led1 darò il seguente comando:
digitalWrite(led[0], HIGH);      
led[0] corrisponde alla prima cella dell’array led (in informatica si conta da 0, quindi led[0] è led1, led[1] è led2 ecc…)
quindi quell’istruzione corrisponde a:
 digitalWrite(led1, HIGH);
led1 sarà una variabile contenente come valore il pin a cui è collegato il primo led.
Uno script per Arduino è composto da due parti, la prima che corrisponde alla funzione setup(), in cui imposto tutti i led come dispositivi di OUTPUT, e la seconda parte che corrisponde alla funzione loop() in cui è eseguito in modo ciclico tutta la logica del programma.
La funzione di loop() è () rieseguita continuamente finché Arduino è in esecuzione.
La testina è collegata ad un nastro dentato, che verrà fatto muovere da un motore passo-passo.
La testina è composta da una fotoresistenza, capace di rilevare la luce, e un sostegno stampato con la stampante 3d che si aggancia al nastro.
 
La fotoresistenza è un componente elettronico la cui resistenza è inversamente proporzionale alla quantità di luce che lo colpisce. Si comporta come un tradizionale resistore, ma il suo valore in ohm diminuisce mano a mano che aumenta l’intensità della luce che la colpisce. Ciò comporta che la corrente elettrica che transita attraverso tale componente è proporzionale all'intensità di una sorgente luminosa.
Il motore passo-passo è un motore composto da 4 elettromagneti disposti intorno ad un rotore metallico.
Se in posizione di equilibrio i 4 elettromagneti terranno immobile il rotore, ma magnetizzando in un determinato ordine i quattro elettromagneti è possibile far muovere il rotore. La particolarità, e il punto di forza, del motore passo-passo è la possibilità di controllare il movimento del motore, infatti è possibile dirgli di quanti passi muoversi.
 Eccitando il magnete in alto, verrà attirato il dente del rotore più vicino all’elettromagnete, allineandolo.
 Quindi viene eccitato il magnete 2, il quale attirerà il dente più vicino a sé, facendo così ruotare di 3.6° il rotore.
 Viene ora eccitato il magnete 3, che porterà ad un’altra rotazione di 3.6°
 Viene quindi eccitato il quarto elettromagnete, portando ad un’altra rotazione di 3.6°.
Sono necessari 100 passi per effettuare una rotazione completa.
Nel progetto ho usato la logica negata, in cui collego il polo negativo del led ad arduino, e il polo positivo del led ai 5v di arduino, che forniscono carica positiva.
Quindi, siccome per accendere un led gli si deve fornire carica positiva e negativa, dovrò dare carica negativa al pin a cui è collegato il led.
Infatti:
Led + / 5v	Led -  /Pin	Risultato
+	         +	        Non si accende
+	        -	          Si accende
Quindi il pin dovrà fornire carica negativa.
In caso utilizzassimo una batteria esterna, usando la logica negata possiamo alimentare i led direttamente con la batteria, evitando così di alimentarli direttamente con arduino, al quale saranno così collegati solo i poli negativi


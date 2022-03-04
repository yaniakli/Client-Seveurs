- taper en premier lieu "make all" pour tout compiler
- ensuite taper "./serveur <port>" pour run le serveur
- "./client1 <port>" et "./client2 <port>" pour lanceer les clients 
- les clients 1 et 2 sont les meme car les 2 peuvent envoyer un INT ou demander un MAX
- une fois le client lancé, entrer le pseudo puis appuyer sur "enter". Ensuite taper soit "INT <val>" ou "MAX" suivant ce que vous voulez

si le makefile ne marche pas compiler avec les lignes suivante ;
	
    gcc -Wall -o client2 client2.c
	gcc -Wall -o client1 client1.c
	gcc -Wall -pthread -o serveur serveur.c
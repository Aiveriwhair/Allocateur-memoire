# TP4_API
Question : Y a-t-il besoin de gérer une liste de zones occupées ?
Une liste de zones libres suffit puisqu'on on travail toujours dans la zone mémoire gérée par l'allocateur. On pourra dès lors connaître les zones occupées. Il n'est donc pas nécessaire d'utiliser une liste des zones occupées.

Question : Quelle est la structure d'une zone allouée ?


Question : Les adresses 0,1,2 sont-elles valides ? De manière générale, un utilisateur peut-il manipuler toutes les adresses ?

Question : Quand on alloue une zone, quelle est l'adresse retournée à l'utilisateur ?
on lui retourne l'adresse du premier bloc de memoire suffisante + la taille nécéssaire pour stocker la struct.


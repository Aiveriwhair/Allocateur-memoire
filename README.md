# Allocateur mémoire

Question : Y a-t-il besoin de gérer une liste de zones occupées ?
Une liste de zones libres suffit puisqu'on on travail toujours dans la zone mémoire gérée par l'allocateur. On pourra dès lors connaître les zones occupées. Il n'est donc pas nécessaire d'utiliser une liste des zones occupées.

Question : Quelle est la structure d'une zone allouée ?


Question : Les adresses 0,1,2 sont-elles valides ? De manière générale, un utilisateur peut-il manipuler toutes les adresses ?


Question : Quand on alloue une zone, quelle est l'adresse retournée à l'utilisateur ?
On lui retourne l'adresse du premier bloc de memoire suffisante + la taille nécéssaire pour stocker la struct.
Question : Quand on alloue dans une zone mémoire libre, il faut faire attention à la procédure de partitionnement. Dans le cas simple, on alloue le début de la zone pour nos besoins et la suite devient une zone de mémoire libre de taille (taille de la zone du début - taille allouée). Toutefois, il est possible que la taille qui reste soit trop petite. Pourquoi ? Comment gérer ce cas?

Il est possible que la taille restante soit trop petite si on ne prend pas en compte la taille de la zone nécéssaire à nos besoins. Pour pallier ce problème on n'oublie pas d'ajouter la taille de notre struct avant d'effectuer la recherche de la zone libre

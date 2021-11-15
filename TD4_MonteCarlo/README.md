######################################
TD4 - Échantillonnage - Hugo Trarieux
######################################


1. Intégration de Monte-Carlo 1D

Premiere partie de ce TD, dans laquelle nous devions calculer
la variance numérique, la variance analytique ainsi que
l'erreur d'estimation. Pour ce faire, il a fallu calculer a la main 
l'intégrale (5x⁴-1)² sur une intervalle de 0 à 1. On peut retrouver
ce calcul dans l'image "calcul.png" dans le dossier screenshots. 
Aucune difficulté rencontrée dans cette partie.


2. Anti-Aliasing

Implémentation de l'échantillonnage régulier en premier lieu.
Dans la méthode Viewer::renderBlock, modification pour que la méthode
lance plusieurs rayons par pixel a travers une grille d'une taille
qui dépend de l'attribut sampleCount de la caméra. Puis, dans un 
second temps, ajout d'un jitter sur chaque rayon pour obtenir un 
échantillonnage stratifié. Ce jitter est un nombre aléatoire entre
0 et 1 ajouté sur l'axe des x et des y.

3. Sources Étendues

Dans cette partie, il fallait tirer un rayon d'ombre dans une direction 
aléatoire obtenue a l'aide de uVec() et vVec(). Et ensuite moduler
l'intensité de la source par une texture de couleur. J'ai eu quelques 
problemes lors de ce calcul. Je prenais un nombre aléatoire entre 0 
et lightSize alors qu'il fallait etre entre -lightSize/2 et lightSize/2 (Voir bug 3).

4.1 Échantillonnage d'un hémisphère

Pas de bug particuliers dans cette partie, implémentation de multiples méthodes 
de la classe warp pour un échantillonnage uniforme de l'hémisphère autour du pôle(0,0,1)
ainsi qu'un échantillonage pondéré par le cosinus de l'hémisphère autour du pôle(0,0,1).




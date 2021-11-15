######################################
TP1 - Deferred shading - Hugo Trarieux
######################################


1. Création et remplissage du g-buffer

Pour cette première étape, aucun gros problème en particulier,
initialisation des textures dans la classe fbo, creation de
gbuffer.vert et gbuffer.frag pour la premiere passe, creation
de _fbo dans viewer.h puis initialisation dans viewer::init.
Activation de _fbo, puis activation du shader gbuffer,
rendu de la scene puis unbind de _fbo. Voir les résultats 
(resultat_1_1 et resultat_1_2) dans le dossier Screens.


2. Calcul de l'éclairage

Création d'un quad dans Viewer::init correspondant a la fenetre
d'affichage, creation de deferred.vert et deferred.frag pour la
deuxieme passe. Utilisation du modele Blinn-Phong. Activation
du nouveau shader, pour lequel il faut passer les 3 textures du
FBO avec glActiveTexture, glBindTexture, glUniform1i. C'est 
d'ailleurs avec cette derniere fonciton que j'ai eu quelques 
petits soucis, au moment d'envoyer des variables uniformes
au deferred.frag, j'ai mélangé glUniform1i et glUniform1f,
ou bien des bugs de variables que j'avais nommé différemment
dans le fragment shader. Voir resultat_2_1 dans le dossier
Screens. Je n'ai cependant pas réussi a implémenter plusieurs
sources de lumières. Avec quelques jours de plus cela aurait 
été possible.

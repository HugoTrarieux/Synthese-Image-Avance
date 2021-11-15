######################################
TD5 - BRDF de Ward - Hugo Trarieux
######################################


1. Eclairage direct
Pour cette premiere partie, j'ai tout d'abord implémenté une
version isotrope de la brdf de Ward. Ce qui me permettait de simplifier
l'equation de rendu (la 3ieme du document) car si l'on considère ax et
ay comme étant égaux, nous pouvons donc additionner les éléments présents
dans la tangante en exposant, car cos2 + sin2 = 1. En continuant avec quelques 
simplifications de cet exposant, la formule devenait un peu plus facile a mettre 
en place dans notre fonction de brdf, ce qui permettais d'arriver plus rapidement 
a un résultat. Une fois ce résultat obtenue j'ai donc implémenté la version 
anisotrope, sans les simplifications cette fois ci car ax peut etre different
de ay. Lors du calcul de la brdf, il fallait egalement utiliser la tangente
ainsi que la bitangente. Ces derniers vecteurs étaient obtenus en calculant
la projection d'un vecteur (0,1,0) sur le plan tangent défini par la normale.
Et la bitangente obtenue en effectuant le produit vectoriel entre le vecteur sur ce plan tangent, et la normale. Aucun vrai problemes dans cette partie,
mis a part un soucis de normalisation qui fesait apparraitre une sorte de
deuxieme tache lumineuse sur le sommet (voir bug1.png).
Voir dossier screenshots pour les resultats.


2. Echantillonnage uniforme de la BRDF.

Cette fois ci, il fallait implémenter un nouvel integrateur: DirectMats.
Dans lequel il fallait remplacer la reflexion miroir par l'echantillonnage
uniforme de la BRDF, sans se soucier de la refraction. Il fallait considerer
la carte d'environnement comme une source lumineuse et intégrer son effet
selon la BRDF de l'objet. A chaque intersection trouvée, il fallait generer N nouvelles directions que nous allions effchantillonner parmi l'hémisphere au dessus du point d'intersection. Ces nouvelles directions sont calculées avec la méthode Material::us. Et pour chacune de ces directions, il faut évaluer la brdf (celle implémentée lors de la premiere partie) qui va venir pondérer
l'intensité lumineuse lue dans la carte d'environnement. Les difficultees 
rencontrees dans cette partie étaient géneralement liées a de mauvaises
orientations de vecteurs(voir bug2_1.png et bug2_2.png);
Voir dossier screenshots pour les resultats.

3. Echantillonnage preferentiel

Pour cette partie, il fallait mettre en oeuvre la technique d'importance
sampling pour reduire la variance en concentrant l'echantillonnage dans les
directions contribuant le plus au resultat final. Cette fois ci c'etait les equations 6 a 8 du document a implementer. Et afin de verifier que nos directions
etaient bonnes, il etait possible de les tracer (voir dossier screenshots).
Aucun probleme dans cette partie.
Voir dossier screenshots pour les resultats.

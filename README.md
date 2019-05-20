# Arduino-Meteo

## Programme prototype pour la station météo autonome.

Pour enregistrer l'heure de chaque mesure, je passe par une requête NTP sur mon Raspberry. On peut aussi passer par un module RTC mais je n'en ai pas.
La sauvegarde se fait sur un fichier CSV qui change de nom au bout d'un nombre d'enregistrements donné et il est alors transmis (avant changement de nom) via FTP sur un serveur local (mon Raspberry).
Dans la fonction loop, la lecture sur le port série permet de vérifier que le programme fonctionne + ou - bien.

En conclusion, cette approche, bien qu'interessante, me semble assez complexe pour des élèves débutants.
je pense que l'on peut simplifier la programmation côté Arduino qui donnera à intervalles réguliers les données à un ordinateur via le port série : un petit programme en Python écoute le port série et enregistre sur un fichier CSV les données...



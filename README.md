```
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡴⠒⠈⠉⡕⢢⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠾⣿⡀⣸⡀⢀⣀⠀⣇⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⣴⡛⠰⠁⠳⣤⠧⠼⢥⡜⠈⡌⣈⣧⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⣸⠃⢈⠶⠗⠈⢀⡠⠀⣀⠁⠺⠶⣉⠈⣧⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⡏⢉⡜⠀⠀⠀⠮⠵⠮⠵⠀⠀⠀⢸⡉⣹⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⢻⡓⠦⣄⣷⣍⡇⠠⠤⣀⠀⠀⠀⠀⣀⡤⠄⢈⣏⡾⣀⡤⠖⡻⠀⠀⠀
  ⠀⠀⠀⠀⢱⡭⠶⡩⡗⣷⠾⢦⣬⣉⠂⠀⢋⣥⡴⠶⣾⡟⠯⠒⢯⡌⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⢻⢶⣼⣻⠈⠓⠄⠻⠉⠀⠀⠀⠿⠡⠘⠉⣇⣆⡼⣟⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⡰⢗⠾⡟⢑⣆⠀⠀⠀⠀⠀⠃⠀⠀⠀⢒⣰⡏⢹⠹⡒⠇⡀⠀⠀⠀
  ⠀⠀⢀⢞⣷⡯⡠⣻⢂⢸⣦⡀⠀⠠⠤⠠⠄⠀⢀⣴⣯⠨⢺⢤⡼⡴⣑⠄⠀⠀
  ⠠⣿⣃⢋⣿⡼⢤⣳⢢⢈⣿⣿⢦⣀⡈⠁⣀⡴⢻⣿⡁⠑⢾⣦⢇⢵⣝⡜⣶⠄
  ⠸⠙⠣⢹⡱⢧⢿⡷⠨⢀⡿⢿⣄⠙⢿⡿⠏⣠⣼⠿⡁⠣⢼⣿⣤⢆⣧⢻⠃⢃
  ⠘⠚⠁⢹⠳⡼⣎⡣⡄⢙⠀⠀⠉⢣⡀⢀⡴⠋⠁⠀⡎⢠⢴⢱⢧⡞⣿⠈⠓⠊
  ⠀⠀⠀⠈⢿⠳⡼⣷⡴⢹⡦⢤⣀⠀⠘⠏⠀⢀⣤⢴⡎⢢⣾⢷⠜⢿⠁⠀⠀⠀
  ⠀⠀⠀⠀⠈⢲⠚⢼⣵⣩⣟⣦⣙⢟⣦⣤⣺⣏⣤⣿⡭⣪⡿⠞⡖⠃⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠁⠒⢰⡟⠻⣶⣿⣿⣾⣟⣾⣿⣷⠟⠻⡆⠒⠊⠁⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠘⣃⡴⠁⢸⡻⢿⣿⢿⡇⠈⠢⣸⡃⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⠻⠟⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  
__________     .__       .___        ._.
\____    /____ |  |    __| _/____    | |
  /     // __ \|  |   / __ |\__  \   | |
 /     /\  ___/|  |__/ /_/ | / __ \_  \|
/_______ \___  >____/\____ |(____  /  __
        \/   \/           \/     \/   \/
```
# Construction d'un moteur de jeux vidéo

Nous allons devoir créer un moteur physique pour les jeux vidéo en équipe de manière ittérative en 4 phases :

- Phase 1 : construction d’un moteur élémentaire de gestion de particules
- Phase 2 : gestion d’objets formés de plusieurs masses par l’ajout de différentes forces et contraintes
- Phase 3 : gestion des corps rigides par l’ajout de la physique rotationnelle
- Phase 4 : ajout d’un système élémentaire de détection et résolution de collisions

## Prérequis

Liste des dépendances nécessaires avant de compiler et exécuter le programme :

**Langage / compilateur** : C++

**Framework** : [OpenFrameworks](https://openframeworks.cc/)

## Compilation

1. Génerer un projet avec projectGenerator 

2. Cloner le repo et copier son contenu à la racine du projet pour remplacer le `src/`

3. Compiler et lancer selon le type de projet

## Auteurs

Membres du groupe :
- Hippolyte COSSERAT
- Emeline GÉHANNO
- Florian HEME
- Dimitri JASICA

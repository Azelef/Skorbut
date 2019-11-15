# Projet d'OS Skorbut

## Organisation du projet
Le code C est dans le dossier src, boot.s et linker.ld sont à la racine.
La compilation s'effectue avec make (ou make comp), l'exécution de l'OS dans QEMU avec make run, la suppression des fichiers autres que le code avec make clean, et on peut combiner compilation et exécution avec make test.

## Architecture
Skorbut est concu pour le Raspberry Pi 2, qui a un processeur ARM. Il utilise le jeu d'instruction AArch32. Le code de base est tiré du tutoriel 'Raspberry Pi Bare Bones' de osdev.org.

## Fonctionnalités
Au démarrage, Skorbut affiche son nom et son logo (une orange) sur l'écran, et ouvre un terminal dans lequel on peut entrer du texte à l'aide du port série. Une douzaine de commandes sont disponibles :
* clear pour effacer l'écran
* echo pour afficher du texte
* logo pour afficher le logo de l'OS
* note pour lancer un mini bloc-note
* cat,cd,ls,mkdir,pwd,rm,rmdir,touch et write pour interagir avec un système de fichiers.

## Implémentation
Pour afficher du texte, Skorbut demande (à l'aide du système de mailbox) au processeur graphique du Raspberry Pi de créer un framebuffer (un espace dans la RAM dont le contenu sera affiché sur l'écran). Il dessine ensuite les caractères pixel par pixel dans ce framebuffer.
Pour afficher son logo, il le copie depuis un tableau contenant un bitmap de l'image à afficher (orange.h). Le bloc-note utilise le même code avec quelques changements (plus précisément, le terminal reprend le code du bloc-note).
Le système de fichiers est stocké dans la RAM. Un fichier (ou dossier) est symbolisé par une structure contenant des pointeurs vers les fichiers suivants, précédents et parent, ainsi que l'adresse et la taille du contenu dans le cas d'un fichier et l'adresse du premier élément dans le cas d'un dossier. Ainsi, on peut parcourir chaque dossier comme s'il s'agissait d'une liste chainée, et aller de dossier en dossier comme dans un arbre.
Des fonctions utiles pour manipuler les chaines de caractères, communiquer par le port série et allouer la RAM ont été écrites en reprenant les noms et comportements de fontions équivalentes dans la bibliothèque standard du C.

## Ce qu'il reste à faire
Il reste de nombreuses choses à ajouter. Celles qui ont été entamées sont :
* La gestion de la mémoire (notamment la pagination et l'ajout de la possibilité de libérer une partie de la mémoire allouée)
* La gestion des interruptions, afin de pouvoir utiliser un clavier directement branché au Raspberry Pi, et créer un timer permettant de reprendre la main après avoir lancé un autre processus

Combinées avec la gestion de la carte SD, pour avoir un système de fichiers permanent, ces deux fonctionnalités permettraient de lancer des processus extérieurs au noyau.

De plus, le système ne fonctionne pas encore sur un véritable Raspberry Pi, pour des raisons qui nous sont inconnues.

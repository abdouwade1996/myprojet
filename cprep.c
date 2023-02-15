#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <errno.h>
#include <sys/types.h>

#define CHEMIN_MAX 512
//Fonction principale

int main(int argc, char *argv[])
{
    //Vérification du nombre d'arguments
    if (argc < 4)
    {
        perror("usage : cprep taille rep f1 ... fn\n");
        exit(1);
    }

    //Récupération de la taille lecture / écriture
    int size = atoi(argv[1]);
    if (size <= 0)
    {
        perror("Erreur : la taille doit être un entier positif\n");
        exit(1);
    }
    
    //Récupération du répertoire de destination 
    if (strlen(argv[2]) >= CHEMIN_MAX) 
    {
        perror("Erreur : le chemin du répertoire est trop long\n");
        exit(1);
    }

    char *rep_dest = argv[2];

    // Création du répertoire s'il n'existe pas

    struct stat st = {0};
    if (stat(rep_dest, &st) == -1)
    {
        mkdir(rep_dest, 0777);
    }

    //copies des fichiers
    for (int i = 3; i < argc; i++)
    {
        //Création du chemin source
        char source[CHEMIN_MAX];
        snprintf(source, sizeof(source), "%s", argv[i]);

        //Création du chemin destination
        char destination[CHEMIN_MAX];
        snprintf(destination, sizeof(destination), "%s/%s", rep_dest, argv[i]);

        //Ouverture du fichier source
        int file_src = open(source, O_RDONLY);
        if (file_src == -1)
        {
            perror("Erreur : impossible d'ouvrir le fichier \n");
            exit(1);
        }

        //Ouverture du fichier destination
        int file_dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (file_dest == -1)
        {
            perror("Erreur : impossible d'ouvrir le fichier \n");
            exit(1);
        }

        //Copie du fichier
        char buffer[size];
        int nbre_octets;
        while ((nbre_octets = read(file_src, buffer, size)) > 0)
        {
            write(file_dest, buffer, nbre_octets);
        }

        if (nbre_octets == -1)
        {
            perror("Erreur : impossible de lire le fichier \n");
            exit(1);
        }

        //Copie des permissions et des dates de modification
        struct stat st;

        if (stat(source, &st) == -1)
        {
            perror("Erreur : impossible de récupérer les informations du fichier \n");
            exit(1);
        }

        if (chmod(destination, st.st_mode) == -1)
        {
            perror("Erreur : impossible de modifier les permissions du fichier \n");
            exit(1);
        }

        struct utimbuf ut;
        ut.actime = st.st_atime;
        ut.modtime = st.st_mtime;

        if (utime(destination, &ut) == -1)
        {
            perror("Erreur : impossible de modifier les dates de modification du fichier \n");
            exit(1);
        }

        //Fermeture des fichiers
        close(file_src);
        close(file_dest); 
    }

    exit(0);
}
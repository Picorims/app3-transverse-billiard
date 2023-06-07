# Comment utiliser Git

demander à Picorims/Charly en cas de problème.

## Sommaire

- syntaxe du document
- création du projet
- navigation / branches
- sauvegarder et récupérer le code
- méthodologie de travail du groupe
- à ne pas faire
- mot de la fin

## Syntaxe de ce document

Soit la commande:

`commande <parametre_obligatoire> [parametre_facultatif]`

Un mot entre <> est un champ obligatoire, tandis qu'un mot entre [] est un champ facultatif. On remplace le mot et les <> (ou []) par un mot clé. Les champs sans <> ou [] sont à recopier tel quel.

Par exemple, j'ai le droit d'écrire `commande foo`, puisque le dernier mot est facultatif, ou `commande bar baz`. Je n'ai pas le droit d'écrire `commande` car il manque `<parametre_obligatoire>`. Je n'ai pas le droit d'écrire `commandetruc` car ce n'est plus `commande`, je n'ai pas le droit de changer ce mot clé.

> **Note :** C'est une convention répandue en informatique, qui se rencontre notamment dans les documentations.



## Création

- ouvrir la page github du projet (https://github.com/Picorims/app3-transverse-billiard)
- cliquer sur le bouton "code"
- Dans l'onglet "HTTPS", copier l'url (URL du projet + extension `.git` => https://github.com/Picorims/app3-transverse-billiard.git)
- Ouvrir Git Bash (ou autre) dans le dossier où le **dossier** racine du projet doit être créé.
- exécuter `git clone <url>`
    - un dossier `app3-transverse-billiard` est crée dans le dossier courant, le code source et le dépôt git local se trouvent dedans.
    - pour s'y rendre, `cd app3-transverse-billiard`.



## Navigation

La branche courrante est affichée entre parenthèses en bleu dans Git Bash. Dans VSCode, l'extension Git Graph permet d'afficher un graphe du dépôt (où chaque noeud est un commit, avec son auteur, son nom, les fichiers modifiés, etc.), et la branche courrante est indiquée en gras.

> **Note :** Sur VSCode, on peut y accéder via, dans le menu de gauche l'icône "Source Control" (3 points reliés par des traits), puis un icône similaire en haut à gauche sous "SOURCE CONTROL". On peut aussi y accéder via Ctrl+Shipt+P puis en tapant "Git Graph: View Git Graph".

**ATTENTION :** Les commits sont **toujours** ajouté sur la branche courrante, aussi il ne faut jamais commit si l'on se trouve sur `main` ou `develop` au risque de rencontrer des problèmes de conflits sur votre machine!



### Structure de branches dans le projet

- `main`: branche contenant l'état final/stable du projet, c'est-à-dire les versions du code tels qu'elle serait par exemple donnée au client.
- `develop`: branche de travail commune sur lequel on se base pour créer nos branche. C'est le **travail commun**.
- **branches de travail**: crées depuis develop, elle contiennent une fonctionnalité en cours de développement, un bug en cours de correction, etc. **Une seule personne travaille sur chaque branche, sinon il y aura des conflits à chaque mise à jour de la branche!**



### Naviguer entre les branches

- se rendre sur une branche: `git checkout <nom_branche>` (possible que si l'espace de travail est vide, c'est-à-dire qu'aucun fichier n'a été modifié)
    - > **Note:** Il existe des commandes pour effacer les modifications "non sauvegardées" ou `git stash` pour les mettre de côté et les restaurer mais c'est un peu plus avancé. En tout cas c'est possible si absolument nécessaire. Au pire, si c'est seulement pour consulter du code, sachez que GitHub permet de naviguer via le navigateur au sein du code de n'importe quelle branche, on ne peut juste pas l'exécuter.
- créer une branche et s'y rendre, au choix:
    - `git branch -c <nom>` puis `git checkout <nom>`
    - `git switch -c <nom>`
- supprimer une branche **localement** (elle sera toujours sur GitHub **SI** elle a été mise en ligne (push) sur GitHub!):
    - `git branch -d <nom>`
- mettre à jour le graphe (ne met **PAS** à jour les branches!): `git fetch`
    - la position des branches distante est indiqué par le préfixe `origin/`. C'est également affiché dans VSCode avec le même mot clé.
    - > **Note :** Si la position `origin/nom` est différente de `nom` (ils ne sont pas sur le même noeud), alors c'est qu'elles ne sont pas synchronisées, leur différence correspond en fait aux commits se trouvant entre les deux noeuds.



## sauvegarder, récupérer, envoyer le code

- `git pull`: récupérer les changement de la branche courrante depuis la branche distante correspondante (GitHub) et les recopier localement.
- `git push`: envoyer les changement de la branche courrante sur le dépôt GitHub distant.
    - si la branche n'existe pas: `git push -u origin <nom>` (faire un TAB au niveau du nom pour autocompléter le nom de la branche)
- sauvegarder le travail:
    - `git status`: voir les changements
        - en rouge, à ajouté via add,
        - en vert, déjà pris en compte.
    - `git add *`: prendre en compte tous les changements
    - `git commit -m "mon message"`: sauvegarder le travail sur la branche (uniquement ce qui a été ajouté via add)
        - Il est recommandé de commit régulièrement à chaque petit changement plutôt que de tout commit d'un coup, pour pouvoir facilement revenir en arrière sans perdre tout son travail.
        - Pour s'y retrouver dans le futur, utiliser un message concit au présent résumant les changements. Par exemple:
            - "ajoute ceci" / "add this"
            - "corrige tel probleme" / "fix this problem"



## Comment travailler

1. Se rendre sur `develop`.
2. Faire `git pull` pour mettre à jour le dépôt local avec les dernier changements.
3. Créer une branche **en s'assurant que la branche courrante est develop**
    - Utiliser des lettres minuscules sans accent, des chiffres et des "-" (du 6), par exemple "feat-ajout-balle", "fix-probleme-collision", etc.
    - Le nom de la branche **doit être** la branche courrante pour commencer à coder, à vérifier!
4. Coder
5. Faire des commits. S'il y a besoin de partager le code, pousser la branche sur GitHub avec `git push`.
6. Une fois prêt, pousser une dernière fois via `git push`.
7. Sur GitHub, aller dans l'onglet "Pull requests", et en créer un. C'est une demande de fusion du code.
    - Choisir votre branche comme source.
    - Choisir `develop` comme destination.
8. Le code est relu et approuvé par les autres
    - S'il n'est pas approuvé, il suffit de continuer à commit et push pour mettre à jour le code du pull request. Des messages l'indiqueront.
9. Le merge est fait, retourner en **1** pour la prochaine tâche.



## A ne pas faire

- `git push -f` pour force : dangereux et peut casser le dépôt distant (globalement, si y a force dans une commande c'est à éviter)
- faire des commits sur main ou develop / sur la branche de quelqu'un d'autre
    - vous aurez des conflits au moment de push/pull, et des ennuis juste après
    - des commandes existent pour annuler des commits avant de push/pull, me prévenir.
- `git rebase` (avancé, se concerter)

## Mot de la fin

- Il y a beaucoup de tutoriels en ligne, de pages d'aide stack overflow, ne pas hésiter à les consulter, **sans copier aveuglément les commandes**.
- documentation: `git <commande> --help`.
******************************************************************************************************
		README pour environement de developement de Slg3DScanner
******************************************************************************************************





I/ Version et developpement (section mis à jour régulièrement)
Slg3DScanner est mon prototype dans le cadre de ma maitrise pour faire du Scan 3D. 
Pour le moment seul la lecture et l'affichage du mesh cloud est implémenté à partir d'un fichier lu et généré avec un projet utilisant Optrix de NVidia (non inclus dans la solution pour le moment due à des raisons techniques et par manque de temps pour les résoudre).

Le code et le suivi du développement est aussi présent dans le projet github : https://github.com/SunlayGGX/Slg3DScanner

Sinon pour avoir un apperçu des changements apportés au fil du temps et connaitre si un projet à changé entre 2 .zip, un fichier Update.txt a été placé à la racine du dossier mère et sera mis à jour régulièrements des nouveaux changements.

Pour naviguer dans la scene 3d :
Translation droite/gauche/bas/haut = flèche directionnelle du clavier
Translation avant/arrière = touche 2 (arrière) - touche 8 (avant)





******************************************************************************************************

II/ Organisation du projet
NB : Les versions debug sont suffixé d'un "_d". Les version Releases n'en n'ont pas lors de la génération des éléments de Slg3DScanner.

La solution se décompose en 13 sous projet appartenant à 2 partie (Application et Tool) :

- Application se compose de 10 projets :
• Slg3DScanner (executable). C'est mon application principale ... Elle gère le lancement, le nettoyage et le bon execution de l'application d'un haut niveau.

• Slg3DScanner (librairie statique) posant les bases et les "objets" helper pour tous les projets Application

• SlgCoreEngine (librairie statique) contient le Core de l'application chargé d'assurer les actions de test et de ne pas polluer avec du code de developpement et de test le projet de l'application principale.

• SlgInputEngine (librairie statique) chargé de lire les input Keyboard (et Gamepad XBox (car provenant d'un autre projet personnelle de fabrication d'un GameEngine custom)). Le projet se sert de direct input et Xinput (2 librairies de DirectX)

• SlgLogger (librairie statique) fourni le logger afin de logger les évènements dans la console

• SlgRenderBase (librairie statique) fourni une base (objet de rendu) pour tous les projets en ayant besoin sans forcément devoir inclure le lourd RenderEngine. Il défini et normalise les structures entres les projets de développement à communiquer avec le RenderEngine.

• SlgRenderEngine (librairie statique) est l'engin de Rendu.

• SlgRessource (librairie statique) commande le loading de ressources. Pour le moment, seul le binaire pure et le format SlgBinPos (format binaire custom-made pour point cloud) sont implémentés.

• SlgSurfaceComputation (librairie statique) sert de librairie de calcul pour reconstruire des Meshes à partir de point cloud. C'est dedans que se trouve l'implémentation de mes essais pour la reconstruction de mesh à partir de point cloud. Je l'ai faite séparée du reste de l'application pour ne pas polluer celle-ci de code d'essai, parfois non fonctionnels.

• SlgWindow (librairie statique) génère et possède les ressources à la bonne production de la fenètre utilisé par l'application.


- Tool se compose de 4 projets :
• le ToolHelper (librairie statique) posant les bases et les "objets" helper pour tous les projets Tool

• l'Installer (executable) chargé d'installer l'environnement de developpement facilement sur n'importe quel plateforme Windows 7 ou plus (voir sections III, IV et V)

• l'Uninstaller (executable) chargé de nettoyer ce qu'à fait l'Installer (voir sections IV et VI)

• Slg3DScannerPackager (executable), pour moi, il me sert à packager automatiquement dans un dossier temporaire (dans un dossier qu'il créé nommé SLG_TEMP), tous les dossiers et fichiers nécessaires de mon environnement de travail, l'Installer, l'Uninstaller, ... dans un seul dossier, placés intelligemments et fait automatiquement un zip. La seul chose manuelle à faire et de pousser le zip sur le drive.





******************************************************************************************************

III/ Minimum Requis:
- Pour builder, compiler et Linker le dossier, il est impératif de posséder visual studio 2017 et son environnement de développement (certaines features sont compilées utilisant les dernières nouveauté de C++17)
- Version supérieure ou égale à Windows 7-64 bit





******************************************************************************************************

IV/ NB:
- Ne changez pas l'emplacements des dossiers SlgDependencies ou Slg3DScanner (le dossier de la solution) à l'interieur du dossier mère Slg3DScanner pour les sortir de celui-ci. 
- Ne changez pas la position de l'Installer_d.exe. Celui-ci doit rester à la racine du dossier mère.
- Ne changez pas l'emplacements des dossiers des projets vis-à-vis de la solution.
- Une fois que vous en avez fini avec le Projet Slg3DScanner. Il est important de suivre l'étape de Désinstallation si vous avez réalisé l'étape d'installation jusqu'au bout.





******************************************************************************************************

V/ Etape d'installation:
- Lancez l'executable Installer_d.exe

Celui-ci va écrire 2 variables d'environement dans User (Rappel pour voir, modifier ou supprimer des variables d'environnement : Panneau de configuration > Système et Sécurité > Système > Paramètres système avancés > Variables d'environnement) :
• SLG_DEVEL pointera sur le path vers l'environnement de développement (Soit Slg3DScanner)
• SLG_DEPEND pointera sur le path vers les dépendances du projets (là sont toutes les librairies statiques, dll, ...)

- Assurrez vous que le projet de démarrage dans Visual studio est Slg3DScanner après avoir ouvert le fichier solution Slg3DScanner.sln

- régénérer (Rebuild). Cette étape est rapide (J'ai fait en sorte que cela le soit. Elle ne devrait pas durer plus de 30 secondes ;) ...).

- Et c'est tout... (en cas de problème, notamment liées au dll, référez vous à la section VII)





******************************************************************************************************

VI/ Desinstallation:
- Lancez l'executable Uninstaller_d.exe
(Celui-ci retirera les variables d'environnement SLG_DEVEL et SLG_DEPEND présent dans User (ne le fait pas dans Système) installées.

- Supprimez le dossier mère Slg3DScanner et tout ce qu'il contient.

NB : Parfois, il se pourrait que vous ayez à redémarrer l'ordinateur pour que la cache de visual studio prenne en compte que les variables d'environnements n'existe plus.





******************************************************************************************************

VII/ En cas de problème :

- dll liées à DirectX (D3D**, XInput**, DINPUT**, D3DX**, ...) manquante, il se pourrait que vous ayez à copier le contenu de Dependencies/DirectX/Dll dans le dossier contenant les éxécutables. Si le problème persiste, installer la version la plus récente de DirectX. 

- Une dll particulière de DirectX est utilisée (Je load manuellement celle-ci utilisant LoadLibrary) : D3Dcompiler_47.dll. Celle-ci ne peut pas être substituée à une version antérieure (D3Dcompiler_43.dll sur Windows 7...).
Installer DirectX ne résout pas le soucis pour cette dll particulière si vous êtes sur Windows7. Si c'est le cas, j'ai inclus celle-ci dans Dependencies/DirectX/Dll. Copiez là dans le dossier de l'executable...



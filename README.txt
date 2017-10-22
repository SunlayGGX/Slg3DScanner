******************************************************************************************************
		README pour environement de developement de Slg3DScanner
******************************************************************************************************





I/ Version et developpement (section mis � jour r�guli�rement)
Slg3DScanner est mon prototype dans le cadre de ma maitrise pour faire du Scan 3D. 
Pour le moment seul la lecture et l'affichage du mesh cloud est impl�ment� � partir d'un fichier lu et g�n�r� avec un projet utilisant Optrix de NVidia (non inclus dans la solution pour le moment due � des raisons techniques et par manque de temps pour les r�soudre).

Le code et le suivi du d�veloppement est aussi pr�sent dans le projet github : https://github.com/SunlayGGX/Slg3DScanner

Sinon pour avoir un apper�u des changements apport�s au fil du temps et connaitre si un projet � chang� entre 2 .zip, un fichier Update.txt a �t� plac� � la racine du dossier m�re et sera mis � jour r�guli�rements des nouveaux changements.

Pour naviguer dans la scene 3d :
Translation droite/gauche/bas/haut = fl�che directionnelle du clavier
Translation avant/arri�re = touche 2 (arri�re) - touche 8 (avant)





******************************************************************************************************

II/ Organisation du projet
NB : Les versions debug sont suffix� d'un "_d". Les version Releases n'en n'ont pas lors de la g�n�ration des �l�ments de Slg3DScanner.

La solution se d�compose en 13 sous projet appartenant � 2 partie (Application et Tool) :

- Application se compose de 10 projets :
� Slg3DScanner (executable). C'est mon application principale ... Elle g�re le lancement, le nettoyage et le bon execution de l'application d'un haut niveau.

� Slg3DScanner (librairie statique) posant les bases et les "objets" helper pour tous les projets Application

� SlgCoreEngine (librairie statique) contient le Core de l'application charg� d'assurer les actions de test et de ne pas polluer avec du code de developpement et de test le projet de l'application principale.

� SlgInputEngine (librairie statique) charg� de lire les input Keyboard (et Gamepad XBox (car provenant d'un autre projet personnelle de fabrication d'un GameEngine custom)). Le projet se sert de direct input et Xinput (2 librairies de DirectX)

� SlgLogger (librairie statique) fourni le logger afin de logger les �v�nements dans la console

� SlgRenderBase (librairie statique) fourni une base (objet de rendu) pour tous les projets en ayant besoin sans forc�ment devoir inclure le lourd RenderEngine. Il d�fini et normalise les structures entres les projets de d�veloppement � communiquer avec le RenderEngine.

� SlgRenderEngine (librairie statique) est l'engin de Rendu.

� SlgRessource (librairie statique) commande le loading de ressources. Pour le moment, seul le binaire pure et le format SlgBinPos (format binaire custom-made pour point cloud) sont impl�ment�s.

� SlgSurfaceComputation (librairie statique) sert de librairie de calcul pour reconstruire des Meshes � partir de point cloud. C'est dedans que se trouve l'impl�mentation de mes essais pour la reconstruction de mesh � partir de point cloud. Je l'ai faite s�par�e du reste de l'application pour ne pas polluer celle-ci de code d'essai, parfois non fonctionnels.

� SlgWindow (librairie statique) g�n�re et poss�de les ressources � la bonne production de la fen�tre utilis� par l'application.


- Tool se compose de 4 projets :
� le ToolHelper (librairie statique) posant les bases et les "objets" helper pour tous les projets Tool

� l'Installer (executable) charg� d'installer l'environnement de developpement facilement sur n'importe quel plateforme Windows 7 ou plus (voir sections III, IV et V)

� l'Uninstaller (executable) charg� de nettoyer ce qu'� fait l'Installer (voir sections IV et VI)

� Slg3DScannerPackager (executable), pour moi, il me sert � packager automatiquement dans un dossier temporaire (dans un dossier qu'il cr�� nomm� SLG_TEMP), tous les dossiers et fichiers n�cessaires de mon environnement de travail, l'Installer, l'Uninstaller, ... dans un seul dossier, plac�s intelligemments et fait automatiquement un zip. La seul chose manuelle � faire et de pousser le zip sur le drive.





******************************************************************************************************

III/ Minimum Requis:
- Pour builder, compiler et Linker le dossier, il est imp�ratif de poss�der visual studio 2017 et son environnement de d�veloppement (certaines features sont compil�es utilisant les derni�res nouveaut� de C++17)
- Version sup�rieure ou �gale � Windows 7-64 bit





******************************************************************************************************

IV/ NB:
- Ne changez pas l'emplacements des dossiers SlgDependencies ou Slg3DScanner (le dossier de la solution) � l'interieur du dossier m�re Slg3DScanner pour les sortir de celui-ci. 
- Ne changez pas la position de l'Installer_d.exe. Celui-ci doit rester � la racine du dossier m�re.
- Ne changez pas l'emplacements des dossiers des projets vis-�-vis de la solution.
- Une fois que vous en avez fini avec le Projet Slg3DScanner. Il est important de suivre l'�tape de D�sinstallation si vous avez r�alis� l'�tape d'installation jusqu'au bout.





******************************************************************************************************

V/ Etape d'installation:
- Lancez l'executable Installer_d.exe

Celui-ci va �crire 2 variables d'environement dans User (Rappel pour voir, modifier ou supprimer des variables d'environnement : Panneau de configuration > Syst�me et S�curit� > Syst�me > Param�tres syst�me avanc�s > Variables d'environnement) :
� SLG_DEVEL pointera sur le path vers l'environnement de d�veloppement (Soit Slg3DScanner)
� SLG_DEPEND pointera sur le path vers les d�pendances du projets (l� sont toutes les librairies statiques, dll, ...)

- Assurrez vous que le projet de d�marrage dans Visual studio est Slg3DScanner apr�s avoir ouvert le fichier solution Slg3DScanner.sln

- r�g�n�rer (Rebuild). Cette �tape est rapide (J'ai fait en sorte que cela le soit. Elle ne devrait pas durer plus de 30 secondes ;) ...).

- Et c'est tout... (en cas de probl�me, notamment li�es au dll, r�f�rez vous � la section VII)





******************************************************************************************************

VI/ Desinstallation:
- Lancez l'executable Uninstaller_d.exe
(Celui-ci retirera les variables d'environnement SLG_DEVEL et SLG_DEPEND pr�sent dans User (ne le fait pas dans Syst�me) install�es.

- Supprimez le dossier m�re Slg3DScanner et tout ce qu'il contient.

NB : Parfois, il se pourrait que vous ayez � red�marrer l'ordinateur pour que la cache de visual studio prenne en compte que les variables d'environnements n'existe plus.





******************************************************************************************************

VII/ En cas de probl�me :

- dll li�es � DirectX (D3D**, XInput**, DINPUT**, D3DX**, ...) manquante, il se pourrait que vous ayez � copier le contenu de Dependencies/DirectX/Dll dans le dossier contenant les �x�cutables. Si le probl�me persiste, installer la version la plus r�cente de DirectX. 

- Une dll particuli�re de DirectX est utilis�e (Je load manuellement celle-ci utilisant LoadLibrary) : D3Dcompiler_47.dll. Celle-ci ne peut pas �tre substitu�e � une version ant�rieure (D3Dcompiler_43.dll sur Windows 7...).
Installer DirectX ne r�sout pas le soucis pour cette dll particuli�re si vous �tes sur Windows7. Si c'est le cas, j'ai inclus celle-ci dans Dependencies/DirectX/Dll. Copiez l� dans le dossier de l'executable...



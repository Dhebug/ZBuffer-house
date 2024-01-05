'
' G‚n‚rateur d'univers 3D
'
' - Une liste de points (x,y,z,c)
' - Une liste de polygones (n,p1,p2,p3,...,pn)
'
'
SCREEN 13, 0, 0, 0

t$(0) = "texture_mur"
t$(1) = "texture_bois"
t$(2) = "texture_pierre"
t$(3) = "texture_sol"
t$(4) = "texture_porte"
t$(5) = "texture_fenetre"

fx = 5
fy = fx * 85 / 80

OPEN "o", #1, "E:\ZBUFFER\INCLUDES\MAISON.CPP"
RESTORE entete
DO
  READ n$
  IF n$ <> "<<END>>" THEN
    PRINT #1, n$
  END IF
LOOP UNTIL n$ = "<<END>>"

RESTORE points
READ cx, cy

PRINT #1, "struct point3d far ps[]="
PRINT #1, "{"
np = 0
DO
  READ x, y, z, c
  IF x <> 9999 THEN
    np = np + 1
    x = (x - cx) * fx
    y = (y - cy) * fx
    z = z * fy
    PRINT #1, "{"; x; ","; y; ","; -z; ","; c; "},"
  END IF
LOOP UNTIL x = 9999
PRINT #1, "};"
PRINT #1, " "

RESTORE faces
PRINT #1, "struct facette far f[]="
PRINT #1, "{"
nf = 0
DO
  READ a, b, c, d, t
  IF a <> 9999 THEN
    nf = nf + 1
    IF t = -1 THEN
      PRINT #1, "{1,0,"; t$(0); ",4,{"; a; ","; b; ","; c; ","; d; "}},"
    ELSE
      READ lum
      PRINT #1, "{2,"; lum; ","; t$(t); ",4,{"; a; ","; b; ","; c; ","; d; "}},"
    END IF
  END IF
LOOP UNTIL a = 9999
PRINT #1, "};"
CLOSE
PRINT

OPEN "o", #1, "E:\ZBUFFER\INCLUDES\NB_DATA.H"
PRINT #1, "//"
PRINT #1, "// G‚n‚r‚ avec X-Edit (c) 1993-94 NeXT"
PRINT #1, "//"
PRINT #1, "#define nb_point "; np
PRINT #1, "#define nb_face "; nf
CLOSE
CLS
PRINT "Points="; np
PRINT "Faces="; nf
END

points:
DATA 3.5,4.5

'
' Le plancher
'
' (0 … 17)
'
DATA 0,0,0,192
DATA 1,0,0,192
DATA 2,0,0,192
DATA 3,0,0,192
DATA 4,0,0,192
DATA 5,0,0,192
DATA 6,0,0,208
DATA 6,1,0,210
DATA 6,2,0,210
DATA 5,2,0,210
DATA 5,3,0,210
DATA 4,3,0,210
DATA 3,3,0,210
DATA 2,3,0,210
DATA 1,3,0,208
DATA 0,3,0,192
DATA 0,2,0,192
DATA 0,1,0,192
'
' Le haut des murs
'
' (18 … 35)
'
DATA 0,0,1,0
DATA 1,0,1,0
DATA 2,0,1,0
DATA 3,0,1,0
DATA 4,0,1,0
DATA 5,0,1,0
DATA 6,0,1,0
DATA 6,1,1,0
DATA 6,2,1,0
DATA 5,2,1,0
DATA 5,3,1,0
DATA 4,3,1,0
DATA 3,3,1,0
DATA 2,3,1,0
DATA 1,3,1,0
DATA 0,3,1,0
DATA 0,2,1,0
DATA 0,1,1,0
'
' La toiture
'
' (36-37)
'
DATA 5,1,1.5,0
DATA 6,1,1.5,0
'
' Le chemin de terre devant la porte
'
' (38-40)
'
DATA 6,3,0,210
DATA 5,4,0,210
DATA 6,4,0,210
'
' L'herbe autour de la maison
'
' (41-64)
'
DATA 7,4,0,224
DATA 7,3,0,220
DATA 7,2,0,224
DATA 7,1,0,228
DATA 7,0,0.1,224
DATA 7,-1,0.2,220
DATA 6,-1,0.1,214
DATA 5,-1,0,220
DATA 4,-1,0,224
DATA 3,-1,0,226
DATA 2,-1,0,228
DATA 1,-1,0,229
DATA 0,-1,0,224
DATA -1,-1,0,224
DATA -1,0,0,224
DATA -1,1,0,224
DATA -1,2,0,230
DATA -1,3,0,224
DATA -1,4,0,224
DATA 0,4,0,224
DATA 1,4,0,215
DATA 2,4,0,224
DATA 3,4,0,224
DATA 4,4,0,224
'
' La toiture de gauche
'
' (65-67)
'
DATA 0,1,1.5,0
DATA 1,1,1.5,0
DATA 2,1,1.5,0
'
' La toiture du milieu
'
' (68-71)
'
DATA 2,1.5,1.7,0
DATA 3,1.5,1.7,0
DATA 4,1.5,1.7,0
DATA 5,1.5,1.7,0
DATA 2,1.5,1,0
DATA 5,1,1,0
'
' La colline derriŠre la maison
'
' (74-82)
'
DATA 7,-2,0.3,210
DATA 6,-2,0.2,215
DATA 5,-2,0.1,220
DATA 4,-2,0,224
DATA 3,-2,0,210
DATA 2,-2,0,220
DATA 1,-2,0,224
DATA 0,-2,0,224
DATA -1,-2,0,224
'
' Un autre rang‚e toujours derriŠre la maison...
'
' (83-91)
'
DATA 7,-3,0.4,205
DATA 6,-3,0.3,210
DATA 5,-3,0.2,215
DATA 4,-3,0.1,217
DATA 3,-3,0.1,220
DATA 2,-3,0,224
DATA 1,-3,0,230
DATA 0,-3,0,232
DATA -1,-3,0,228
'
' La chemin‚e
'
' (92-99)
'
DATA 1,1.5,1.75,0
DATA 1,2,1.75,0
DATA 1.5,2,1.75,0
DATA 1.5,1.5,1.75,0
DATA 1,1.5,1.3,0
DATA 1,2,1.3,0
DATA 1.5,2,1.3,0
DATA 1.5,1.5,1.3,0
'
' Le reste du chemin avant le pont
'
' (100-105)
'
DATA 4.5,5,0.1,224
DATA 5.5,5.5,0.1,224
DATA 4,6,0.25,224
DATA 5,6.5,0.25,224
DATA 3,7,0.3,224
DATA 4,7.5,0.3,224
'
' Le reste du chemin aprŠs le pont
'
' (106-111)
'
DATA 2,8,0.35,224
DATA 3,8.5,0.35,224
DATA 1,9,0.25,224
DATA 1.5,9.5,0.2,224
DATA 0.5,10.5,0.1,224
DATA 1.5,11,0.1,224
'
' La riviŠre...
'
' (112-129)
'
DATA -1,5.5,0,160
DATA -1.5,6,0,180
DATA 0.5,6,0,160
DATA -0.5,6.5,0,180
DATA 1,6.5,0,160
DATA 0.5,7,0,180
DATA 2.5,7,0,160
DATA 2,7.5,0,180
DATA 3.5,7.5,0,160
DATA 3,8,0,180
DATA 4.5,8,0,160
DATA 3.5,8.5,0,180
DATA 5.5,8.5,0,160
DATA 4.5,9,0,190
DATA 6,9,0,170
DATA 5.5,9.5,0,190
DATA 7.5,9.5,0,160
DATA 7,10.5,0,180
'
' L'herbe devant la maison, … gauche du chemin jusqu'… la riviŠre
'
' (130-147)
'
DATA -1,4.5,0.1,224
DATA 0,4.5,0.1,224
DATA 1,5,0.1,228
DATA 2,5,0.1,235
DATA 3,5,0.1,230
DATA 4,5,0.1,225
'
DATA -1.5,5,0.1,225
DATA -0.3,5,0.1,215
DATA 0.8,5.5,0.1,225
DATA 1.5,6,0.1,220
DATA 2.5,6,0.1,223
DATA 3,6,0.1,225
'
DATA -1,5.5,0.1,225
DATA -0.5,5.5,0.1,220
DATA 0.5,6,0.12,225
DATA 1,6.5,0.1,205
DATA 2,6.5,0.15,210
DATA 2.5,6.6,0.2,220
'
' Compl‚ments de riviŠre pour jonction avec la berge
'
' (148-153)
'
DATA -1,6.4,0,160
DATA -0.5,5.7,0,170
DATA 1.4,7.2,0,160
DATA 1.7,6.6,0,165
DATA 2.5,7.7,0,180
DATA 3,7.25,0,170
'
' L'herbe devant la maison, … droite du chemin jusqu'… la riviŠre
'
' (154-147)
'
DATA 4.5,7.75,0.25,220
DATA 5.5,8.3,0.2,215
DATA 6,8.75,0.1,220
DATA 7.5,9.25,0.1,224
'
DATA 5.5,7,0,224
DATA 6,7,0,226
DATA 6.5,7.25,0,230
DATA 7.5,7.5,0,226
'
DATA 6,5.9,0,224
DATA 6.5,6,0,220
DATA 7,6,0,215
DATA 7.5,6,0,224
'
DATA 6.5,4.5,0,227
DATA 7,4.5,0,210
DATA 8,5,0,224

DATA 9999,9999,9999,9999






'
' D‚claration des polygones:
' - Point 1
' - Point 2
' - Point 3
' - Point 4
' - Texture (0-5), -1 pour le Gouraud
'
faces:
'
' Les murs de soutenement
'
DATA 19,1,0,18,0,32
DATA 20,2,1,19,0,32
DATA 21,3,2,20,5,32
DATA 22,4,3,21,0,32
DATA 23,5,4,22,0,32
DATA 24,6,5,23,5,32
'
DATA 25,7,6,24,0,55
DATA 26,8,7,25,5,55
'
DATA 27,9,8,26,4,63
'
DATA 28,10,9,27,0,55
'
DATA 29,11,10,28,0,63
DATA 30,12,11,29,5,63
DATA 31,13,12,30,0,63
DATA 32,14,13,31,5,63
DATA 33,15,14,32,0,63
'
DATA 34,16,15,33,0,40
DATA 35,17,16,34,5,40
DATA 18,0,17,35,0,40
'
' La toiture de l'entr‚e … droite
'
DATA 36,27,26,37,1,63
DATA 37,24,23,36,1,50
DATA 37,26,25,37,0,55
DATA 37,25,24,37,0,55
'
' Le chemin de terre devant la porte
'
DATA 10,38,8,9,3,63
DATA 39,40,38,10,3,63
'
' L'herbe autour de la maison
'
DATA 38,40,41,42,-1
DATA 8,38,42,43,-1
DATA 7,8,43,44,-1
DATA 6,7,44,45,-1
DATA 47,6,45,46,-1
DATA 48,5,6,47,-1
DATA 49,4,5,48,-1
DATA 50,3,4,49,-1
DATA 51,2,3,50,-1
DATA 52,1,2,51,-1
DATA 53,0,1,52,-1
DATA 54,55,0,53,-1
DATA 55,56,17,0,-1
DATA 56,57,16,17,-1
DATA 57,58,15,16,-1
DATA 58,59,60,15,-1
DATA 15,60,61,14,-1
DATA 14,61,62,13,-1
DATA 13,62,63,12,-1
DATA 12,63,64,11,-1
DATA 11,64,39,10,-1
'
' La toiture de gauche
'
DATA 65,33,32,66,1,63
DATA 66,32,31,67,1,63
DATA 67,20,19,66,1,50
DATA 66,19,18,65,1,50
'
' La toiture du milieu
'
DATA 68,31,30,69,1,63
DATA 69,30,29,70,1,63
DATA 70,29,28,71,1,63
DATA 71,23,22,70,1,48
DATA 70,22,21,69,1,48
DATA 69,21,20,68,1,48
'
' Le mur du cot‚ gauche de la toiture de gauche
'
DATA 65,18,35,65,0,40
DATA 65,35,33,65,0,40
'
' Le mur du cot‚ gauche de la toiture centrale
'
DATA 68,20,72,68,0,40
DATA 68,72,31,68,0,40
'
' Le mur du cot‚ droit de la toiture centrale
'
DATA 71,28,73,36,0,55
DATA 71,73,23,36,0,55
'
' La seconde bande d'herbe derriŠre la maison
'
DATA 75,47,46,74,-1
DATA 76,48,47,75,-1
DATA 77,49,48,76,-1
DATA 78,50,49,77,-1
DATA 79,51,50,78,-1
DATA 80,52,51,79,-1
DATA 81,53,52,80,-1
DATA 82,54,53,81,-1
'
' La troiŠme bande d'herbe derriŠre la maison
'
DATA 84,75,74,83,-1
DATA 85,76,75,84,-1
DATA 86,77,76,85,-1
DATA 87,78,77,86,-1
DATA 88,79,78,87,-1
DATA 89,80,79,88,-1
DATA 90,81,80,89,-1
DATA 91,82,81,90,-1
'
' La chemin‚e
'
DATA 92,96,97,93,0,40
DATA 93,97,98,94,0,63
DATA 94,98,99,95,0,55
DATA 95,99,96,92,0,32
'
' Le reste du chemin de terre avant le pont
'
DATA 100,101,40,39,3,63
DATA 102,103,101,100,3,63
DATA 104,105,103,102,3,63
'
' Le reste du chemin de terre aprŠs le pont
'
DATA 108,109,107,106,3,63
DATA 110,111,109,108,3,63
'
' La riviŠre
'
DATA 112,113,148,149,-1
DATA 149,148,115,114,-1
DATA 114,115,117,116,-1
DATA 116,117,150,151,-1
DATA 151,150,119,118,-1
DATA 118,119,152,153,-1
DATA 153,152,121,120,-1
DATA 120,121,123,122,-1
DATA 122,123,125,124,-1
DATA 124,125,127,126,-1
DATA 126,127,129,128,-1
'
' L'herbe devant la maison, … gauche du chemin jusqu'… la riviŠre
'
DATA 59,130,131,60,-1
DATA 60,131,132,61,-1
DATA 61,132,133,62,-1
DATA 62,133,134,63,-1
DATA 63,134,135,64,-1
DATA 64,135,100,39,-1
'
DATA 130,136,137,131,-1
DATA 131,137,138,132,-1
DATA 132,138,139,133,-1
DATA 133,139,140,134,-1
DATA 134,140,141,135,-1
DATA 135,141,102,100,-1
'
DATA 136,142,143,137,-1
DATA 137,143,144,138,-1
DATA 138,144,145,139,-1
DATA 139,145,146,140,-1
DATA 140,146,147,141,-1
DATA 141,147,104,102,-1
'
' La terre entre l'herbe et la riviŠre (la berge quoi !)
'
DATA 142,112,149,143,2,55
DATA 143,149,114,144,2,55
DATA 144,114,116,145,2,55
DATA 145,116,151,146,2,55
DATA 146,151,118,147,2,55
DATA 147,118,153,104,2,40
DATA 104,153,120,105,2,30
'
' Le plancher composant le pont entre les deux berges
'
DATA 104,106,107,105,2,32
'
' L'herbe devant la maison, … droite du chemin jusqu'… la riviŠre
'
DATA 103,105,154,158,-1
DATA 158,154,155,159,-1
DATA 159,155,156,160,-1
DATA 160,156,157,161,-1
'
DATA 101,103,158,162,-1
DATA 162,158,159,163,-1
DATA 163,159,160,164,-1
DATA 164,160,161,165,-1
'
DATA 40,101,162,166,-1
DATA 166,162,163,167,-1
DATA 167,163,164,168,-1
'
DATA 40,166,167,41,-1
'
' La suite de la berge, … droite du pont
'
DATA 105,120,122,154,2,55
DATA 154,122,124,155,2,55
DATA 155,124,126,156,2,55
DATA 156,126,128,157,2,55

DATA 9999,9999,9999,9999,9999




entete:
DATA "// "
DATA "// G‚n‚r‚ avec X-Edit (c) 1993-94 NeXT"
DATA "// "
DATA "extern unsigned char texture_mur[];"
DATA "extern unsigned char texture_bois[];"
DATA "extern unsigned char texture_pierre[];"
DATA "extern unsigned char texture_sol[];"
DATA "extern unsigned char texture_porte[];"
DATA "extern unsigned char texture_fenetre[];"
DATA " "
DATA "struct point3d"
DATA "{"
DATA "  float x;"
DATA "  float y;"
DATA "  float z;"
DATA "  float c;"
DATA "};"
DATA " "
DATA "struct facette"
DATA "{"
DATA "  unsigned char type;     // 0-> Uni (=MOTIF) // 1-> Gouraud // 2-> Texture"
DATA "  unsigned char motif;    // Indice de couleur"
DATA "  unsigned char* texture; // Adresse de la texture"
DATA "  unsigned char nb;       // Nombre de points de la facette"
DATA "  unsigned int p[4];      // Liste des points affect‚s … la facette"
DATA "};"
DATA " "
DATA "<<END>>"


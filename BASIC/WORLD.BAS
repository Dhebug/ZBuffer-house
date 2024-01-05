'
' G‚n‚rateur d'univers 3D
'
' - Une liste de points (x,y,z,c)
' - Une liste de polygones (n,p1,p2,p3,...,pn)
'
SCREEN 13, 0, 0, 0
n = 20
hauteur = 20
DIM sol(n, n)
PRINT "G‚nere sol"
FOR y = 1 TO n
  FOR x = 1 TO n
    sol(x, y) = RND * hauteur
  NEXT x
NEXT y

fx = 5

OPEN "o", #1, "E:\ZBUFFER\INCLUDES\MATRICE.H"

PRINT #1, "struct point3d far ps[]="
PRINT #1, "{"
FOR y = 1 TO n
  FOR x = 1 TO n
    PRINT #1, "{"; INT(x - n / 2) * fx; ","; INT(y - n / 2) * fx; ","; sol(x, y); ","; RND * 50; "},"
    np = np + 1
  NEXT x
NEXT y
PRINT #1, "};"
PRINT #1, " "
PRINT #1, "struct facette far f[]="
PRINT #1, "{"
FOR y = 0 TO n - 1
  FOR x = 0 TO n - 1
    a = x + y * n
    b = x + 1 + y * n
    c = x + (y + 1) * n
    d = x + 1 + (y + 1) * n
    couleur = INT(128 + 64 * RND)
    PRINT #1, "{1,"; couleur; ",texture_mur,4,{"; a; ","; b; ","; c; ","; d; "}},"
    nf = nf + 1
  NEXT x
NEXT y
PRINT #1, "};"
CLOSE

PRINT "Points:"; np
PRINT "Faces:"; nf


struct point3d far ps[]=
{
// 0
{-5,5,-5,0},  // Fenetre de derriere
{-5,5,5,0},
{5,5,5,0},
{5,5,-5,0},

// 4
{-5,-5,5,0},	// D‚finition du sol en terre devant la porte (1)
{5,-5,5,0},
{5,-15,5.5,0},
{-5,-15,5.5,0},

// 8
{5,-5,-5,0},   // D‚finition de la porte avant
{5,-5,5,0},
{-5,-5,5,0},
{-5,-5,-5,0},

// 12
{-5,-5,-5,0},  // D‚finition du toit
{-5,5,-5,0},
{5,5,-5,0},
{5,-5,-5,0},

// 16
{-5,5,-5,0},  // Le cot‚ gauche
{-5,5,5,0},
{-5,-5,5,0},
{-5,-5,-5,0},

// 20
{5,5,5,0},    // Le cot‚ droit
{5,5,-5,0},
{5,-5,-5,0},
{5,-5,5,0},

// 24
{-5,-15,5.5,0},	// D‚finition du sol en terre devant la porte (2)
{5,-15,5.5,0},
{5,-25,6,0},
{-5,-25,6.5,0},

// 28
{5,-5,5,200},	// D‚finition de l'herbe … gauche de la porte (1)
{15,-5,5.5,220},
{15,-15,4.5,240},
{5,-15,5.5,200},

// 32
{5,-15,5.5,200},	// D‚finition de l'herbe … gauche de la porte (2)
{15,-15,4.5,240},
{15,-25,5,220},
{5,-25,7,200},

// 36
{-5,-5,5,200},	   // D‚finition de l'herbe … droite de la porte (1)
{-15,-5,5.5,220},
{-15,-15,5.5,240},
{-5,-15,5.5,200},

// 40
{-5,-15,5.5,200},	// D‚finition de l'herbe … droite de la porte (2)
{-15,-15,5.5,240},
{-15,-25,6,220},
{-5,-25,6.5,200},

// 44
{5,5,5,200},	// D‚finition de l'herbe … gauche de la porte (3)
{15,5,5,220},
{15,-5,5.5,220},
{5,-5,5,200},

// 48
{-5,5,5,200},	// D‚finition de l'herbe … droite de la porte (3)
{-15,5,5,220},
{-15,-5,5.5,220},
{-5,-5,5,200},

// 52
{-5,5,5,200},	  // D‚finition de l'herbe devant la fenetre (1)
{5,5,5,220},
{5,15,5,220},
{-5,15,5,200},

// 56
{5,5,5,200},	  // D‚finition de l'herbe … droite de la fenetre (4)
{5,15,5,220},
{15,15,5,220},
{15,5,5,200},

// 60
{-5,5,5,200},	  // D‚finition de l'herbe … gauche de la fenetre (4)
{-5,15,5,220},
{-15,15,5,220},
{-15,5,5,200},

// 64
};

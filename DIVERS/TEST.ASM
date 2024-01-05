	 ;		long x=0x12345678;
	mov	dword ptr [bp-4],large 0x012345678
	 ;
	 ;		int a;
	 ;		int* y;
	 ;		y=(int*)&x;
	lea	ax,word ptr [bp-4]
	mov	si,ax
	 ;
	 ;		a=0;
	xor	dx,dx
	 ;
	 ;		a+=int(x>>16);
	mov	eax,dword ptr [bp-4]
	sar	eax,16
	add	dx,ax
	 ;
	 ;		a=0;
	xor	dx,dx
	 ;
	 ;		a+=*(y+1);
	add	dx,word ptr [si+2]

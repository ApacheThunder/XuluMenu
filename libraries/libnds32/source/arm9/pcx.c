/*---------------------------------------------------------------------------------

 	Copyright (C) 2005
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/
#include <nds/ndstypes.h>
#include <nds/arm9/video.h>
#include <nds/arm9/image.h>
#include <malloc.h>

//---------------------------------------------------------------------------------
int loadPCX(const unsigned char* pcx, sImage* image) {
//---------------------------------------------------------------------------------
	//struct rgb {unsigned char b,g,r;};
	RGB_24* pal;
	
	PCXHeader* hdr = (PCXHeader*) pcx;

	pcx += sizeof(PCXHeader);
	
	unsigned char c;
	int size;
	int count;
	int run;
	int i;
	int iy;
	int width, height;
	int scansize = hdr->bytesPerLine;
	unsigned char *scanline;


	width = image->width  = hdr->xmax - hdr->xmin + 1 ;
	height = image->height = hdr->ymax - hdr->ymin + 1;

	size = image->width * image->height;

	if(hdr->bitsPerPixel != 8)
		return 0;
	
	scanline = image->image.data8 = (unsigned char*)malloc(size);
	image->palette = (unsigned short*)malloc(256 * 2);

	count = 0;

	for(iy = 0; iy < height; iy++) {
		count = 0;
		while(count < scansize)
		{
			c = *pcx++;
			
			if(c < 192) {
				scanline[count++] = c;
			} else {
				run = c - 192;
			
				c = *pcx++;
				
				for(i = 0; i < run && count < scansize; i++)
					scanline[count++] = c;
			}
		}
		scanline += width;
	}

	//check for the palette marker.
	//I have seen PCX files without this, but the docs don't seem ambiguous--it must be here.
	//Anyway, the support among other apps is poor, so we're going to reject it.
	if(*pcx != 0x0C)
	{
		free(image->image.data8);
		image->image.data8 = 0;
		free(image->palette);
		image->palette = 0;
		return 0;
	}

	pcx++;

	pal = (RGB_24*)(pcx);

	image->bpp = 8;

	for(i = 0; i < 256; i++)
	{
		u8 r = (pal[i].r + 4 > 255) ? 255 : (pal[i].r + 4);
		u8 g = (pal[i].g + 4 > 255) ? 255 : (pal[i].g + 4);
		u8 b = (pal[i].b + 4 > 255) ? 255 : (pal[i].b + 4);
		image->palette[i] = RGB15(r >> 3 , g >> 3 , b >> 3) ;
	}
	return 1;
}

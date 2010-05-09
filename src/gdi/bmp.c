/*
    MIL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "MIL_config.h"

/* 
   Code to load and save surfaces in Windows BMP format.

   Why support BMP format?  Well, it's a native format for Windows, and
   most image processing programs can read and write it.  It would be nice
   to be able to have at least one image format that we can natively load
   and save, and since PNG is so complex that it would bloat the library,
   BMP is a good alternative. 

   This code currently supports Win32 DIBs in uncompressed 8 and 24 bpp.
*/

#include "MIL_video.h"
#include "MIL_endian.h"
#include "MIL_rwops.h"
#include "surface.h"

/* Compression encodings for BMP files */
#ifndef BI_RGB
#define BI_RGB		0
#define BI_RLE8		1
#define BI_RLE4		2
#define BI_BITFIELDS	3
#endif


Surface * MIL_LoadBMP_RW(MIL_RWops* src, int freesrc)
{
	MIL_Bool was_error;
	long fp_offset;
	int bmpPitch;
	int i, pad;
	Surface *surface;
	Uint32 Rmask;
	Uint32 Gmask;
	Uint32 Bmask;
	MIL_Palette *palette;
	Uint8 *bits;
	Uint8 *top, *end;
	MIL_Bool topDown;
	int ExpandBMP;

	/* The Win32 BMP file header (14 bytes) */
	char   magic[2];
	Uint32 bfSize;
	Uint16 bfReserved1;
	Uint16 bfReserved2;
	Uint32 bfOffBits;

	/* The Win32 BITMAPINFOHEADER struct (40 bytes) */
	Uint32 biSize;
	Sint32 biWidth;
	Sint32 biHeight;
	Uint16 biPlanes;
	Uint16 biBitCount;
	Uint32 biCompression;
	Uint32 biSizeImage;
	Sint32 biXPelsPerMeter;
	Sint32 biYPelsPerMeter;
	Uint32 biClrUsed;
	Uint32 biClrImportant;

	/* Make sure we are passed a valid data source */
	surface = NULL;
	was_error = MIL_FALSE;
	if ( src == NULL ) {
		was_error = MIL_TRUE;
		goto done;
	}

	/* Read in the BMP file header */
	fp_offset = MIL_RWtell(src);
	MIL_ClearError();
	if ( MIL_RWread(src, magic, 1, 2) != 2 ) {
		MIL_Error(MIL_EFREAD);
		was_error = MIL_TRUE;
		goto done;
	}
	if ( MIL_strncmp(magic, "BM", 2) != 0 ) {
		MIL_SetError("File is not a Windows BMP file");
		was_error = MIL_TRUE;
		goto done;
	}
	bfSize		= MIL_ReadLE32(src);
	bfReserved1	= MIL_ReadLE16(src);
	bfReserved2	= MIL_ReadLE16(src);
	bfOffBits	= MIL_ReadLE32(src);

	/* Read the Win32 BITMAPINFOHEADER */
	biSize		= MIL_ReadLE32(src);
	if ( biSize == 12 ) {
		biWidth		= (Uint32)MIL_ReadLE16(src);
		biHeight	= (Uint32)MIL_ReadLE16(src);
		biPlanes	= MIL_ReadLE16(src);
		biBitCount	= MIL_ReadLE16(src);
		biCompression	= BI_RGB;
		biSizeImage	= 0;
		biXPelsPerMeter	= 0;
		biYPelsPerMeter	= 0;
		biClrUsed	= 0;
		biClrImportant	= 0;
	} else {
		biWidth		= MIL_ReadLE32(src);
		biHeight	= MIL_ReadLE32(src);
		biPlanes	= MIL_ReadLE16(src);
		biBitCount	= MIL_ReadLE16(src);
		biCompression	= MIL_ReadLE32(src);
		biSizeImage	= MIL_ReadLE32(src);
		biXPelsPerMeter	= MIL_ReadLE32(src);
		biYPelsPerMeter	= MIL_ReadLE32(src);
		biClrUsed	= MIL_ReadLE32(src);
		biClrImportant	= MIL_ReadLE32(src);
	}
	if (biHeight < 0) {
		topDown = MIL_TRUE;
		biHeight = -biHeight;
	} else {
		topDown = MIL_FALSE;
	}

	/* Check for read error */
	if ( MIL_strcmp(MIL_GetError(), "") != 0 ) {
		was_error = MIL_TRUE;
		goto done;
	}

	/* Expand 1 and 4 bit bitmaps to 8 bits per pixel */
	switch (biBitCount) {
		case 1:
		case 4:
			ExpandBMP = biBitCount;
			biBitCount = 8;
			break;
		default:
			ExpandBMP = 0;
			break;
	}

	/* We don't support any BMP compression right now */
	Rmask = Gmask = Bmask = 0;
	switch (biCompression) {
		case BI_RGB:
			/* If there are no masks, use the defaults */
			if ( bfOffBits == (14+biSize) ) {
				/* Default values for the BMP format */
				switch (biBitCount) {
					case 15:
					case 16:
						Rmask = 0x7C00;
						Gmask = 0x03E0;
						Bmask = 0x001F;
						break;
					case 24:
#if MIL_BYTEORDER == MIL_BIG_ENDIAN
					        Rmask = 0x000000FF;
					        Gmask = 0x0000FF00;
					        Bmask = 0x00FF0000;
						break;
#endif
					case 32:
						Rmask = 0x00FF0000;
						Gmask = 0x0000FF00;
						Bmask = 0x000000FF;
						break;
					default:
						break;
				}
				break;
			}
			/* Fall through -- read the RGB masks */

		case BI_BITFIELDS:
			switch (biBitCount) {
				case 15:
				case 16:
				case 32:
					Rmask = MIL_ReadLE32(src);
					Gmask = MIL_ReadLE32(src);
					Bmask = MIL_ReadLE32(src);
					break;
				default:
					break;
			}
			break;
		default:
			MIL_SetError("Compressed BMP files not supported");
			was_error = MIL_TRUE;
			goto done;
	}

	/* Create a compatible surface, note that the colors are RGB ordered */
	surface = (Surface*)CreateRGBSurface(MIL_SWSURFACE,
			biWidth, biHeight, biBitCount, Rmask, Gmask, Bmask, 0);
	if ( surface == NULL ) {
		was_error = MIL_TRUE;
		goto done;
	}

	/* Load the palette, if any */
	palette = (surface->format)->palette;
	if ( palette ) {
		if ( biClrUsed == 0 ) {
			biClrUsed = 1 << biBitCount;
		}
		if ( biSize == 12 ) {
			for ( i = 0; i < (int)biClrUsed; ++i ) {
				MIL_RWread(src, &palette->colors[i].b, 1, 1);
				MIL_RWread(src, &palette->colors[i].g, 1, 1);
				MIL_RWread(src, &palette->colors[i].r, 1, 1);
				palette->colors[i].unused = 0;
			}	
		} else {
			for ( i = 0; i < (int)biClrUsed; ++i ) {
				MIL_RWread(src, &palette->colors[i].b, 1, 1);
				MIL_RWread(src, &palette->colors[i].g, 1, 1);
				MIL_RWread(src, &palette->colors[i].r, 1, 1);
				MIL_RWread(src, &palette->colors[i].unused, 1, 1);
			}	
		}
		palette->ncolors = biClrUsed;
	}

	/* Read the surface pixels.  Note that the bmp image is upside down */
	if ( MIL_RWseek(src, fp_offset+bfOffBits, RW_SEEK_SET) < 0 ) {
		MIL_Error(MIL_EFSEEK);
		was_error = MIL_TRUE;
		goto done;
	}
	top = (Uint8 *)surface->pixels;
	end = (Uint8 *)surface->pixels+(surface->h*surface->pitch);
	switch (ExpandBMP) {
		case 1:
			bmpPitch = (biWidth + 7) >> 3;
			pad  = (((bmpPitch)%4) ? (4-((bmpPitch)%4)) : 0);
			break;
		case 4:
			bmpPitch = (biWidth + 1) >> 1;
			pad  = (((bmpPitch)%4) ? (4-((bmpPitch)%4)) : 0);
			break;
		default:
			pad  = ((surface->pitch%4) ?
					(4-(surface->pitch%4)) : 0);
			break;
	}
	if ( topDown ) {
		bits = top;
	} else {
		bits = end - surface->pitch;
	}
	while ( bits >= top && bits < end ) {
		switch (ExpandBMP) {
			case 1:
			case 4: {
			Uint8 pixel = 0;
			int   shift = (8-ExpandBMP);
			for ( i=0; i<surface->w; ++i ) {
				if ( i%(8/ExpandBMP) == 0 ) {
					if ( !MIL_RWread(src, &pixel, 1, 1) ) {
						MIL_SetError(
					"Error reading from BMP");
						was_error = MIL_TRUE;
						goto done;
					}
				}
				*(bits+i) = (pixel>>shift);
				pixel <<= ExpandBMP;
			} }
			break;

			default:
			if ( MIL_RWread(src, bits, 1, surface->pitch)
							 != surface->pitch ) {
				MIL_Error(MIL_EFREAD);
				was_error = MIL_TRUE;
				goto done;
			}
#if MIL_BYTEORDER == MIL_BIG_ENDIAN
			/* Byte-swap the pixels if needed. Note that the 24bpp
			   case has already been taken care of above. */
			switch(biBitCount) {
				case 15:
				case 16: {
				        Uint16 *pix = (Uint16 *)bits;
					for(i = 0; i < surface->w; i++)
					        pix[i] = MIL_Swap16(pix[i]);
					break;
				}

				case 32: {
				        Uint32 *pix = (Uint32 *)bits;
					for(i = 0; i < surface->w; i++)
					        pix[i] = MIL_Swap32(pix[i]);
					break;
				}
			}
#endif
			break;
		}
		/* Skip padding bytes, ugh */
		if ( pad ) {
			Uint8 padbyte;
			for ( i=0; i<pad; ++i ) {
				MIL_RWread(src, &padbyte, 1, 1);
			}
		}
		if ( topDown ) {
			bits += surface->pitch;
		} else {
			bits -= surface->pitch;
		}
	}
done:
	if ( was_error ) {
		if ( src ) {
			MIL_RWseek(src, fp_offset, RW_SEEK_SET);
		}
		if ( surface ) {
			Delete(surface);
		}
		surface = NULL;
	}
	if ( freesrc && src ) {
		MIL_RWclose(src);
	}
	return(surface);
}

int MIL_SaveBMP_RW (Surface *saveme, MIL_RWops *dst, int freedst)
{
	long fp_offset;
	int i, pad;
	Surface *surface;
	Uint8 *bits;

	/* The Win32 BMP file header (14 bytes) */
	char   magic[2] = { 'B', 'M' };
	Uint32 bfSize;
	Uint16 bfReserved1;
	Uint16 bfReserved2;
	Uint32 bfOffBits;

	/* The Win32 BITMAPINFOHEADER struct (40 bytes) */
	Uint32 biSize;
	Sint32 biWidth;
	Sint32 biHeight;
	Uint16 biPlanes;
	Uint16 biBitCount;
	Uint32 biCompression;
	Uint32 biSizeImage;
	Sint32 biXPelsPerMeter;
	Sint32 biYPelsPerMeter;
	Uint32 biClrUsed;
	Uint32 biClrImportant;

	/* Make sure we have somewhere to save */
	surface = NULL;
	if ( dst ) {
		if ( saveme->format->palette ) {
			if ( saveme->format->BitsPerPixel == 8 ) {
				surface = saveme;
			} else {
				MIL_SetError("%d bpp BMP files not supported",
						saveme->format->BitsPerPixel);
			}
		}
		else if ( (saveme->format->BitsPerPixel == 24) &&
#if MIL_BYTEORDER == MIL_LIL_ENDIAN
				(saveme->format->Rmask == 0x00FF0000) &&
				(saveme->format->Gmask == 0x0000FF00) &&
				(saveme->format->Bmask == 0x000000FF)
#else
				(saveme->format->Rmask == 0x000000FF) &&
				(saveme->format->Gmask == 0x0000FF00) &&
				(saveme->format->Bmask == 0x00FF0000)
#endif
			  ) {
			surface = saveme;
		} else {
			MIL_Rect bounds;

			/* Convert to 24 bits per pixel */
			surface = (Surface*)CreateRGBSurface(MIL_SWSURFACE,
					saveme->w, saveme->h, 24,
#if MIL_BYTEORDER == MIL_LIL_ENDIAN
					0x00FF0000, 0x0000FF00, 0x000000FF,
#else
					0x000000FF, 0x0000FF00, 0x00FF0000,
#endif
					0);
			if ( surface != NULL ) {
				bounds.x = 0;
				bounds.y = 0;
				bounds.w = saveme->w;
				bounds.h = saveme->h;
				if ( MIL_LowerBlit(saveme, &bounds, surface,
							&bounds) < 0 ) {
					Delete(surface);
					MIL_SetError(
					"Couldn't convert image to 24 bpp");
					surface = NULL;
				}
			}
		}
	}

	if ( surface && (_vc0(surface, lock) == 0) ) {
		const int bw = surface->w*surface->format->BytesPerPixel;

		/* Set the BMP file header values */
		bfSize = 0;		 /* We'll write this when we're done */
		bfReserved1 = 0;
		bfReserved2 = 0;
		bfOffBits = 0;		/* We'll write this when we're done */

		/* Write the BMP file header values */
		fp_offset = MIL_RWtell(dst);
		MIL_ClearError();
		MIL_RWwrite(dst, magic, 2, 1);
		MIL_WriteLE32(dst, bfSize);
		MIL_WriteLE16(dst, bfReserved1);
		MIL_WriteLE16(dst, bfReserved2);
		MIL_WriteLE32(dst, bfOffBits);

		/* Set the BMP info values */
		biSize = 40;
		biWidth = surface->w;
		biHeight = surface->h;
		biPlanes = 1;
		biBitCount = surface->format->BitsPerPixel;
		biCompression = BI_RGB;
		biSizeImage = surface->h*surface->pitch;
		biXPelsPerMeter = 0;
		biYPelsPerMeter = 0;
		if ( surface->format->palette ) {
			biClrUsed = surface->format->palette->ncolors;
		} else {
			biClrUsed = 0;
		}
		biClrImportant = 0;

		/* Write the BMP info values */
		MIL_WriteLE32(dst, biSize);
		MIL_WriteLE32(dst, biWidth);
		MIL_WriteLE32(dst, biHeight);
		MIL_WriteLE16(dst, biPlanes);
		MIL_WriteLE16(dst, biBitCount);
		MIL_WriteLE32(dst, biCompression);
		MIL_WriteLE32(dst, biSizeImage);
		MIL_WriteLE32(dst, biXPelsPerMeter);
		MIL_WriteLE32(dst, biYPelsPerMeter);
		MIL_WriteLE32(dst, biClrUsed);
		MIL_WriteLE32(dst, biClrImportant);

		/* Write the palette (in BGR color order) */
		if ( surface->format->palette ) {
			MIL_Color *colors;
			int       ncolors;

			colors = surface->format->palette->colors;
			ncolors = surface->format->palette->ncolors;
			for ( i=0; i<ncolors; ++i ) {
				MIL_RWwrite(dst, &colors[i].b, 1, 1);
				MIL_RWwrite(dst, &colors[i].g, 1, 1);
				MIL_RWwrite(dst, &colors[i].r, 1, 1);
				MIL_RWwrite(dst, &colors[i].unused, 1, 1);
			}
		}

		/* Write the bitmap offset */
		bfOffBits = MIL_RWtell(dst)-fp_offset;
		if ( MIL_RWseek(dst, fp_offset+10, RW_SEEK_SET) < 0 ) {
			MIL_Error(MIL_EFSEEK);
		}
		MIL_WriteLE32(dst, bfOffBits);
		if ( MIL_RWseek(dst, fp_offset+bfOffBits, RW_SEEK_SET) < 0 ) {
			MIL_Error(MIL_EFSEEK);
		}

		/* Write the bitmap image upside down */
		bits = (Uint8 *)surface->pixels+(surface->h*surface->pitch);
		pad  = ((bw%4) ? (4-(bw%4)) : 0);
		while ( bits > (Uint8 *)surface->pixels ) {
			bits -= surface->pitch;
			if ( MIL_RWwrite(dst, bits, 1, bw) != bw) {
				MIL_Error(MIL_EFWRITE);
				break;
			}
			if ( pad ) {
				const Uint8 padbyte = 0;
				for ( i=0; i<pad; ++i ) {
					MIL_RWwrite(dst, &padbyte, 1, 1);
				}
			}
		}

		/* Write the BMP file size */
		bfSize = MIL_RWtell(dst)-fp_offset;
		if ( MIL_RWseek(dst, fp_offset+2, RW_SEEK_SET) < 0 ) {
			MIL_Error(MIL_EFSEEK);
		}
		MIL_WriteLE32(dst, bfSize);
		if ( MIL_RWseek(dst, fp_offset+bfSize, RW_SEEK_SET) < 0 ) {
			MIL_Error(MIL_EFSEEK);
		}

		/* Close it up.. */
		_vc0(surface, unlock);
		if ( surface != saveme ) {
			Delete(surface);
		}
	}

	if ( freedst && dst ) {
		MIL_RWclose(dst);
	}
	return((MIL_strcmp(MIL_GetError(), "") == 0) ? 0 : -1);
}

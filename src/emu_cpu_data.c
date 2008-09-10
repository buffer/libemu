/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/


#include "emu/emu_cpu_data.h"


/**
 * int type min/max for signed/unsigned int to 32bits size 
 *  
 * access is 
 * max_inittype_border[bits/8][signed|unsigned][min|max] 
 *  
 * where signed/min is 0 
 * and unsigned/max is 1 
 */
int64_t max_inttype_borders[][2][2] =                                            
{                                                                          
	{                                                                      
		{0, 0},                                                             
		{0, 0},                                                             
	},                                                                     
	{                                                                      
		{MIN_INT8, MAX_INT8},                                              
		{MIN_UINT8, MAX_UINT8},                                            
	},                                                                     
	{                                                                      
		{MIN_INT16, MAX_INT16},                                            
		{MIN_UINT16, MAX_UINT16},                                          
	},                                                                     
	{                                                                      
		{0, 0},                                                             
		{0, 0},                                                             
	},                                                                     
	{                                                                      
		{MIN_INT32, MAX_INT32},                                            
		{MIN_UINT32, MAX_UINT32},                                          
	}                                                                      
};                                                                          


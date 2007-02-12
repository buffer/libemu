/* @header@ */

#include "emu/emu_cpu_data.h"

int64_t max_inttype_borders[][2][2] =                                            
{                                                                          
	{                                                                      
		{0,0},                                                             
		{0,0},                                                             
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
		{0,0},                                                             
		{0,0},                                                             
	},                                                                     
	{                                                                      
		{MIN_INT32, MAX_INT32},                                            
		{MIN_UINT32, MAX_UINT32},                                          
	}                                                                      
};                                                                          


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

#ifndef HAVE_EMU_LOG_H
#define HAVE_EMU_LOG_H

struct emu;

enum emu_log_level
{
	EMU_LOG_NONE,
	EMU_LOG_INFO,
	EMU_LOG_DEBUG
};

typedef void (*emu_log_logcb)(struct emu *e, enum emu_log_level level, const char *msg);

struct emu_logging *emu_log_new(void);
void emu_log_free(struct emu_logging *el);

void emu_log_level_set(struct emu_logging *el, enum emu_log_level level);

void emu_log(struct emu *e, enum emu_log_level level, const char *format, ...);

void emu_log_set_logcb(struct emu_logging *el, emu_log_logcb logcb);

void emu_log_default_logcb(struct emu *e, enum emu_log_level level, const char *msg);

#define logInfo(e, format...) emu_log(e, EMU_LOG_INFO, format)

#ifdef DEBUG
#define logDebug(e, format...) emu_log(e, EMU_LOG_DEBUG, format)
#else
#define logDebug(e, format...)
#endif // DEBUG

#define logPF(e) logDebug(e, "in <%s> %s:%i>\n", __PRETTY_FUNCTION__, __FILE__,  __LINE__)

#endif // HAVE_EMU_LOG_H

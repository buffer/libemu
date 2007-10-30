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

#include <stdint.h>

// for the socket hooks
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_memory.h"
#include "emu/emu_string.h"

int32_t instr_int_cd(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
//	printf("interrupt %02x %i\n",*i->imm8, *c->reg8[al]);
//	uint32_t arg0 = c->reg[ecx];


	switch ( *c->reg8[al] )
	{
	case 1:
		printf("sys_exit\n");
		break;

	case 2:
		printf("sys_fork\n");
		break;

	case 3:
		printf("sys_read\n");
		break;

	case 4:
		printf("sys_write\n");
		break;

	case 5:
		printf("sys_open\n");
		break;

	case 6:
		printf("sys_close\n");
		break;

	case 7:
		printf("sys_waitpid\n");
		break;

	case 8:
		printf("sys_creat\n");
		break;

	case 9:
		printf("sys_link\n");
		break;

	case 10:
		printf("sys_unlink\n");
		break;

	case 11: // sys_execve
		{
			struct emu_string *name = emu_string_new();
			emu_memory_read_string(emu_memory_get(c->emu), c->reg[ebx], name, 255);
			printf("int execve (const char *dateiname=%08x={%s}, const char * argv[], const char *envp[]);\n", 
				   c->reg[ebx],
				   emu_string_char(name));
		}
		break;

	case 12:
		printf("sys_chdir\n");
		break;

	case 13:
		printf("sys_time\n");
		break;

	case 14:
		printf("sys_mknod\n");
		break;

	case 15:
		printf("sys_chmod\n");
		break;

	case 16:
		printf("sys_lchown\n");
		break;

	case 18:
		printf("sys_stat\n");
		break;

	case 19:
		printf("sys_lseek\n");
		break;

	case 20:
		printf("sys_getpid\n");
		break;

	case 21:
		printf("sys_mount\n");
		break;

	case 22:
		printf("sys_oldumount\n");
		break;

	case 23:
		printf("sys_setuid\n");
		break;

	case 24:
		printf("sys_getuid\n");
		break;

	case 25:
		printf("sys_stime\n");
		break;

	case 26:
		printf("sys_ptrace\n");
		break;

	case 27:
		printf("sys_alarm\n");
		break;

	case 28:
		printf("sys_fstat\n");
		break;

	case 29:
		printf("sys_pause\n");
		break;

	case 30:
		printf("sys_utime\n");
		break;

	case 33:
		printf("sys_access\n");
		break;

	case 34:
		printf("sys_nice\n");
		break;

	case 36:
		printf("sys_sync\n");
		break;

	case 37:
		printf("sys_kill\n");
		break;

	case 38:
		printf("sys_rename\n");
		break;

	case 39:
		printf("sys_mkdir\n");
		break;

	case 40:
		printf("sys_rmdir\n");
		break;

	case 41:
		printf("sys_dup\n");
		break;

	case 42:
		printf("sys_pipe\n");
		break;

	case 43:
		printf("sys_times\n");
		break;

	case 45:
		printf("sys_brk\n");
		break;

	case 46:
		printf("sys_setgid\n");
		break;

	case 47:
		printf("sys_getgid\n");
		break;

	case 48:
		printf("sys_signal\n");
		break;

	case 49:
		printf("sys_geteuid\n");
		break;

	case 50:
		printf("sys_getegid\n");
		break;

	case 51:
		printf("sys_acct\n");
		break;

	case 52:
		printf("sys_umount\n");
		break;

	case 54:
		printf("sys_ioctl\n");
		break;

	case 55:
		printf("sys_fcntl\n");
		break;

	case 57:
		printf("sys_setpgid\n");
		break;

	case 59:
		printf("sys_olduname\n");
		break;

	case 60:
		printf("sys_umask\n");
		break;

	case 61:
		printf("sys_chroot\n");
		break;

	case 62:
		printf("sys_ustat\n");
		break;

	case 63: 
		printf("int dup2(int oldfd=%i, int newfd=%i);\n", c->reg[ebx], c->reg[ecx]);
		emu_cpu_reg32_set(c, eax, c->reg[ecx]);
		break;

	case 64:
		printf("sys_getppid\n");
		break;

	case 65:
		printf("sys_getpgrp\n");
		break;

	case 66:
		printf("sys_setsid\n");
		break;

	case 67:
		printf("sys_sigaction\n");
		break;

	case 68:
		printf("sys_sgetmask\n");
		break;

	case 69:
		printf("sys_ssetmask\n");
		break;

	case 70:
		printf("sys_setreuid\n");
		break;

	case 71:
		printf("sys_setregid\n");
		break;

	case 72:
		printf("sys_sigsuspend\n");
		break;

	case 73:
		printf("sys_sigpending\n");
		break;

	case 74:
		printf("sys_sethostname\n");
		break;

	case 75:
		printf("sys_setrlimit\n");
		break;

	case 76:
		printf("sys_getrlimit\n");
		break;

	case 77:
		printf("sys_getrusage\n");
		break;

	case 78:
		printf("sys_gettimeofday\n");
		break;

	case 79:
		printf("sys_settimeofday\n");
		break;

	case 80:
		printf("sys_getgroups\n");
		break;

	case 81:
		printf("sys_setgroups\n");
		break;

	case 82:
		printf("old_select\n");
		break;

	case 83:
		printf("sys_symlink\n");
		break;

	case 84:
		printf("sys_lstat\n");
		break;

	case 85:
		printf("sys_readlink\n");
		break;

	case 86:
		printf("sys_uselib\n");
		break;

	case 87:
		printf("sys_swapon\n");
		break;

	case 88:
		printf("sys_reboot\n");
		break;

	case 89:
		printf("old_readdir\n");
		break;

	case 90:
		printf("old_mmap\n");
		break;

	case 91:
		printf("sys_munmap\n");
		break;

	case 92:
		printf("sys_truncate\n");
		break;

	case 93:
		printf("sys_ftruncate\n");
		break;

	case 94:
		printf("sys_fchmod\n");
		break;

	case 95:
		printf("sys_fchown\n");
		break;

	case 96:
		printf("sys_getpriority\n");
		break;

	case 97:
		printf("sys_setpriority\n");
		break;

	case 99:
		printf("sys_statfs\n");
		break;

	case 100:
		printf("sys_fstatfs\n");
		break;

	case 101:
		printf("sys_ioperm\n");
		break;

	case 102:
//		printf("sys_socketcall\n");
		{
/* Argument list sizes for sys_socketcall */
#define AL(x) (x)
			static unsigned char nargs[18]={AL(0),AL(3),AL(3),AL(3),AL(2),AL(3),
				AL(3),AL(3),AL(4),AL(4),AL(4),AL(6),
				AL(6),AL(2),AL(5),AL(5),AL(3),AL(3)};
#undef AL

			uint32_t a[6];
			int i;
			for (i=0;i<nargs[c->reg[ebx]];i++)
			{
				emu_memory_read_dword(emu_memory_get(c->emu),c->reg[ecx]+4*i,a+i);
			}

			switch ( c->reg[ebx] )
			{
			case 1:	// SYS_SOCKET 
				printf("int socket(int domain=%i, int type=%i, int protocol=%i);\n",
					   a[0],
					   a[1],
					   a[2]);
				emu_cpu_reg32_set(c, eax, 4);
				break;

			case 2:	// SYS_BIND 
				printf("sys_bind(2)\n");
				{
					struct sockaddr sa;
					memset(&sa, 0, sizeof(struct sockaddr));
					emu_memory_read_block(emu_memory_get(c->emu), a[1], &sa, sizeof(struct sockaddr));

					printf("int bind(int sockfd=%i, struct sockaddr *my_addr=%08x={host %s port %i}, int addrlen);\n",
						   a[0],
						   a[1], inet_ntoa(*(struct in_addr *)&((struct sockaddr_in *)&sa)->sin_addr), ntohs(((struct sockaddr_in *)&sa)->sin_port) 
						   );
				}
				emu_cpu_reg32_set(c, eax, 0);
				break;

			case 3:	// SYS_CONNECT 
				{
					struct sockaddr sa;
					memset(&sa, 0, sizeof(struct sockaddr));
					emu_memory_read_block(emu_memory_get(c->emu), a[1], &sa, sizeof(struct sockaddr));

					printf("int connect(int sockfd=%i, struct sockaddr *my_addr=%08x={host %s port %i}, int addrlen);\n",
						   a[0],
						   a[1], inet_ntoa(*(struct in_addr *)&((struct sockaddr_in *)&sa)->sin_addr), ntohs(((struct sockaddr_in *)&sa)->sin_port)
						   );

				}
				break;

			case 4:	// SYS_LISTEN 
				printf("int listen(int s=%i, int backlog=%i);\n", 
					   a[0], 
					   a[1]);
				break;

			case 5:	// SYS_ACCEPT 
                printf("int accept(int s=%i, struct sockaddr *addr=%08x, int *addrlen=%08x);\n",
					   a[0],
					   a[1],
					   a[2]);
				emu_cpu_reg32_set(c, eax, 112);
				break;

			case 6:	// SYS_GETSOCKNAME 
				printf("sys_getsockname(2)\n");
				break;

			case 7:	// SYS_GETPEERNAME 
				printf("sys_getpeername(2)\n");
				break;

			case 8:	// SYS_SOCKETPAIR 
				printf("sys_socketpair(2)\n");
				break;

			case 9:	// SYS_SEND 
				printf("sys_send(2)\n");
				break;

			case 10: // SYS_RECV 
				printf("sys_recv(2)\n");
				break;

			case 11: // SYS_SENDTO 
				printf("sys_sendto(2)\n");
				break;

			case 12: // SYS_RECVFROM 
				printf("sys_recvfrom(2)\n");
				break;

			case 13: // SYS_SHUTDOWN 
				printf("sys_shutdown(2)\n");
				break;

			case 14: // SYS_SETSOCKOPT 
				printf("sys_setsockopt(2)\n");
				break;

			case 15: // SYS_GETSOCKOPT 
				printf("sys_getsockopt(2)\n");
				break;

			case 16: // SYS_SENDMSG 
				printf("sys_sendmsg(2)\n");
				break;

			case 17: // SYS_RECVMSG 
				printf("sys_recvmsg(2)\n");
				break;
			}
		}
		break;

	case 103:
		printf("sys_syslog\n");
		break;

	case 104:
		printf("sys_setitimer\n");
		break;

	case 105:
		printf("sys_getitimer\n");
		break;

	case 106:
		printf("sys_newstat\n");
		break;

	case 107:
		printf("sys_newlstat\n");
		break;

	case 108:
		printf("sys_newfstat\n");
		break;

	case 109:
		printf("sys_uname\n");
		break;

	case 110:
		printf("sys_iopl\n");
		break;

	case 111:
		printf("sys_vhangup\n");
		break;

	case 112:
		printf("sys_idle\n");
		break;

	case 113:
		printf("sys_vm86old\n");
		break;

	case 114:
		printf("sys_wait4\n");
		break;

	case 115:
		printf("sys_swapoff\n");
		break;

	case 116:
		printf("sys_sysinfo\n");
		break;

	case 117:
		printf("sys_ipc\n");
		break;

	case 118:
		printf("sys_fsync\n");
		break;

	case 119:
		printf("sys_sigreturn\n");
		break;

	case 120:
		printf("sys_clone\n");
		break;

	case 121:
		printf("sys_setdomainname\n");
		break;

	case 122:
		printf("sys_newuname\n");
		break;

	case 123:
		printf("sys_modify_ldt\n");
		break;

	case 124:
		printf("sys_adjtimex\n");
		break;

	case 125:
		printf("sys_mprotect\n");
		break;

	case 126:
		printf("sys_sigprocmask\n");
		break;

	case 127:
		printf("sys_create_module\n");
		break;

	case 128:
		printf("sys_init_module\n");
		break;

	case 129:
		printf("sys_delete_module\n");
		break;

	case 130:
		printf("sys_get_kernel_syms\n");
		break;

	case 131:
		printf("sys_quotactl\n");
		break;

	case 132:
		printf("sys_getpgid\n");
		break;

	case 133:
		printf("sys_fchdir\n");
		break;

	case 134:
		printf("sys_bdflush\n");
		break;

	case 135:
		printf("sys_sysfs\n");
		break;

	case 136:
		printf("sys_personality\n");
		break;

	case 138:
		printf("sys_setfsuid\n");
		break;

	case 139:
		printf("sys_setfsgid\n");
		break;

	case 140:
		printf("sys_llseek\n");
		break;

	case 141:
		printf("sys_getdents\n");
		break;

	case 142:
		printf("sys_select\n");
		break;

	case 143:
		printf("sys_flock\n");
		break;

	case 144:
		printf("sys_msync\n");
		break;

	case 145:
		printf("sys_readv\n");
		break;

	case 146:
		printf("sys_writev\n");
		break;

	case 147:
		printf("sys_getsid\n");
		break;

	case 148:
		printf("sys_fdatasync\n");
		break;

	case 149:
		printf("sys_sysctl\n");
		break;

	case 150:
		printf("sys_mlock\n");
		break;

	case 151:
		printf("sys_munlock\n");
		break;

	case 152:
		printf("sys_mlockall\n");
		break;

	case 153:
		printf("sys_munlockall\n");
		break;

	case 154:
		printf("sys_sched_setparam\n");
		break;

	case 155:
		printf("sys_sched_getparam\n");
		break;

	case 156:
		printf("sys_sched_setscheduler\n");
		break;

	case 157:
		printf("sys_sched_getscheduler\n");
		break;

	case 158:
		printf("sys_sched_yield\n");
		break;

	case 159:
		printf("sys_sched_get_priority_max\n");
		break;

	case 160:
		printf("sys_sched_get_priority_min\n");
		break;

	case 161:
		printf("sys_sched_rr_get_interval\n");
		break;

	case 162:
		printf("sys_nanosleep\n");
		break;

	case 163:
		printf("sys_mremap\n");
		break;

	case 164:
		printf("sys_setresuid\n");
		break;

	case 165:
		printf("sys_getresuid\n");
		break;

	case 166:
		printf("sys_vm86\n");
		break;

	case 167:
		printf("sys_query_module\n");
		break;

	case 168:
		printf("sys_poll\n");
		break;

	case 169:
		printf("sys_nfsservctl\n");
		break;

	case 170:
		printf("sys_setresgid\n");
		break;

	case 171:
		printf("sys_getresgid\n");
		break;

	case 172:
		printf("sys_prctl\n");
		break;

	case 173:
		printf("sys_rt_sigreturn\n");
		break;

	case 174:
		printf("sys_rt_sigaction\n");
		break;

	case 175:
		printf("sys_rt_sigprocmask\n");
		break;

	case 176:
		printf("sys_rt_sigpending\n");
		break;

	case 177:
		printf("sys_rt_sigtimedwait\n");
		break;

	case 178:
		printf("sys_rt_sigqueueinfo\n");
		break;

	case 179:
		printf("sys_rt_sigsuspend\n");
		break;

	case 180:
		printf("sys_pread\n");
		break;

	case 181:
		printf("sys_pwrite\n");
		break;

	case 182:
		printf("sys_chown\n");
		break;

	case 183:
		printf("sys_getcwd\n");
		break;

	case 184:
		printf("sys_capget\n");
		break;

	case 185:
		printf("sys_capset\n");
		break;

	case 186:
		printf("sys_sigaltstack\n");
		break;

	case 187:
		printf("sys_sendfile\n");
		break;

	case 190:
		printf("sys_vfork\n");
		break;

	}
	return 0;
}



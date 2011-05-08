
uint32_t user_hook_ExitProcess(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_ExitThread(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_CreateProcess(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_WaitForSingleObject(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_exit(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_accept(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_bind(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_closesocket(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_connect(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_fclose(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_fopen(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_fwrite(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_listen(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_recv(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_send(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_socket(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_WSASocket(struct emu_env *env, struct emu_env_hook *hook, ...);

uint32_t user_hook_CreateFile(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_WriteFile(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_CloseHandle(struct emu_env *env, struct emu_env_hook *hook, ...);

uint32_t user_hook_URLDownloadToFile(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t user_hook_IEWinMain(struct emu_env *env, struct emu_env_hook *hook, ...);

package minhook

foreign import minhook "MinHook.x64.a"
import "core:sys/windows"

MH_ALL_HOOKS :: proc() -> rawptr {
	return nil
}

MH_STATUS :: enum i32 {
	MH_UNKNOWN = -1,
	MH_OK = 0,
	MH_ERROR_ALREADY_INITIALIZED,
	MH_ERROR_NOT_INITIALIZED,
	MH_ERROR_ALREADY_CREATED,
	MH_ERROR_NOT_CREATED,
	MH_ERROR_ENABLED,
	MH_ERROR_DISABLED,
	MH_ERROR_NOT_EXECUTABLE,
	MH_ERROR_UNSUPPORTED_FUNCTION,
	MH_ERROR_MEMORY_ALLOC,
	MH_ERROR_MEMORY_PROTECT,
	MH_ERROR_MODULE_NOT_FOUND,
	MH_ERROR_FUNCTION_NOT_FOUND,
}

@(default_calling_convention = "system")
foreign minhook {
	MH_Initialize :: proc() -> MH_STATUS ---
	MH_Uninitialize :: proc() -> MH_STATUS ---
	MH_CreateHook :: proc(pTarget: windows.LPVOID, lpDetour: windows.LPVOID, ppOriginal: ^windows.LPVOID) -> MH_STATUS ---
	MH_CreateHookApi :: proc(pszModule: windows.LPCWSTR, pszProcName: windows.LPCSTR, pDetour: windows.LPVOID, ppOriginal: ^windows.LPVOID) -> MH_STATUS ---
	MH_CreateHookApiEx :: proc(pszModule: windows.LPCWSTR, pszProcName: windows.LPCSTR, pDetour: ^windows.LPVOID, ppOriginal: ^windows.LPVOID) -> MH_STATUS ---
	MH_RemoveHook :: proc(pTarget: windows.LPVOID) -> MH_STATUS ---
	MH_EnableHook :: proc(pTarget: windows.LPVOID) -> MH_STATUS ---
	MH_DisableHook :: proc(pTarget: windows.LPVOID) -> MH_STATUS ---
	MH_QueueEnableHook :: proc(pTarget: windows.LPVOID) -> MH_STATUS ---
	MH_QueueDisableHook :: proc(pTarget: windows.LPVOID) -> MH_STATUS ---
	MH_ApplyQueued :: proc() -> MH_STATUS ---
	MH_StatusToString :: proc(status: MH_STATUS) -> cstring ---
}

package MainLoader_Odin

import "base:runtime"
import c "core:c/libc"
import "core:strings"
import "core:sys/windows"
import "core:thread"
import "dllLoader"
import config "dllLoaderConfig"
import "exports"
import "helpers"

g_pluginLoader: dllLoader.dllLoader
g_cfg: config.loaderConfig

setup :: proc() {
	g_cfg = config.loaderConfig_init("GlumityToolSuite2.ini")

	if g_cfg.useConsole && helpers.init_console() {
		exports.Glumity_printf("Initialized console...\n")
	}

	g_pluginLoader = dllLoader.dllLoader_create()

	dllLoader.dllLoader_getDllsToLoad(&g_pluginLoader, g_cfg.pluginPath, g_cfg.blockList)
	dllLoader.dllLoader_loadDlls(&g_pluginLoader)

	if len(g_pluginLoader.loadedDlls) <= 0 {
		exports.Glumity_printf(
			"No plugins found [%s\b]!\n",
			strings.unsafe_string_to_cstring(g_cfg.pluginPath),
		)
	} else {
		exports.Glumity_printf("Loaded dlls: \n")
		for hModule in g_pluginLoader.loadedDlls {
			exports.Glumity_printf("%p\n", hModule)
		}
		dllLoader.dllLoader_call_exported_from_dlls(&g_pluginLoader, dllLoader.GLUMITY_ENTRY)
	}
}

cleanup :: proc() {
	defer config.loaderConfig_unload(&g_cfg)
	defer dllLoader.dllLoader_dispose(&g_pluginLoader)
}

setup_thread_adapter :: proc "std" (param: rawptr) -> u32 {
	context = runtime.default_context()
	setup() // call your original setup function
	return 0 // thread exit code
}

main :: proc() {
	hThread := windows.CreateThread(
		nil, // lpThreadAttributes
		0, // dwStackSize
		setup_thread_adapter, // lpStartAddress
		nil, // lpParameter
		0, // dwCreationFlags
		nil, // lpThreadId
	)
	windows.CloseHandle(hThread)
}

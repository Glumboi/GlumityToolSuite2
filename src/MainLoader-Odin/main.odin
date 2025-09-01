package MainLoader_Odin

import c "core:c/libc"
import "core:strings"
import "core:sys/windows"
import "core:thread"
import "dllLoader"
import config "dllLoaderConfig"
import "exports"
import "helpers"

setup :: proc() {
	cfg := config.loaderConfig_init("glumityv2conf.ini")

	if cfg.useConsole && helpers.init_console() {
		exports.Glumity_printf("Initialized console...\n")
	}

	pluginLoader := dllLoader.dllLoader_create()
	exports.bind_global_loader(&pluginLoader)
	defer dllLoader.dllLoader_dispose(&pluginLoader)

	dllLoader.dllLoader_getDllsToLoad(&pluginLoader, "./Plugins")
	dllLoader.dllLoader_loadDlls(&pluginLoader)

	exports.Glumity_printf("Loaded dlls: \n")
	for hModule in pluginLoader.loadedDlls {
		exports.Glumity_printf("%p\n", hModule)
	}
	dllLoader.dllLoader_call_exported_from_dlls(&pluginLoader, "GlumityMain")
}

main :: proc() {
	thread.run(setup, context)
}

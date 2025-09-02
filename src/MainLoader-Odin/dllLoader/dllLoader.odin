package dllLoader

import "../exports"
import "../helpers"
import c "core:c/libc"
import "core:os"
import "core:strings"
import "core:sys/windows"

dllLoader_generic_function :: distinct proc()

dllLoader :: struct {
	loadedDlls: [dynamic]windows.HMODULE,
	dllsToLoad: map[string]bool,
}

dllLoader_create :: proc() -> dllLoader {
	return {
		loadedDlls = make_dynamic_array([dynamic]windows.HMODULE),
		dllsToLoad = make_map(map[string]bool),
	}
}

dllLoader_getDllsToLoad :: proc(
	loader: ^dllLoader,
	locationToSearch: string,
	blockList: []string,
) {
	if loader == nil {return}

	if !os.exists(locationToSearch) || !os.is_dir(locationToSearch) {
		exports.Glumity_printf(
			"Directory %s does not exist, creating it...\n",
			strings.unsafe_string_to_cstring(locationToSearch),
		)
		os.make_directory(locationToSearch)
		return
	}

	// Correctly open the directory handle
	dirHandle, openErr := os.open(locationToSearch)
	if openErr != nil {
		exports.Glumity_printf(
			"Failed to open directory %s\n",
			strings.unsafe_string_to_cstring(locationToSearch),
		)
		return
	}
	defer os.close(dirHandle)

	allocator := os.heap_allocator

	files, readErr := os.read_dir(dirHandle, -1)
	if readErr != nil {
		exports.Glumity_printf(
			"Error reading directory (%s)!\n",
			strings.unsafe_string_to_cstring(locationToSearch),
		)
	}

	if len(files) == 0 {
		return
	}

	for &file in files {
		if helpers.has_file_extension(&file, ".dll") {
			if helpers.is_in_slice([]string, blockList, file.name) {
				exports.Glumity_printf(
					"Blocked dll: %s\n",
					strings.unsafe_string_to_cstring(file.name),
				)
				continue
			}
			loader.dllsToLoad[file.fullpath] = true
		}
	}
}

dllLoader_loadDlls :: proc(loader: ^dllLoader) {
	if loader == nil || len(loader.dllsToLoad) <= 0 {return}
	for k, v in loader.dllsToLoad {
		exports.Glumity_printf("Trying to load: %s\n", strings.unsafe_string_to_cstring(k))
		mod := windows.LoadLibraryA(strings.unsafe_string_to_cstring(k))
		if mod == nil {
			lastErr := windows.GetLastError()
			exports.Glumity_printf(
				"Could not load library [%s] error: %d\n",
				strings.unsafe_string_to_cstring(k),
				lastErr,
			)
			continue
		}

		append_elem(&loader.loadedDlls, mod)
	}
}

dllLoader_call_exported_from_dlls :: proc(
	loader: ^dllLoader,
	exportToCall: string,
) -> (
	success: bool,
) {
	if loader == nil {
		return false
	}

	for dll in loader.loadedDlls {
		procAdd := windows.GetProcAddress(dll, strings.unsafe_string_to_cstring(exportToCall))
		if procAdd == nil {
			continue
		}

		dllLoader_generic_function(procAdd)()
	}

	return true
}

dllLoader_dispose :: proc(loader: ^dllLoader) {
	if loader == nil {return}
	delete_dynamic_array(loader.loadedDlls)
	delete_map(loader.dllsToLoad)
}

package dllLoaderConfig

import "../exports"
import "base:runtime"
import "core:encoding/ini"
import "core:os"
import "core:strconv"
import "core:strings"

@(private)
DEFAULT_CONFIG: string : "[config]\nuseConsole=1\nblockList=\n"

loaderConfig :: struct {
	cfg:        ini.Map,
	useConsole: bool,
	blockList:  []string,
}

loaderConfig_init :: proc(path: string) -> loaderConfig {
	returnVal: loaderConfig = {
		useConsole = true,
	}

	if !os.exists(path) {
		if !os.write_entire_file(path, transmute([]u8)DEFAULT_CONFIG) {
			exports.Glumity_printf(
				"Could not create default ini at: %s!\n",
				strings.unsafe_string_to_cstring(path),
			)
		}

		exports.Glumity_printf(
			"Created default ini config: %s!\n",
			strings.unsafe_string_to_cstring(path),
		)
	}

	fileData, success := os.read_entire_file_from_filename(path)
	if !success {
		exports.Glumity_printf(
			"Could not read ini at: %s!\n",
			strings.unsafe_string_to_cstring(path),
		)
	}

	err: runtime.Allocator_Error
	returnVal.cfg, err = ini.load_map_from_string(transmute(string)fileData, context.allocator)

	if err != nil {
		exports.Glumity_printf("Error in loading config data!\n")
		return returnVal
	}

	exports.Glumity_printf("Config!\n")
	returnVal.useConsole, _ = strconv.parse_bool(returnVal.cfg["config"]["useConsole"])
    // TODO: fix split only splitting last occurance
	returnVal.blockList, err = strings.split(returnVal.cfg["config"]["blockList"], ",")

	if err != nil {
		exports.Glumity_printf("Error in reading blocklist from config!\n")
	}

	return returnVal
}

loaderConfig_unload :: proc(loader: ^loaderConfig) {
	if loader == nil {return}
	delete(loader.blockList)
}

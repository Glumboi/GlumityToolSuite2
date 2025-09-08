package backCompat

import "../helpers"
import c "core:c/libc"
import "core:os"
import "core:strings"

@(private)
REQUEST_ENDING :: ".offsetRequests.json"

is_old_version_plugin :: proc(
	dll: string,
	dllDir: string,
) -> (
	isOld: bool,
	jsonRequestFile: string,
) {
	// Remove extension
	if dll == "" {return false, ""}
	name := helpers.get_file_name(helpers.remove_file_extension(dll))

	if os.exists(dllDir) && os.is_dir(dllDir) {
		jsonFile := strings.concatenate(
			{dllDir, "\\", name, REQUEST_ENDING},
		)
		if os.exists(jsonFile) {
			return true, jsonFile
		}
	}

	return false, ""
}

package helpers

import "core:os"
import "core:path/filepath"
import "core:strings"

has_file_extension :: proc(file: ^os.File_Info, fileExtension: string) -> bool {
	if file.is_dir {return false}
	// get extension
	extension := file.name[strings.last_index_byte(file.name, byte('.')):len(file.name)]
	return extension == fileExtension
}

remove_file_extension :: proc(file: string) -> string {
	slice := file[0:strings.last_index(file, ".")]
	return slice
}

get_file_name :: proc(file: string) -> string {
	if len(file) <= 0 {
		return ""
	}

	return file[strings.last_index(file, "/"):]
}

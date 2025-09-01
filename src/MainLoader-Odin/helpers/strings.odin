package helpers

import c "core:c/libc"
import "core:os"
import "core:strings"

has_file_extension :: proc(file: ^os.File_Info, fileExtension: string) -> bool {
	if file.is_dir {return false}
	// get extension
	extension := file.name[strings.last_index_byte(file.name, byte('.')):len(file.name)]
	return extension == fileExtension
}

package exports

import c "core:c/libc"
import "core:fmt"
import "core:strconv"
import "core:strings"

global_loader: rawptr

bind_global_loader :: proc(loaderPtr: rawptr) {
	global_loader = loaderPtr
}

@(export)
GetLoaderInstance :: proc() -> rawptr {
	return global_loader
} // add compatibility for old plugins

@(export)
Glumity_printf :: proc(format: cstring, args: ..any) {
	c.printf("[GlumityV2]: ")
	str_builder := strings.builder_make()
	defer strings.builder_destroy(&str_builder)
	for arg in args {
		s := fmt.tprintf("%v", arg) // turn `any` into a string
		strings.write_string(&str_builder, s)
		strings.write_string(&str_builder, " ")
	}
	c.printf(format, strings.to_string(str_builder))
}

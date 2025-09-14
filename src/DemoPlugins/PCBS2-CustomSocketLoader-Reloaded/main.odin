package main

import c "core:c/libc"

import "minhook"

CpuSocket_GetUiName :: distinct proc(type: i32, result: cstring)

@(export)
GlumityMain :: proc() {
	init := minhook.MH_Initialize()
	if init != .MH_OK {
		return
	}

    
}

main :: proc() {

}

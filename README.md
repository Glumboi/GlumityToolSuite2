# GlumityToolSuite2
A work in progress, lightweight-ish suite for modifying il2cpp unity games

# Features/Startup steps
- A "version.dll" preloader that will load the main ModLoader
- A mainloader as Glumity "GlumityToolSuite2.dll"
- An IL2CPP runtime dumper as "GlumityV2IL2CPPDumper.dll" (it is more of a function pointer lookup helper than an actual dumper, might get expanded though)
- An IL2CPPAPIBridge as "IL2CPPAPIBridge.dll", this is essentially our runtime compiler, embeds a tcc enviornment tailored to work with GlumityV2

## Requirements
- Target Unity Game compiled with IL2CPP (x64 recommended).
- Windows 7 SP1 or higher (x64).
- C/C++ Runtime libraries compatible with the Tiny C Compiler (TCC) for JIT scripting.

# GlumityToolSuite2.dll
 - Works on its own already, heavily restricted and basically just a .dll loader - can be injected via the "version.dll" proxy or external tools, personal preference.
 
# Hooking IL2CPP code (using IL2CPPDumper and GlumityLib)
- "GlumityV2IL2CPPDumper.dll" to be present within the "Plugins" folder, for examples of utilizing it, refer to DemoPlugins or to the example JIT scripts.

# Hooking IL2CPP code (using IL2CPPAPIBridge and GlumityLib)
- "IL2CPPDumper.dll" in the "Plugins" folder and its requirements.
- A valid TCC enviornment, for structure, view "Directory Structure" 

## Directory Structure
Upon first initialization, the suite creates or expects the following structural hierarchy inside the game directory:
```text
TargetGame/
├── GameAssembly.dll
├── version.dll                  	<- Proxy Preloader
├── GlumityToolSuite2.dll    		<- Main Mod Loader
└── Plugins/                 		<- Compiled Native DLL Plugins (.dll)
    ├── GlumityV2IL2CPPDumper.dll	
    └── IL2CPPAPIBridge.dll
	└── jit_scripts/ 		 		<- C Source Scripts (.c) compiled at runtime via TCC
	└── tcc_include/ 		
	|	└── include/ 		 		<- TCC include location (TCC here has a custom GlumityLib version, it can be changed how you want)
	└── tcc_libs/ 		 
		└── lib/ 		 			<- TCC library files location (e.g: user32.def, libtcc1-64.a)
```